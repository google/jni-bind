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

#ifndef JNI_BIND_IMPLEMENTATION_PROTO_HELPER_LIFECYCLE_PROTO_H_
#define JNI_BIND_IMPLEMENTATION_PROTO_HELPER_LIFECYCLE_PROTO_H_

#include <string>
#include <string_view>

#include "bind_lang/lifecycle.h"
#include "bind_lang/metaprogramming/lambda_string.h"
#include "implementation/jni_helper/trace.h"
#include "implementation/proto_helper/fake_proto_test_constants.h"  // IWYU pragma: keep
#include "implementation/proto_helper/proto_env.h"
#include "proto_dep.h"

namespace bind_lang {

// Specializes `bind_lang::LifecycleHelper` for `google::protobuf::Message*` so
// that the shared `RefBase` and JNI Bind's `Scoped`/`Entry` promotion mechanics
// can be reused directly by `proto::Message`. Specializations of `bind_lang`
// customization points must live in `namespace bind_lang`.
template <>
struct LifecycleHelper<google::protobuf::Message*, LifecycleType::LOCAL> {
  static google::protobuf::Message* Construct(
      const google::protobuf::Descriptor* descriptor) {
    ::jni::Trace(metaprogramming::LambdaToStr(STR("NewMessage")), descriptor);

#ifdef DRY_RUN
    return ::jni::Fake<google::protobuf::Message*>();
#else
    return proto::ProtoEnv::GetEnv()->NewMessage(descriptor);
#endif  // DRY_RUN
  }

  static google::protobuf::Message* NewReference(
      const google::protobuf::Message* message) {
    ::jni::Trace(metaprogramming::LambdaToStr(STR("CloneMessage")), message);

#ifdef DRY_RUN
    return ::jni::Fake<google::protobuf::Message*>();
#else
    return proto::ProtoEnv::GetEnv()->CloneMessage(message);
#endif  // DRY_RUN
  }

  static void Delete(google::protobuf::Message* message) {
    ::jni::Trace(metaprogramming::LambdaToStr(STR("DeleteMessage")), message);

#ifdef DRY_RUN
#else
    proto::ProtoEnv::GetEnv()->DeleteMessage(message);
#endif  // DRY_RUN
  }

  static std::string SerializeAsString(
      const google::protobuf::Message* message) {
    ::jni::Trace(metaprogramming::LambdaToStr(STR("SerializeAsString")),
                 message);

#ifdef DRY_RUN
    return "DRY_RUN_WIRE";
#else
    return proto::ProtoEnv::GetEnv()->SerializeAsString(message);
#endif  // DRY_RUN
  }

  static bool ParseFromString(google::protobuf::Message* message,
                              std::string_view data) {
    ::jni::Trace(metaprogramming::LambdaToStr(STR("ParseFromString")), message);

#ifdef DRY_RUN
    return true;
#else
    return proto::ProtoEnv::GetEnv()->ParseFromString(message, data);
#endif  // DRY_RUN
  }
};

}  // namespace bind_lang

#endif  // JNI_BIND_IMPLEMENTATION_PROTO_HELPER_LIFECYCLE_PROTO_H_
