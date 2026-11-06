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

#ifndef JNI_BIND_IMPLEMENTATION_PROTO_FIELD_H_
#define JNI_BIND_IMPLEMENTATION_PROTO_FIELD_H_

// IWYU pragma: private, include "third_party/jni_wrapper/proto_bind.h"

#include <cstddef>
#include <type_traits>

#include "bind_lang/metaprogramming/string_literal.h"

namespace proto {

enum class FieldLabel {
  OPTIONAL,
  REQUIRED,
  REPEATED,
};

struct ProtoFieldBase {};

// Maps non-literal C++ declaration types (such as `std::string`) to a literal,
// trivially-destructible placeholder (`const char*`) so field descriptors can
// be stored inside `static constexpr proto::Template` and used as C++20
// structural non-type template parameters.
template <typename T>
struct RawStorageHelper {
  using type =
      std::conditional_t<std::is_trivially_destructible_v<T>, T, const char*>;
};

template <typename T>
using RawStorage_t = typename RawStorageHelper<T>::type;

template <std::size_t kTag_, FieldLabel kLabel_, typename Raw_,
          bind_lang::metaprogramming::StringLiteral kNameLiteral_>
struct ProtoFieldSpec : public ProtoFieldBase {
  static constexpr std::size_t kTag = kTag_;
  static constexpr FieldLabel kLabel = kLabel_;
  using Raw = Raw_;
  using StorageT = RawStorage_t<Raw_>;

  const char* name_ = kNameLiteral_.value;
  const StorageT raw_ = {};

  constexpr ProtoFieldSpec() : name_(kNameLiteral_.value), raw_{} {}
  constexpr explicit ProtoFieldSpec(const char* name) : name_(name), raw_{} {}
  constexpr ProtoFieldSpec(const char* name, StorageT value_raw)
      : name_(name), raw_(value_raw) {}
};

template <std::size_t kTag_, typename Raw_,
          bind_lang::metaprogramming::StringLiteral kNameLiteral_ = "">
struct Optional
    : public ProtoFieldSpec<kTag_, FieldLabel::OPTIONAL, Raw_, kNameLiteral_> {
  using Base = ProtoFieldSpec<kTag_, FieldLabel::OPTIONAL, Raw_, kNameLiteral_>;
  using Base::Base;
};

template <std::size_t kTag_, typename Raw_,
          bind_lang::metaprogramming::StringLiteral kNameLiteral_ = "">
struct Required
    : public ProtoFieldSpec<kTag_, FieldLabel::REQUIRED, Raw_, kNameLiteral_> {
  using Base = ProtoFieldSpec<kTag_, FieldLabel::REQUIRED, Raw_, kNameLiteral_>;
  using Base::Base;
};

template <std::size_t kTag_, typename Raw_,
          bind_lang::metaprogramming::StringLiteral kNameLiteral_ = "">
struct Repeated
    : public ProtoFieldSpec<kTag_, FieldLabel::REPEATED, Raw_, kNameLiteral_> {
  using Base = ProtoFieldSpec<kTag_, FieldLabel::REPEATED, Raw_, kNameLiteral_>;
  using Base::Base;
};

}  // namespace proto

#endif  // JNI_BIND_IMPLEMENTATION_PROTO_FIELD_H_
