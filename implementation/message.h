/*
 * Copyright 2026 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JNI_BIND_IMPLEMENTATION_MESSAGE_H_
#define JNI_BIND_IMPLEMENTATION_MESSAGE_H_

// IWYU pragma: private, include "third_party/jni_wrapper/proto_bind.h"

#include <cstddef>
#include <optional>
#include <type_traits>
#include <utility>

#include "bind_lang/lifecycle.h"
#include "implementation/descriptor_ref.h"
#include "implementation/message_ref.h"
#include "implementation/proto_field_ref.h"
#include "implementation/proto_helper/lifecycle_proto.h"
#include "implementation/proto_type.h"
#include "proto_dep.h"

namespace proto {

// Tag indicating that a `proto::Message` adopts ownership of an existing
// heap-allocated `google::protobuf::Message*`.
struct AdoptLocal {};

// Tag indicating that a `proto::Message` deep-clones an existing
// `const google::protobuf::Message*` into a new owned message.
struct NewRef {};

// Scoped runtime Protocol Buffer message bound to a compile-time
// `proto::Template` schema `template_v_`.
// Mirrors `jni::LocalObject` in `implementation/local_object.h`.
//
// Example:
//   static constexpr proto::Template kTemplate{
//       proto::Optional<0, int>{"some_optional_int_field"},
//       proto::Required<1, std::string>{"some_string_field"},
//   };
//   proto::Message<kTemplate> runtime_msg{123, "string_val"};
template <const auto& template_v_>
class Message : public MessageRef<ProtoT<template_v_>> {
 public:
  using ProtoT_ = ProtoT<template_v_>;
  using Base = MessageRef<ProtoT_>;
  using SpanType = google::protobuf::Message*;
  using LifecycleT =
      bind_lang::LifecycleHelper<google::protobuf::Message*,
                                 bind_lang::LifecycleType::LOCAL>;

  // Default-constructs an empty message of `template_v_`'s schema.
  Message()
      : Base(LifecycleT::Construct(
            DescriptorRef<ProtoT_>::GetAndMaybeLoadDescriptor())),
        owns_message_(true) {}

  // Adopts ownership of an existing `google::protobuf::Message*`.
  Message(AdoptLocal, google::protobuf::Message* raw_message)
      : Base(raw_message), owns_message_(true) {}

  // Clones an existing `google::protobuf::Message*` into a new owned message.
  Message(NewRef, const google::protobuf::Message* raw_message)
      : Base(raw_message != nullptr ? LifecycleT::NewReference(raw_message)
                                    : nullptr),
        owns_message_(true) {}

  // Wraps a non-owning `google::protobuf::Message*` (such as a mutable
  // sub-message inside an enclosing parent proto).
  Message(Unowned, google::protobuf::Message* raw_message)
      : Base(raw_message), owns_message_(false) {}

  // Move constructor transfers ownership.
  Message(Message&& rhs) noexcept
      : Base(rhs.Release()), owns_message_(rhs.ReleaseOwnershipFlag()) {}

  template <const auto& rhs_template_v,
            typename = std::enable_if_t<rhs_template_v == template_v_>>
  Message(Message<rhs_template_v>&& rhs) noexcept
      : Base(rhs.Release()), owns_message_(rhs.ReleaseOwnershipFlag()) {}

  Message(const Message&) = delete;
  Message& operator=(const Message&) = delete;

  Message& operator=(Message&& rhs) noexcept {
    if (this != &rhs) {
      if (owns_message_ && Base::object_ref_ != nullptr) {
        LifecycleT::Delete(Base::object_ref_);
      }
      Base::object_ref_ = rhs.Release();
      owns_message_ = rhs.ReleaseOwnershipFlag();
    }
    return *this;
  }

  // Field-initializing constructor:
  //   proto::Message<kTemplate> msg{123, "string_val"};
  // Validates each argument against the corresponding field specification in
  // `template_v_` at compile time. `std::nullopt` may be passed for
  // `proto::Optional` or `proto::Repeated` fields to leave them unset.
  template <typename FirstArg, typename... RestArgs,
            typename = std::enable_if_t<
                (1 + sizeof...(RestArgs) == ProtoT_::kNumFields) &&
                !std::is_base_of_v<Base, std::decay_t<FirstArg>> &&
                !std::is_same_v<std::decay_t<FirstArg>, AdoptLocal> &&
                !std::is_same_v<std::decay_t<FirstArg>, NewRef> &&
                !std::is_same_v<std::decay_t<FirstArg>, Unowned>>>
  explicit Message(FirstArg&& first_arg, RestArgs&&... rest_args)
      : Base(LifecycleT::Construct(
            DescriptorRef<ProtoT_>::GetAndMaybeLoadDescriptor())),
        owns_message_(true) {
    static_assert(ValidateFieldArgs<FirstArg, RestArgs...>(
                      std::make_index_sequence<ProtoT_::kNumFields>{}),
                  "Invalid argument types passed to proto::Message constructor "
                  "(check field types and Required vs Optional constraints).");
    InitializeFields(std::make_index_sequence<ProtoT_::kNumFields>{},
                     std::forward<FirstArg>(first_arg),
                     std::forward<RestArgs>(rest_args)...);
  }

  ~Message() {
    if (owns_message_ && Base::object_ref_ != nullptr) {
      LifecycleT::Delete(Base::object_ref_);
    }
  }

 private:
  template <const auto&>
  friend class Message;

  bool ReleaseOwnershipFlag() noexcept {
    const bool prev = owns_message_;
    owns_message_ = false;
    return prev;
  }

  template <typename... Args, std::size_t... Is>
  static constexpr bool ValidateFieldArgs(std::index_sequence<Is...>) {
    return (ProtoFieldSelection<ProtoT_, Is>::template ArgValable<Args>() &&
            ...);
  }

  template <std::size_t I, typename Arg>
  void InitializeOneField(Arg&& arg) {
    if constexpr (!std::is_same_v<std::decay_t<Arg>, std::nullopt_t>) {
      this->template AccessAtIdx<I>().Set(std::forward<Arg>(arg));
    }
  }

  template <std::size_t... Is, typename... Args>
  void InitializeFields(std::index_sequence<Is...>, Args&&... args) {
    (InitializeOneField<Is>(std::forward<Args>(args)), ...);
  }

  bool owns_message_ = true;
};

}  // namespace proto

#endif  // JNI_BIND_IMPLEMENTATION_MESSAGE_H_
