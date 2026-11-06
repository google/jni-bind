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

#ifndef JNI_BIND_IMPLEMENTATION_PROTO_HELPER_PROTO_HELPER_H_
#define JNI_BIND_IMPLEMENTATION_PROTO_HELPER_PROTO_HELPER_H_

#include "bind_lang/metaprogramming/lambda_string.h"
#include "implementation/jni_helper/trace.h"
#include "implementation/proto_helper/fake_proto_test_constants.h"  // IWYU pragma: keep
#include "implementation/proto_helper/proto_env.h"
#include "proto_dep.h"

namespace proto {

// Porcelain helper shim for Protocol Buffer descriptor and field lookup.
// Mirrors `jni::JniHelper` in `implementation/jni_helper/jni_helper.h`.
class ProtoHelper {
 public:
  static const google::protobuf::Descriptor* FindDescriptor(const char* name) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("FindDescriptor")),
               name);

#ifdef DRY_RUN
    return jni::Fake<const google::protobuf::Descriptor*>();
#else
    return ProtoEnv::GetEnv()->FindMessageTypeByName(name);
#endif  // DRY_RUN
  }

  static const google::protobuf::Descriptor* RegisterDynamicDescriptor(
      const google::protobuf::FileDescriptorProto& file_proto,
      const char* message_name) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(
                   STR("RegisterDynamicDescriptor")),
               message_name);

#ifdef DRY_RUN
    return jni::Fake<const google::protobuf::Descriptor*>();
#else
    return ProtoEnv::GetEnv()->RegisterDynamicDescriptor(file_proto,
                                                         message_name);
#endif  // DRY_RUN
  }

  static const google::protobuf::FieldDescriptor* GetFieldDescriptor(
      const google::protobuf::Descriptor* descriptor, int field_number,
      const char* name) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("GetFieldDescriptor")),
        descriptor, field_number, name);

#ifdef DRY_RUN
    return jni::Fake<const google::protobuf::FieldDescriptor*>();
#else
    return ProtoEnv::GetEnv()->GetFieldDescriptor(descriptor, field_number,
                                                  name);
#endif  // DRY_RUN
  }
};

}  // namespace proto

#endif  // JNI_BIND_IMPLEMENTATION_PROTO_HELPER_PROTO_HELPER_H_
