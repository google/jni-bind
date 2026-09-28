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

#ifndef JNI_BIND_IMPLEMENTATION_PROTO_HELPER_FAKE_PROTO_TEST_CONSTANTS_H_
#define JNI_BIND_IMPLEMENTATION_PROTO_HELPER_FAKE_PROTO_TEST_CONSTANTS_H_

#include <cstdint>

#include "implementation/jni_helper/fake_test_constants.h"  // IWYU pragma: export
#include "implementation/proto_helper/proto_env.h"
#include "proto_dep.h"

namespace jni {

template <>
struct FakeImpl<google::protobuf::Descriptor> {
  static intptr_t Val(int offset) {
    return static_cast<intptr_t>(0xD0C0D0C0D000LL) + offset;
  }
};

template <>
struct FakeImpl<const google::protobuf::Descriptor> {
  static intptr_t Val(int offset) {
    return static_cast<intptr_t>(0xD0C0D0C0D000LL) + offset;
  }
};

template <>
struct FakeImpl<google::protobuf::FieldDescriptor> {
  static intptr_t Val(int offset) {
    return static_cast<intptr_t>(0xF1E1DF1E1D00LL) + offset;
  }
};

template <>
struct FakeImpl<const google::protobuf::FieldDescriptor> {
  static intptr_t Val(int offset) {
    return static_cast<intptr_t>(0xF1E1DF1E1D00LL) + offset;
  }
};

template <>
struct FakeImpl<google::protobuf::Message> {
  static intptr_t Val(int offset) {
    return static_cast<intptr_t>(0xB0B0B0B0B000LL) + offset;
  }
};

template <>
struct FakeImpl<const google::protobuf::Message> {
  static intptr_t Val(int offset) {
    return static_cast<intptr_t>(0xB0B0B0B0B000LL) + offset;
  }
};

template <>
struct FakeImpl<proto::ProtoEnvInterface> {
  static intptr_t Val(int offset) {
    return static_cast<intptr_t>(0xE0E0E0E0E000LL) + offset;
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_PROTO_HELPER_FAKE_PROTO_TEST_CONSTANTS_H_
