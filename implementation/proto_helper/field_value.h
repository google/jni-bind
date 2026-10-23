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

#ifndef JNI_BIND_IMPLEMENTATION_PROTO_HELPER_FIELD_VALUE_H_
#define JNI_BIND_IMPLEMENTATION_PROTO_HELPER_FIELD_VALUE_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

#include "bind_lang/metaprogramming/lambda_string.h"
#include "implementation/jni_helper/trace.h"
#include "implementation/proto_helper/fake_proto_test_constants.h"  // IWYU pragma: keep
#include "implementation/proto_helper/proto_env.h"
#include "proto_dep.h"

namespace proto {

// Shared presence/clear base for all ProtoFieldHelper specializations.
struct ProtoFieldPresenceHelper {
  static bool HasValue(const google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("HasField")),
               message_ref, field_ref);
#ifdef DRY_RUN
    return true;
#else
    return ProtoEnv::GetEnv()->HasField(message_ref, field_ref);
#endif  // DRY_RUN
  }

  static void ClearValue(google::protobuf::Message* message_ref,
                         const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("ClearField")),
               message_ref, field_ref);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->ClearField(message_ref, field_ref);
#endif  // DRY_RUN
  }
};

// Porcelain getter/setter specializations per canonical Proto C++ type.
// Mirrors `jni::FieldHelper` in `implementation/jni_helper/field_value.h`.
template <typename Raw, std::size_t kRank = 0, typename Enable = void>
struct ProtoFieldHelper;

template <>
struct ProtoFieldHelper<bool, 0, void> : public ProtoFieldPresenceHelper {
  static bool GetValue(const google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("GetBoolField")),
               message_ref, field_ref);
#ifdef DRY_RUN
    return false;
#else
    return ProtoEnv::GetEnv()->GetBoolField(message_ref, field_ref);
#endif  // DRY_RUN
  }

  static void SetValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       bool value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("SetBoolField")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetBoolField(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<int32_t, 0, void> : public ProtoFieldPresenceHelper {
  static int32_t GetValue(const google::protobuf::Message* message_ref,
                          const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("GetInt32Field")),
               message_ref, field_ref);
#ifdef DRY_RUN
    return 123;
#else
    return ProtoEnv::GetEnv()->GetInt32Field(message_ref, field_ref);
#endif  // DRY_RUN
  }

  static void SetValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       int32_t value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("SetInt32Field")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetInt32Field(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<uint32_t, 0, void> : public ProtoFieldPresenceHelper {
  static uint32_t GetValue(const google::protobuf::Message* message_ref,
                           const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("GetUInt32Field")),
               message_ref, field_ref);
#ifdef DRY_RUN
    return 123U;
#else
    return ProtoEnv::GetEnv()->GetUInt32Field(message_ref, field_ref);
#endif  // DRY_RUN
  }

  static void SetValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       uint32_t value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("SetUInt32Field")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetUInt32Field(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<int64_t, 0, void> : public ProtoFieldPresenceHelper {
  static int64_t GetValue(const google::protobuf::Message* message_ref,
                          const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("GetInt64Field")),
               message_ref, field_ref);
#ifdef DRY_RUN
    return 123LL;
#else
    return ProtoEnv::GetEnv()->GetInt64Field(message_ref, field_ref);
#endif  // DRY_RUN
  }

  static void SetValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       int64_t value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("SetInt64Field")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetInt64Field(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<uint64_t, 0, void> : public ProtoFieldPresenceHelper {
  static uint64_t GetValue(const google::protobuf::Message* message_ref,
                           const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("GetUInt64Field")),
               message_ref, field_ref);
#ifdef DRY_RUN
    return 123ULL;
#else
    return ProtoEnv::GetEnv()->GetUInt64Field(message_ref, field_ref);
#endif  // DRY_RUN
  }

  static void SetValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       uint64_t value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("SetUInt64Field")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetUInt64Field(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<float, 0, void> : public ProtoFieldPresenceHelper {
  static float GetValue(const google::protobuf::Message* message_ref,
                        const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("GetFloatField")),
               message_ref, field_ref);
#ifdef DRY_RUN
    return 123.f;
#else
    return ProtoEnv::GetEnv()->GetFloatField(message_ref, field_ref);
#endif  // DRY_RUN
  }

  static void SetValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       float value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("SetFloatField")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetFloatField(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<double, 0, void> : public ProtoFieldPresenceHelper {
  static double GetValue(const google::protobuf::Message* message_ref,
                         const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("GetDoubleField")),
               message_ref, field_ref);
#ifdef DRY_RUN
    return 123.;
#else
    return ProtoEnv::GetEnv()->GetDoubleField(message_ref, field_ref);
#endif  // DRY_RUN
  }

  static void SetValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       double value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("SetDoubleField")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetDoubleField(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<std::string, 0, void>
    : public ProtoFieldPresenceHelper {
  static std::string GetValue(
      const google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("GetStringField")),
               message_ref, field_ref);
#ifdef DRY_RUN
    return "DEAD_BEEF";
#else
    return ProtoEnv::GetEnv()->GetStringField(message_ref, field_ref);
#endif  // DRY_RUN
  }

  static void SetValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       std::string value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("SetStringField")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetStringField(message_ref, field_ref,
                                       std::move(value));
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<google::protobuf::Message*, 0, void>
    : public ProtoFieldPresenceHelper {
  static const google::protobuf::Message* GetValue(
      const google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("GetMessageField")),
               message_ref, field_ref);
#ifdef DRY_RUN
    return jni::Fake<const google::protobuf::Message*>();
#else
    return ProtoEnv::GetEnv()->GetMessageField(message_ref, field_ref);
#endif  // DRY_RUN
  }

  static google::protobuf::Message* MutableValue(
      google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("MutableMessageField")),
        message_ref, field_ref);
#ifdef DRY_RUN
    return jni::Fake<google::protobuf::Message*>();
#else
    return ProtoEnv::GetEnv()->MutableMessageField(message_ref, field_ref);
#endif  // DRY_RUN
  }

  static void SetValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       const google::protobuf::Message* value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("SetMessageField")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetMessageField(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

struct ProtoRepeatedFieldSizeHelper : public ProtoFieldPresenceHelper {
  static int Size(const google::protobuf::Message* message_ref,
                  const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("FieldSize")),
               message_ref, field_ref);
#ifdef DRY_RUN
    return 0;
#else
    return ProtoEnv::GetEnv()->FieldSize(message_ref, field_ref);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<bool, 1, void> : public ProtoRepeatedFieldSizeHelper {
  static bool GetRepeatedValue(
      const google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("GetRepeatedBoolField")),
        message_ref, field_ref, index);
#ifdef DRY_RUN
    return false;
#else
    return ProtoEnv::GetEnv()->GetRepeatedBoolField(message_ref, field_ref,
                                                    index);
#endif  // DRY_RUN
  }

  static void SetRepeatedValue(
      google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index,
      bool value) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("SetRepeatedBoolField")),
        message_ref, field_ref, index, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetRepeatedBoolField(message_ref, field_ref, index,
                                             value);
#endif  // DRY_RUN
  }

  static void AddValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       bool value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("AddBoolField")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->AddBoolField(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<int32_t, 1, void>
    : public ProtoRepeatedFieldSizeHelper {
  static int32_t GetRepeatedValue(
      const google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("GetRepeatedInt32Field")),
        message_ref, field_ref, index);
#ifdef DRY_RUN
    return 123;
#else
    return ProtoEnv::GetEnv()->GetRepeatedInt32Field(message_ref, field_ref,
                                                     index);
#endif  // DRY_RUN
  }

  static void SetRepeatedValue(
      google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index,
      int32_t value) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("SetRepeatedInt32Field")),
        message_ref, field_ref, index, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetRepeatedInt32Field(message_ref, field_ref, index,
                                              value);
#endif  // DRY_RUN
  }

  static void AddValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       int32_t value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("AddInt32Field")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->AddInt32Field(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<uint32_t, 1, void>
    : public ProtoRepeatedFieldSizeHelper {
  static uint32_t GetRepeatedValue(
      const google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("GetRepeatedUInt32Field")),
        message_ref, field_ref, index);
#ifdef DRY_RUN
    return 123U;
#else
    return ProtoEnv::GetEnv()->GetRepeatedUInt32Field(message_ref, field_ref,
                                                      index);
#endif  // DRY_RUN
  }

  static void SetRepeatedValue(
      google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index,
      uint32_t value) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("SetRepeatedUInt32Field")),
        message_ref, field_ref, index, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetRepeatedUInt32Field(message_ref, field_ref, index,
                                               value);
#endif  // DRY_RUN
  }

  static void AddValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       uint32_t value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("AddUInt32Field")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->AddUInt32Field(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<int64_t, 1, void>
    : public ProtoRepeatedFieldSizeHelper {
  static int64_t GetRepeatedValue(
      const google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("GetRepeatedInt64Field")),
        message_ref, field_ref, index);
#ifdef DRY_RUN
    return 123LL;
#else
    return ProtoEnv::GetEnv()->GetRepeatedInt64Field(message_ref, field_ref,
                                                     index);
#endif  // DRY_RUN
  }

  static void SetRepeatedValue(
      google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index,
      int64_t value) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("SetRepeatedInt64Field")),
        message_ref, field_ref, index, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetRepeatedInt64Field(message_ref, field_ref, index,
                                              value);
#endif  // DRY_RUN
  }

  static void AddValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       int64_t value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("AddInt64Field")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->AddInt64Field(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<uint64_t, 1, void>
    : public ProtoRepeatedFieldSizeHelper {
  static uint64_t GetRepeatedValue(
      const google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("GetRepeatedUInt64Field")),
        message_ref, field_ref, index);
#ifdef DRY_RUN
    return 123ULL;
#else
    return ProtoEnv::GetEnv()->GetRepeatedUInt64Field(message_ref, field_ref,
                                                      index);
#endif  // DRY_RUN
  }

  static void SetRepeatedValue(
      google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index,
      uint64_t value) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("SetRepeatedUInt64Field")),
        message_ref, field_ref, index, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetRepeatedUInt64Field(message_ref, field_ref, index,
                                               value);
#endif  // DRY_RUN
  }

  static void AddValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       uint64_t value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("AddUInt64Field")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->AddUInt64Field(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<float, 1, void> : public ProtoRepeatedFieldSizeHelper {
  static float GetRepeatedValue(
      const google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("GetRepeatedFloatField")),
        message_ref, field_ref, index);
#ifdef DRY_RUN
    return 123.f;
#else
    return ProtoEnv::GetEnv()->GetRepeatedFloatField(message_ref, field_ref,
                                                     index);
#endif  // DRY_RUN
  }

  static void SetRepeatedValue(
      google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index,
      float value) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("SetRepeatedFloatField")),
        message_ref, field_ref, index, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetRepeatedFloatField(message_ref, field_ref, index,
                                              value);
#endif  // DRY_RUN
  }

  static void AddValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       float value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("AddFloatField")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->AddFloatField(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<double, 1, void> : public ProtoRepeatedFieldSizeHelper {
  static double GetRepeatedValue(
      const google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("GetRepeatedDoubleField")),
        message_ref, field_ref, index);
#ifdef DRY_RUN
    return 123.;
#else
    return ProtoEnv::GetEnv()->GetRepeatedDoubleField(message_ref, field_ref,
                                                      index);
#endif  // DRY_RUN
  }

  static void SetRepeatedValue(
      google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index,
      double value) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("SetRepeatedDoubleField")),
        message_ref, field_ref, index, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetRepeatedDoubleField(message_ref, field_ref, index,
                                               value);
#endif  // DRY_RUN
  }

  static void AddValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       double value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("AddDoubleField")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->AddDoubleField(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<std::string, 1, void>
    : public ProtoRepeatedFieldSizeHelper {
  static std::string GetRepeatedValue(
      const google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("GetRepeatedStringField")),
        message_ref, field_ref, index);
#ifdef DRY_RUN
    return "DEAD_BEEF";
#else
    return ProtoEnv::GetEnv()->GetRepeatedStringField(message_ref, field_ref,
                                                      index);
#endif  // DRY_RUN
  }

  static void SetRepeatedValue(
      google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index,
      std::string value) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("SetRepeatedStringField")),
        message_ref, field_ref, index, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetRepeatedStringField(message_ref, field_ref, index,
                                               std::move(value));
#endif  // DRY_RUN
  }

  static void AddValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       std::string value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("AddStringField")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->AddStringField(message_ref, field_ref,
                                       std::move(value));
#endif  // DRY_RUN
  }
};

template <>
struct ProtoFieldHelper<google::protobuf::Message*, 1, void>
    : public ProtoRepeatedFieldSizeHelper {
  static const google::protobuf::Message* GetRepeatedValue(
      const google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("GetRepeatedMessageField")),
        message_ref, field_ref, index);
#ifdef DRY_RUN
    return jni::Fake<const google::protobuf::Message*>();
#else
    return ProtoEnv::GetEnv()->GetRepeatedMessageField(message_ref, field_ref,
                                                       index);
#endif  // DRY_RUN
  }

  static google::protobuf::Message* MutableRepeatedValue(
      google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(
                   STR("MutableRepeatedMessageField")),
               message_ref, field_ref, index);
#ifdef DRY_RUN
    return jni::Fake<google::protobuf::Message*>();
#else
    return ProtoEnv::GetEnv()->MutableRepeatedMessageField(message_ref,
                                                           field_ref, index);
#endif  // DRY_RUN
  }

  static void SetRepeatedValue(
      google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref, int index,
      const google::protobuf::Message* value) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("SetRepeatedMessageField")),
        message_ref, field_ref, index, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->SetRepeatedMessageField(message_ref, field_ref, index,
                                                value);
#endif  // DRY_RUN
  }

  static google::protobuf::Message* AddMutableValue(
      google::protobuf::Message* message_ref,
      const google::protobuf::FieldDescriptor* field_ref) {
    jni::Trace(
        bind_lang::metaprogramming::LambdaToStr(STR("AddMutableMessageField")),
        message_ref, field_ref);
#ifdef DRY_RUN
    return jni::Fake<google::protobuf::Message*>();
#else
    return ProtoEnv::GetEnv()->AddMutableMessageField(message_ref, field_ref);
#endif  // DRY_RUN
  }

  static void AddValue(google::protobuf::Message* message_ref,
                       const google::protobuf::FieldDescriptor* field_ref,
                       const google::protobuf::Message* value) {
    jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("AddMessageField")),
               message_ref, field_ref, value);
#ifdef DRY_RUN
#else
    ProtoEnv::GetEnv()->AddMessageField(message_ref, field_ref, value);
#endif  // DRY_RUN
  }
};

}  // namespace proto

#endif  // JNI_BIND_IMPLEMENTATION_PROTO_HELPER_FIELD_VALUE_H_
