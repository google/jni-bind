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

#ifndef JNI_BIND_MOCK_PROTO_ENV_H_
#define JNI_BIND_MOCK_PROTO_ENV_H_

#include <cstdint>
#include <string>
#include <string_view>

#include <gmock/gmock.h>
#include "implementation/proto_helper/proto_env.h"
#include "proto_dep.h"

namespace proto::test {

// gMock implementation of `ProtoEnvInterface` for hermetic unit testing of
// `proto::Message`, `ProtoFieldRef`, `DescriptorRef`, and `ProtoHelper` without
// requiring compiled `.proto` schemas.
// Mirrors `jni::test::MockJniEnv` in `mock_jni_env.h`.
class MockProtoEnv : public ProtoEnvInterface {
 public:
  MOCK_METHOD(const google::protobuf::Descriptor*, FindMessageTypeByName,
              (const char* name), (override));
  MOCK_METHOD(const google::protobuf::Descriptor*, RegisterDynamicDescriptor,
              (const google::protobuf::FileDescriptorProto& file_proto,
               const char* message_name),
              (override));
  MOCK_METHOD(const google::protobuf::FieldDescriptor*, GetFieldDescriptor,
              (const google::protobuf::Descriptor* descriptor, int field_number,
               const char* field_name),
              (override));

  MOCK_METHOD(google::protobuf::Message*, NewMessage,
              (const google::protobuf::Descriptor* descriptor), (override));
  MOCK_METHOD(google::protobuf::Message*, CloneMessage,
              (const google::protobuf::Message* message), (override));
  MOCK_METHOD(void, DeleteMessage, (google::protobuf::Message * message),
              (override));
  MOCK_METHOD(std::string, SerializeAsString,
              (const google::protobuf::Message* message), (override));
  MOCK_METHOD(bool, ParseFromString,
              (google::protobuf::Message * message, std::string_view data),
              (override));

  MOCK_METHOD(bool, HasField,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field),
              (override));
  MOCK_METHOD(void, ClearField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field),
              (override));

  MOCK_METHOD(int32_t, GetInt32Field,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field),
              (override));
  MOCK_METHOD(void, SetInt32Field,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int32_t value),
              (override));

  MOCK_METHOD(int64_t, GetInt64Field,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field),
              (override));
  MOCK_METHOD(void, SetInt64Field,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int64_t value),
              (override));

  MOCK_METHOD(uint32_t, GetUInt32Field,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field),
              (override));
  MOCK_METHOD(void, SetUInt32Field,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, uint32_t value),
              (override));

  MOCK_METHOD(uint64_t, GetUInt64Field,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field),
              (override));
  MOCK_METHOD(void, SetUInt64Field,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, uint64_t value),
              (override));

  MOCK_METHOD(float, GetFloatField,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field),
              (override));
  MOCK_METHOD(void, SetFloatField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, float value),
              (override));

  MOCK_METHOD(double, GetDoubleField,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field),
              (override));
  MOCK_METHOD(void, SetDoubleField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, double value),
              (override));

  MOCK_METHOD(bool, GetBoolField,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field),
              (override));
  MOCK_METHOD(void, SetBoolField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, bool value),
              (override));

  MOCK_METHOD(std::string, GetStringField,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field),
              (override));
  MOCK_METHOD(void, SetStringField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field,
               std::string value),
              (override));

  MOCK_METHOD(const google::protobuf::Message*, GetMessageField,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field),
              (override));
  MOCK_METHOD(google::protobuf::Message*, MutableMessageField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field),
              (override));
  MOCK_METHOD(void, SetMessageField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field,
               const google::protobuf::Message* sub_message),
              (override));

  MOCK_METHOD(int, FieldSize,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field),
              (override));

  MOCK_METHOD(bool, GetRepeatedBoolField,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field, int index),
              (override));
  MOCK_METHOD(void, SetRepeatedBoolField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int index,
               bool value),
              (override));
  MOCK_METHOD(void, AddBoolField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, bool value),
              (override));

  MOCK_METHOD(int32_t, GetRepeatedInt32Field,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field, int index),
              (override));
  MOCK_METHOD(void, SetRepeatedInt32Field,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int index,
               int32_t value),
              (override));
  MOCK_METHOD(void, AddInt32Field,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int32_t value),
              (override));

  MOCK_METHOD(uint32_t, GetRepeatedUInt32Field,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field, int index),
              (override));
  MOCK_METHOD(void, SetRepeatedUInt32Field,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int index,
               uint32_t value),
              (override));
  MOCK_METHOD(void, AddUInt32Field,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, uint32_t value),
              (override));

  MOCK_METHOD(int64_t, GetRepeatedInt64Field,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field, int index),
              (override));
  MOCK_METHOD(void, SetRepeatedInt64Field,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int index,
               int64_t value),
              (override));
  MOCK_METHOD(void, AddInt64Field,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int64_t value),
              (override));

  MOCK_METHOD(uint64_t, GetRepeatedUInt64Field,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field, int index),
              (override));
  MOCK_METHOD(void, SetRepeatedUInt64Field,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int index,
               uint64_t value),
              (override));
  MOCK_METHOD(void, AddUInt64Field,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, uint64_t value),
              (override));

  MOCK_METHOD(float, GetRepeatedFloatField,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field, int index),
              (override));
  MOCK_METHOD(void, SetRepeatedFloatField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int index,
               float value),
              (override));
  MOCK_METHOD(void, AddFloatField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, float value),
              (override));

  MOCK_METHOD(double, GetRepeatedDoubleField,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field, int index),
              (override));
  MOCK_METHOD(void, SetRepeatedDoubleField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int index,
               double value),
              (override));
  MOCK_METHOD(void, AddDoubleField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, double value),
              (override));

  MOCK_METHOD(std::string, GetRepeatedStringField,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field, int index),
              (override));
  MOCK_METHOD(void, SetRepeatedStringField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int index,
               std::string value),
              (override));
  MOCK_METHOD(void, AddStringField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field,
               std::string value),
              (override));

  MOCK_METHOD(const google::protobuf::Message*, GetRepeatedMessageField,
              (const google::protobuf::Message* message,
               const google::protobuf::FieldDescriptor* field, int index),
              (override));
  MOCK_METHOD(google::protobuf::Message*, MutableRepeatedMessageField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int index),
              (override));
  MOCK_METHOD(void, SetRepeatedMessageField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field, int index,
               const google::protobuf::Message* sub_message),
              (override));
  MOCK_METHOD(google::protobuf::Message*, AddMutableMessageField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field),
              (override));
  MOCK_METHOD(void, AddMessageField,
              (google::protobuf::Message * message,
               const google::protobuf::FieldDescriptor* field,
               const google::protobuf::Message* sub_message),
              (override));
};

}  // namespace proto::test

#endif  // JNI_BIND_MOCK_PROTO_ENV_H_
