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

#ifndef JNI_BIND_PROTO_BIND_IMPLEMENTATION_PROTO_HELPER_PROTO_HELPER_H_
#define JNI_BIND_PROTO_BIND_IMPLEMENTATION_PROTO_HELPER_PROTO_HELPER_H_

#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>

#include "google/protobuf/descriptor.h" // IWYU pragma: keep
#include "google/protobuf/message.h" // IWYU pragma: keep
#include "google/protobuf/reflection.h" // IWYU pragma: keep

namespace proto {
namespace helper {

template <typename CppType>
void SetField(google::protobuf::Message* msg, const char* field_name, CppType value) { // NOLINT
  const google::protobuf::Descriptor* desc = msg->GetDescriptor(); // NOLINT
  const google::protobuf::FieldDescriptor* field = desc->FindFieldByName(field_name); // NOLINT
  if (!field) return;

  const google::protobuf::Reflection* ref = msg->GetReflection(); // NOLINT

  if constexpr (std::is_same_v<CppType, int32_t>) {
    ref->SetInt32(msg, field, value);
  } else if constexpr (std::is_same_v<CppType, std::string_view> ||
                       std::is_same_v<CppType, const char*> ||
                       std::is_same_v<CppType, std::string>) {
    ref->SetString(msg, field, std::string(value));
  }
}

template <typename CppType>
CppType GetField(const google::protobuf::Message* msg, const char* field_name) { // NOLINT
  const google::protobuf::Descriptor* desc = msg->GetDescriptor(); // NOLINT
  const google::protobuf::FieldDescriptor* field = desc->FindFieldByName(field_name); // NOLINT
  if (!field) return CppType{};

  const google::protobuf::Reflection* ref = msg->GetReflection(); // NOLINT

  if constexpr (std::is_same_v<CppType, int32_t>) {
    return ref->GetInt32(*msg, field);
  } else if constexpr (std::is_same_v<CppType, std::string>) {
    return ref->GetString(*msg, field);
  }

  return CppType{};
}

}  // namespace helper
}  // namespace proto

#endif  // JNI_BIND_PROTO_BIND_IMPLEMENTATION_PROTO_HELPER_PROTO_HELPER_H_
