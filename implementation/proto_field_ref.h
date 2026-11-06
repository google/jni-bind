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

#ifndef JNI_BIND_IMPLEMENTATION_PROTO_FIELD_REF_H_
#define JNI_BIND_IMPLEMENTATION_PROTO_FIELD_REF_H_

// IWYU pragma: private, include "third_party/jni_wrapper/proto_bind.h"

#include <algorithm>
#include <cstddef>
#include <mutex>
#include <optional>
#include <ranges>  // NOLINT(build/c++20)
#include <type_traits>
#include <utility>
#include <vector>

#include "bind_lang/id_type.h"
#include "bind_lang/metaprogramming/double_locked_value.h"
#include "bind_lang/ref_base.h"
#include "implementation/descriptor_ref.h"
#include "implementation/proto_helper/field_value.h"
#include "implementation/proto_helper/proto_helper.h"
#include "implementation/proto_id.h"
#include "implementation/proxy_definitions_proto.h"
#include "implementation/proxy_temporary.h"
#include "proto_dep.h"

namespace proto {

// Tag indicating that a `proto::Message` wraps a non-owning pointer (e.g. a
// mutable sub-message owned by an enclosing parent `google::protobuf::Message`
// or an externally managed message).
struct Unowned {};

inline auto& GetDefaultLoadedFieldDescriptorList() {
  static auto* ret_val =
      new std::vector<bind_lang::metaprogramming::DoubleLockedValue<
          const google::protobuf::FieldDescriptor*>*>{};
  return *ret_val;
}

// Resets all cached message and field descriptors. Intended exclusively for
// single-threaded unit test teardown (mirrors `JvmRef::~JvmRef`). Calling this
// from concurrent tests could race with `DoubleLockedValue::LoadAndMaybeInit`
// in other threads, so test execution should remain sequential.
inline void ReleaseAllProtoDescriptorsForTests() {
  std::lock_guard<std::mutex> lock(  // NOLINT(build/c++11)
      GetDescriptorCacheMutex());
  std::ranges::for_each(  // NOLINT(build/c++20)
      GetDefaultLoadedDescriptorList(),
      [](auto* desc_slot) { desc_slot->Reset(); });
  GetDefaultLoadedDescriptorList().clear();

  std::ranges::for_each(  // NOLINT(build/c++20)
      GetDefaultLoadedFieldDescriptorList(),
      [](auto* field_slot) { field_slot->Reset(); });
  GetDefaultLoadedFieldDescriptorList().clear();
}

// Compile-time argument viability validator for `ProtoFieldRef`.
// Mirrors `jni::FieldSelection` in `implementation/field_selection.h`.
template <typename ProtoT_, std::size_t field_idx_>
struct ProtoFieldSelection {
  using IdT = ProtoId<ProtoT_, bind_lang::IdType::FIELD, field_idx_>;

  template <typename T>
  static constexpr bool ElementValable() {
    using DecayT = std::decay_t<T>;
    return ProtoProxy_t<typename IdT::RawValT>::template kViable<IdT, DecayT>;
  }

  template <typename T>
  static constexpr bool RepeatedRangeValable() {
    if constexpr (!IdT::kIsRepeated) {
      return false;
    } else if constexpr (std::ranges::input_range<T> &&  // NOLINT(build/c++20)
                         !ElementValable<T>()) {
      // Prevents treating types like std::string (which satisfy input_range) as
      // a range of their constituent characters instead of as a single element.
      return ElementValable<
          std::ranges::range_value_t<T>>();  // NOLINT(build/c++20)
    } else {
      return false;
    }
  }

  template <typename T>
  static constexpr bool ArgValable() {
    using DecayT = std::decay_t<T>;
    if constexpr (std::is_same_v<DecayT, std::nullopt_t>) {
      return IdT::kIsOptional || IdT::kIsRepeated;
    } else if constexpr (IdT::kIsRepeated) {
      return RepeatedRangeValable<T>();
    } else {
      return ElementValable<T>();
    }
  }

  template <typename... Ts>
  static constexpr bool Valable() {
    return sizeof...(Ts) == 1 && (ArgValable<Ts>() && ...);
  }
};

// Represents a live bound reference to Field `I` of a `proto::Message`.
// Mirrors `jni::FieldRef` in `implementation/field_ref.h`.
template <typename ProtoT_, std::size_t I>
class ProtoFieldRef {
 public:
  using IdT = ProtoId<ProtoT_, bind_lang::IdType::FIELD, I>;
  using FieldSelectionT = ProtoFieldSelection<ProtoT_, I>;
  using CDeclT = typename IdT::MaterializeCDeclT;
  using ReturnProxied = ProtoReturn_t<CDeclT, IdT>;

  explicit ProtoFieldRef(const google::protobuf::Descriptor* descriptor_ref,
                         google::protobuf::Message* message_ref)
      : descriptor_ref_(descriptor_ref), message_ref_(message_ref) {}

  ProtoFieldRef(const ProtoFieldRef&) = delete;
  ProtoFieldRef(ProtoFieldRef&&) = delete;
  void operator=(const ProtoFieldRef&) = delete;

  static const google::protobuf::FieldDescriptor* GetFieldDescriptor(
      const google::protobuf::Descriptor* descriptor) {
    static bind_lang::metaprogramming::DoubleLockedValue<
        const google::protobuf::FieldDescriptor*>
        return_value;

    return return_value.LoadAndMaybeInit([=]() {
      {
        std::lock_guard<std::mutex> lock(  // NOLINT(build/c++11)
            GetDescriptorCacheMutex());
        GetDefaultLoadedFieldDescriptorList().push_back(&return_value);
      }
      return ProtoHelper::GetFieldDescriptor(
          descriptor, static_cast<int>(IdT::kTag), IdT::Name());
    });
  }

  bool Has() const {
    static_assert(!IdT::kIsRepeated,
                  "Has() is only valid on singular (Optional/Required) fields; "
                  "use Size() for Repeated fields.");
    return ProtoFieldHelper<CDeclT, 0>::HasValue(
        message_ref_, GetFieldDescriptor(descriptor_ref_));
  }

  void Clear() {
    ProtoFieldHelper<CDeclT, IdT::kRank>::ClearValue(
        message_ref_, GetFieldDescriptor(descriptor_ref_));
  }

  ReturnProxied Get() const {
    static_assert(!IdT::kIsRepeated,
                  "Get() is only valid on singular fields; use At(index) or "
                  "Get(index) for Repeated fields.");
    if constexpr (std::is_base_of_v<bind_lang::RefBaseBase, ReturnProxied>) {
      return ReturnProxied{
          Unowned{}, ProtoFieldHelper<CDeclT, 0>::MutableValue(
                         message_ref_, GetFieldDescriptor(descriptor_ref_))};
    } else {
      return ReturnProxied{ProtoFieldHelper<CDeclT, 0>::GetValue(
          message_ref_, GetFieldDescriptor(descriptor_ref_))};
    }
  }

  std::optional<ReturnProxied> GetOptional() const {
    static_assert(IdT::kIsOptional,
                  "GetOptional() is only valid on proto::Optional fields.");
    if (!Has()) {
      return std::nullopt;
    }
    return Get();
  }

  template <typename T>
  void Set(T&& value) {
    static_assert(FieldSelectionT::template Valable<T>(),
                  "Invalid argument type for proto::ProtoFieldRef::Set.");
    if constexpr (std::is_same_v<std::decay_t<T>, std::nullopt_t>) {
      Clear();
    } else if constexpr (FieldSelectionT::template RepeatedRangeValable<T>()) {
      Clear();
      using ElemT = std::ranges::range_value_t<  // NOLINT(build/c++20)
          std::remove_cvref_t<T>>;
      std::ranges::for_each(  // NOLINT(build/c++20)
          value, [this](auto&& elem) {
            if constexpr (std::is_same_v<ElemT, bool>) {
              this->Add(static_cast<bool>(elem));
            } else {
              this->Add(std::forward<decltype(elem)>(elem));
            }
          });
    } else if constexpr (!IdT::kIsRepeated) {
      ProtoFieldHelper<CDeclT, 0>::SetValue(
          message_ref_, GetFieldDescriptor(descriptor_ref_),
          jni::ForwardWithProxyTemporaryStrip(
              ProtoProxy_t<CDeclT>::ProxyAsArg(std::forward<T>(value))));
    }
  }

  int Size() const {
    static_assert(IdT::kIsRepeated,
                  "Size() is only valid on proto::Repeated fields.");
    return ProtoFieldHelper<CDeclT, 1>::Size(
        message_ref_, GetFieldDescriptor(descriptor_ref_));
  }

  ReturnProxied At(int index) {
    static_assert(IdT::kIsRepeated,
                  "At(index) is only valid on proto::Repeated fields.");
    if constexpr (std::is_base_of_v<bind_lang::RefBaseBase, ReturnProxied>) {
      return ReturnProxied{
          Unowned{},
          ProtoFieldHelper<CDeclT, 1>::MutableRepeatedValue(
              message_ref_, GetFieldDescriptor(descriptor_ref_), index)};
    } else {
      return ReturnProxied{ProtoFieldHelper<CDeclT, 1>::GetRepeatedValue(
          message_ref_, GetFieldDescriptor(descriptor_ref_), index)};
    }
  }

  ReturnProxied Get(int index) { return At(index); }

  ReturnProxied operator[](int index) { return At(index); }

  template <typename T>
  void Set(int index, T&& value) {
    static_assert(IdT::kIsRepeated,
                  "Set(index, value) is only valid on proto::Repeated fields.");
    static_assert(FieldSelectionT::template ElementValable<T>(),
                  "Invalid element type for proto::ProtoFieldRef::Set.");
    ProtoFieldHelper<CDeclT, 1>::SetRepeatedValue(
        message_ref_, GetFieldDescriptor(descriptor_ref_), index,
        jni::ForwardWithProxyTemporaryStrip(
            ProtoProxy_t<CDeclT>::ProxyAsArg(std::forward<T>(value))));
  }

  ReturnProxied Add() {
    static_assert(IdT::kIsRepeated &&
                      std::is_base_of_v<bind_lang::RefBaseBase, ReturnProxied>,
                  "Zero-argument Add() is only valid on proto::Repeated "
                  "SubMessage fields.");
    return ReturnProxied{
        Unowned{}, ProtoFieldHelper<CDeclT, 1>::AddMutableValue(
                       message_ref_, GetFieldDescriptor(descriptor_ref_))};
  }

  template <typename T>
  void Add(T&& value) {
    static_assert(IdT::kIsRepeated,
                  "Add() is only valid on proto::Repeated fields.");
    static_assert(FieldSelectionT::template ElementValable<T>(),
                  "Invalid argument type for proto::ProtoFieldRef::Add.");
    ProtoFieldHelper<CDeclT, 1>::AddValue(
        message_ref_, GetFieldDescriptor(descriptor_ref_),
        jni::ForwardWithProxyTemporaryStrip(
            ProtoProxy_t<CDeclT>::ProxyAsArg(std::forward<T>(value))));
  }

 private:
  const google::protobuf::Descriptor* descriptor_ref_;
  google::protobuf::Message* message_ref_;
};

}  // namespace proto

#endif  // JNI_BIND_IMPLEMENTATION_PROTO_FIELD_REF_H_
