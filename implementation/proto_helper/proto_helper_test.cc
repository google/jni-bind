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

#include "implementation/proto_helper/proto_helper.h"

#include <cstdint>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "bind_lang/lifecycle.h"
#include "implementation/proto_helper/fake_proto_test_constants.h"  // IWYU pragma: keep
#include "implementation/proto_helper/field_value.h"
#include "implementation/proto_helper/lifecycle_proto.h"
#include "proto_dep.h"
#include "proto_test.h"

namespace {

using ::bind_lang::LifecycleHelper;
using ::bind_lang::LifecycleType;
using ::jni::Fake;
using ::proto::ProtoFieldHelper;
using ::proto::ProtoHelper;
using ::proto::test::AsClonedMessage;
using ::proto::test::ProtoTest;
using ::testing::Eq;
using ::testing::Return;
using ::testing::StrEq;

TEST_F(ProtoTest, FindDescriptorForwardsToProtoEnv) {
  EXPECT_CALL(*env_, FindMessageTypeByName(StrEq("my.pkg.MyProto")))
      .WillOnce(Return(Fake<const google::protobuf::Descriptor*>(7)));

  EXPECT_EQ(ProtoHelper::FindDescriptor("my.pkg.MyProto"),
            Fake<const google::protobuf::Descriptor*>(7));
}

TEST_F(ProtoTest, GetFieldDescriptorForwardsToProtoEnv) {
  const auto* fake_desc = Fake<const google::protobuf::Descriptor*>(3);
  const auto* fake_field = Fake<const google::protobuf::FieldDescriptor*>(5);

  EXPECT_CALL(*env_,
              GetFieldDescriptor(Eq(fake_desc), Eq(1), StrEq("my_field")))
      .WillOnce(Return(fake_field));

  EXPECT_EQ(ProtoHelper::GetFieldDescriptor(fake_desc, 1, "my_field"),
            fake_field);
}

TEST_F(ProtoTest, LifecycleHelperConstructCloneAndDelete) {
  using LifecycleT =
      LifecycleHelper<google::protobuf::Message*, LifecycleType::LOCAL>;
  const auto* fake_desc = Fake<const google::protobuf::Descriptor*>();
  auto* fake_msg = Fake<google::protobuf::Message*>();

  EXPECT_CALL(*env_, NewMessage(Eq(fake_desc))).WillOnce(Return(fake_msg));
  EXPECT_EQ(LifecycleT::Construct(fake_desc), fake_msg);

  EXPECT_CALL(*env_, CloneMessage(Eq(fake_msg)))
      .WillOnce(Return(AsClonedMessage(fake_msg)));
  auto* cloned = LifecycleT::NewReference(fake_msg);
  EXPECT_EQ(cloned, AsClonedMessage(fake_msg));

  EXPECT_CALL(*env_, DeleteMessage(Eq(fake_msg)));
  LifecycleT::Delete(fake_msg);

  EXPECT_CALL(*env_, SerializeAsString(Eq(fake_msg)))
      .WillOnce(Return("serialized_bytes"));
  EXPECT_EQ(LifecycleT::SerializeAsString(fake_msg), "serialized_bytes");

  EXPECT_CALL(*env_, ParseFromString(Eq(fake_msg), StrEq("wire_input")))
      .WillOnce(Return(true));
  EXPECT_TRUE(LifecycleT::ParseFromString(fake_msg, "wire_input"));
}

TEST_F(ProtoTest, ProtoFieldHelperDispatchesScalarAndStringAccessors) {
  auto* fake_msg = Fake<google::protobuf::Message*>();
  const auto* fake_field = Fake<const google::protobuf::FieldDescriptor*>();

  EXPECT_CALL(*env_, SetInt32Field(fake_msg, fake_field, 42));
  ProtoFieldHelper<int32_t>::SetValue(fake_msg, fake_field, 42);

  int32_t lvalue_int = 77;
  EXPECT_CALL(*env_, SetInt32Field(fake_msg, fake_field, 77));
  ProtoFieldHelper<int32_t>::SetValue(fake_msg, fake_field, lvalue_int);

  EXPECT_CALL(*env_, GetInt32Field(fake_msg, fake_field)).WillOnce(Return(42));
  EXPECT_EQ(ProtoFieldHelper<int32_t>::GetValue(fake_msg, fake_field), 42);

  EXPECT_CALL(*env_, SetUInt32Field(fake_msg, fake_field, 100u));
  ProtoFieldHelper<uint32_t>::SetValue(fake_msg, fake_field, 100u);
  EXPECT_CALL(*env_, GetUInt32Field(fake_msg, fake_field))
      .WillOnce(Return(100u));
  EXPECT_EQ(ProtoFieldHelper<uint32_t>::GetValue(fake_msg, fake_field), 100u);

  EXPECT_CALL(*env_, SetInt64Field(fake_msg, fake_field, -9000LL));
  ProtoFieldHelper<int64_t>::SetValue(fake_msg, fake_field, -9000LL);
  EXPECT_CALL(*env_, GetInt64Field(fake_msg, fake_field))
      .WillOnce(Return(-9000LL));
  EXPECT_EQ(ProtoFieldHelper<int64_t>::GetValue(fake_msg, fake_field), -9000LL);

  EXPECT_CALL(*env_, SetUInt64Field(fake_msg, fake_field, 9000ULL));
  ProtoFieldHelper<uint64_t>::SetValue(fake_msg, fake_field, 9000ULL);
  EXPECT_CALL(*env_, GetUInt64Field(fake_msg, fake_field))
      .WillOnce(Return(9000ULL));
  EXPECT_EQ(ProtoFieldHelper<uint64_t>::GetValue(fake_msg, fake_field),
            9000ULL);

  EXPECT_CALL(*env_, SetFloatField(fake_msg, fake_field, 1.5f));
  ProtoFieldHelper<float>::SetValue(fake_msg, fake_field, 1.5f);
  EXPECT_CALL(*env_, GetFloatField(fake_msg, fake_field))
      .WillOnce(Return(1.5f));
  EXPECT_FLOAT_EQ(ProtoFieldHelper<float>::GetValue(fake_msg, fake_field),
                  1.5f);

  EXPECT_CALL(*env_, SetDoubleField(fake_msg, fake_field, 2.5));
  ProtoFieldHelper<double>::SetValue(fake_msg, fake_field, 2.5);
  EXPECT_CALL(*env_, GetDoubleField(fake_msg, fake_field))
      .WillOnce(Return(2.5));
  EXPECT_DOUBLE_EQ(ProtoFieldHelper<double>::GetValue(fake_msg, fake_field),
                   2.5);

  EXPECT_CALL(*env_, SetBoolField(fake_msg, fake_field, true));
  ProtoFieldHelper<bool>::SetValue(fake_msg, fake_field, true);
  EXPECT_CALL(*env_, GetBoolField(fake_msg, fake_field)).WillOnce(Return(true));
  EXPECT_TRUE(ProtoFieldHelper<bool>::GetValue(fake_msg, fake_field));

  EXPECT_CALL(*env_, SetStringField(fake_msg, fake_field, "hello"));
  ProtoFieldHelper<std::string>::SetValue(fake_msg, fake_field,
                                          std::string{"hello"});

  EXPECT_CALL(*env_, GetStringField(fake_msg, fake_field))
      .WillOnce(Return("hello"));
  EXPECT_EQ(ProtoFieldHelper<std::string>::GetValue(fake_msg, fake_field),
            "hello");

  EXPECT_CALL(*env_, HasField(fake_msg, fake_field)).WillOnce(Return(true));
  EXPECT_TRUE(ProtoFieldHelper<int32_t>::HasValue(fake_msg, fake_field));

  EXPECT_CALL(*env_, ClearField(fake_msg, fake_field));
  ProtoFieldHelper<int32_t>::ClearValue(fake_msg, fake_field);
}

TEST_F(ProtoTest, ProtoFieldHelperDispatchesRepeatedAccessorsForAllTypes) {
  auto* fake_msg = Fake<google::protobuf::Message*>(1);
  auto* fake_sub = Fake<google::protobuf::Message*>(2);
  const auto* fake_field = Fake<const google::protobuf::FieldDescriptor*>();

  EXPECT_CALL(*env_, AddBoolField(fake_msg, fake_field, true));
  ProtoFieldHelper<bool, 1>::AddValue(fake_msg, fake_field, true);
  EXPECT_CALL(*env_, SetRepeatedBoolField(fake_msg, fake_field, 0, false));
  ProtoFieldHelper<bool, 1>::SetRepeatedValue(fake_msg, fake_field, 0, false);
  EXPECT_CALL(*env_, GetRepeatedBoolField(fake_msg, fake_field, 0))
      .WillOnce(Return(false));
  EXPECT_FALSE(
      (ProtoFieldHelper<bool, 1>::GetRepeatedValue(fake_msg, fake_field, 0)));

  EXPECT_CALL(*env_, AddInt32Field(fake_msg, fake_field, 10));
  ProtoFieldHelper<int32_t, 1>::AddValue(fake_msg, fake_field, 10);
  EXPECT_CALL(*env_, SetRepeatedInt32Field(fake_msg, fake_field, 0, 20));
  ProtoFieldHelper<int32_t, 1>::SetRepeatedValue(fake_msg, fake_field, 0, 20);
  EXPECT_CALL(*env_, GetRepeatedInt32Field(fake_msg, fake_field, 0))
      .WillOnce(Return(20));
  EXPECT_EQ(
      (ProtoFieldHelper<int32_t, 1>::GetRepeatedValue(fake_msg, fake_field, 0)),
      20);

  EXPECT_CALL(*env_, AddUInt32Field(fake_msg, fake_field, 30u));
  ProtoFieldHelper<uint32_t, 1>::AddValue(fake_msg, fake_field, 30u);
  EXPECT_CALL(*env_, SetRepeatedUInt32Field(fake_msg, fake_field, 0, 40u));
  ProtoFieldHelper<uint32_t, 1>::SetRepeatedValue(fake_msg, fake_field, 0, 40u);
  EXPECT_CALL(*env_, GetRepeatedUInt32Field(fake_msg, fake_field, 0))
      .WillOnce(Return(40u));
  EXPECT_EQ((ProtoFieldHelper<uint32_t, 1>::GetRepeatedValue(fake_msg,
                                                             fake_field, 0)),
            40u);

  EXPECT_CALL(*env_, AddInt64Field(fake_msg, fake_field, 50LL));
  ProtoFieldHelper<int64_t, 1>::AddValue(fake_msg, fake_field, 50LL);
  EXPECT_CALL(*env_, SetRepeatedInt64Field(fake_msg, fake_field, 0, 60LL));
  ProtoFieldHelper<int64_t, 1>::SetRepeatedValue(fake_msg, fake_field, 0, 60LL);
  EXPECT_CALL(*env_, GetRepeatedInt64Field(fake_msg, fake_field, 0))
      .WillOnce(Return(60LL));
  EXPECT_EQ(
      (ProtoFieldHelper<int64_t, 1>::GetRepeatedValue(fake_msg, fake_field, 0)),
      60LL);

  EXPECT_CALL(*env_, AddUInt64Field(fake_msg, fake_field, 70ULL));
  ProtoFieldHelper<uint64_t, 1>::AddValue(fake_msg, fake_field, 70ULL);
  EXPECT_CALL(*env_, SetRepeatedUInt64Field(fake_msg, fake_field, 0, 80ULL));
  ProtoFieldHelper<uint64_t, 1>::SetRepeatedValue(fake_msg, fake_field, 0,
                                                  80ULL);
  EXPECT_CALL(*env_, GetRepeatedUInt64Field(fake_msg, fake_field, 0))
      .WillOnce(Return(80ULL));
  EXPECT_EQ((ProtoFieldHelper<uint64_t, 1>::GetRepeatedValue(fake_msg,
                                                             fake_field, 0)),
            80ULL);

  EXPECT_CALL(*env_, AddFloatField(fake_msg, fake_field, 1.25f));
  ProtoFieldHelper<float, 1>::AddValue(fake_msg, fake_field, 1.25f);
  EXPECT_CALL(*env_, SetRepeatedFloatField(fake_msg, fake_field, 0, 2.5f));
  ProtoFieldHelper<float, 1>::SetRepeatedValue(fake_msg, fake_field, 0, 2.5f);
  EXPECT_CALL(*env_, GetRepeatedFloatField(fake_msg, fake_field, 0))
      .WillOnce(Return(2.5f));
  EXPECT_FLOAT_EQ(
      (ProtoFieldHelper<float, 1>::GetRepeatedValue(fake_msg, fake_field, 0)),
      2.5f);

  EXPECT_CALL(*env_, AddDoubleField(fake_msg, fake_field, 3.5));
  ProtoFieldHelper<double, 1>::AddValue(fake_msg, fake_field, 3.5);
  EXPECT_CALL(*env_, SetRepeatedDoubleField(fake_msg, fake_field, 0, 4.5));
  ProtoFieldHelper<double, 1>::SetRepeatedValue(fake_msg, fake_field, 0, 4.5);
  EXPECT_CALL(*env_, GetRepeatedDoubleField(fake_msg, fake_field, 0))
      .WillOnce(Return(4.5));
  EXPECT_DOUBLE_EQ(
      (ProtoFieldHelper<double, 1>::GetRepeatedValue(fake_msg, fake_field, 0)),
      4.5);

  EXPECT_CALL(*env_, AddStringField(fake_msg, fake_field, "a"));
  ProtoFieldHelper<std::string, 1>::AddValue(fake_msg, fake_field, "a");
  EXPECT_CALL(*env_, SetRepeatedStringField(fake_msg, fake_field, 0, "b"));
  ProtoFieldHelper<std::string, 1>::SetRepeatedValue(fake_msg, fake_field, 0,
                                                     "b");
  EXPECT_CALL(*env_, GetRepeatedStringField(fake_msg, fake_field, 0))
      .WillOnce(Return("b"));
  EXPECT_EQ((ProtoFieldHelper<std::string, 1>::GetRepeatedValue(fake_msg,
                                                                fake_field, 0)),
            "b");

  EXPECT_CALL(*env_, AddMessageField(fake_msg, fake_field, fake_sub));
  ProtoFieldHelper<google::protobuf::Message*, 1>::AddValue(
      fake_msg, fake_field, fake_sub);
  EXPECT_CALL(*env_, AddMutableMessageField(fake_msg, fake_field))
      .WillOnce(Return(fake_sub));
  EXPECT_EQ((ProtoFieldHelper<google::protobuf::Message*, 1>::AddMutableValue(
                fake_msg, fake_field)),
            fake_sub);
  EXPECT_CALL(*env_,
              SetRepeatedMessageField(fake_msg, fake_field, 0, fake_sub));
  ProtoFieldHelper<google::protobuf::Message*, 1>::SetRepeatedValue(
      fake_msg, fake_field, 0, fake_sub);
  EXPECT_CALL(*env_, MutableRepeatedMessageField(fake_msg, fake_field, 0))
      .WillOnce(Return(fake_sub));
  EXPECT_EQ(
      (ProtoFieldHelper<google::protobuf::Message*, 1>::MutableRepeatedValue(
          fake_msg, fake_field, 0)),
      fake_sub);
}

}  // namespace
