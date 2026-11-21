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

#ifndef JNI_BIND_PROTO_TEST_H_
#define JNI_BIND_PROTO_TEST_H_

#include <cstdint>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "mock_proto_env.h"
#include "proto_bind.h"
#include "proto_dep.h"

namespace proto::test {

static constexpr std::uintptr_t kProtoCloneOffset = 0x100000000000A0ULL;

inline google::protobuf::Message* AsClonedMessage(
    const google::protobuf::Message* msg) {
  return reinterpret_cast<google::protobuf::Message*>(
      reinterpret_cast<std::uintptr_t>(msg) + kProtoCloneOffset);
}

// Standard unit test fixture for Proto Bind that installs a `MockProtoEnv` with
// default fake return values (`Fake<const Descriptor*>()`,
// `Fake<const FieldDescriptor*>()`, `Fake<Message*>()`).
// Mirrors `jni::test::JniTest` in `jni_test.h`.
class ProtoTest : public ::testing::Test {
 public:
  void SetUp() override {
    using ::testing::_;
    using ::testing::Return;

    env_ = std::make_unique<MockProtoEnv>();
    ProtoEnv::SetEnv(env_.get());

    ON_CALL(*env_, FindMessageTypeByName)
        .WillByDefault(
            Return(jni::Fake<const google::protobuf::Descriptor*>()));
    ON_CALL(*env_, RegisterDynamicDescriptor)
        .WillByDefault(
            Return(jni::Fake<const google::protobuf::Descriptor*>()));
    ON_CALL(*env_, GetFieldDescriptor)
        .WillByDefault(
            Return(jni::Fake<const google::protobuf::FieldDescriptor*>()));
    ON_CALL(*env_, NewMessage)
        .WillByDefault(Return(jni::Fake<google::protobuf::Message*>()));
    ON_CALL(*env_, CloneMessage)
        .WillByDefault([](const google::protobuf::Message* msg) {
          return AsClonedMessage(msg);
        });
    ON_CALL(*env_, MutableMessageField)
        .WillByDefault(Return(jni::Fake<google::protobuf::Message*>(2)));
    ON_CALL(*env_, GetMessageField)
        .WillByDefault(Return(jni::Fake<const google::protobuf::Message*>(2)));
    ON_CALL(*env_, MutableRepeatedMessageField)
        .WillByDefault(Return(jni::Fake<google::protobuf::Message*>(3)));
    ON_CALL(*env_, GetRepeatedMessageField)
        .WillByDefault(Return(jni::Fake<const google::protobuf::Message*>(3)));
    ON_CALL(*env_, AddMutableMessageField)
        .WillByDefault(Return(jni::Fake<google::protobuf::Message*>(4)));
    ON_CALL(*env_, ParseFromString).WillByDefault(Return(true));
  }

  void TearDown() override {
    ReleaseAllProtoDescriptorsForTests();
    ProtoEnv::SetEnv(nullptr);
    env_.reset();
  }

 protected:
  std::unique_ptr<MockProtoEnv> env_;
};

}  // namespace proto::test

#endif  // JNI_BIND_PROTO_TEST_H_
