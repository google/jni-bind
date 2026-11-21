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

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/proto_helper/fake_proto_test_constants.h"  // IWYU pragma: keep
#include "proto_bind.h"
#include "proto_dep.h"
#include "proto_test.h"

namespace {

using ::jni::Fake;
using ::proto::AdoptLocal;
using ::proto::Message;
using ::proto::NewRef;
using ::proto::Optional;
using ::proto::ProtoFieldSelection;
using ::proto::ProtoT;
using ::proto::Repeated;
using ::proto::Required;
using ::proto::Template;
using ::proto::Unowned;
using ::proto::test::AsClonedMessage;
using ::proto::test::ProtoTest;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

// Style 1: CTAD with field names in field constructors (the primary target
// syntax from the task description).
static constexpr proto::Template kCtadTemplate{
    proto::Optional<0, int>{"some_optional_int_field"},
    proto::Required<1, std::string>{"some_string_field"},
};

// Style 2: Explicit template parameters with field names passed to the
// `proto::Template` constructor.
static constexpr proto::Template<proto::Optional<0, int>,
                                 proto::Required<1, std::string>>
    kExplicitTemplate{"some_optional_int_field", "some_string_field"};

// Style 3: Explicit template parameters with C++20 `StringLiteral` NTTP field
// names.
static constexpr proto::Template<
    proto::Optional<0, int, "some_optional_int_field">,
    proto::Required<1, std::string, "some_string_field">>
    kNttpNamedTemplate{};

// Compile-time validation of `Optional` vs `Required` field constraints:
using CtadProtoT = ProtoT<kCtadTemplate>;
static_assert(ProtoFieldSelection<CtadProtoT, 0>::Valable<int>());
static_assert(ProtoFieldSelection<CtadProtoT, 0>::Valable<std::nullopt_t>());
static_assert(!ProtoFieldSelection<CtadProtoT, 0>::Valable<const char*>());

static_assert(ProtoFieldSelection<CtadProtoT, 1>::Valable<const char*>());
static_assert(ProtoFieldSelection<CtadProtoT, 1>::Valable<std::string>());
static_assert(ProtoFieldSelection<CtadProtoT, 1>::Valable<std::string_view>());
// `Required` fields reject `std::nullopt_t` at compile time!
static_assert(!ProtoFieldSelection<CtadProtoT, 1>::Valable<std::nullopt_t>());
static_assert(!ProtoFieldSelection<CtadProtoT, 1>::Valable<int>());

// Compile-time validation of unique field tag enforcement:
static_assert(proto::AreFieldTagsUnique<>());
static_assert(proto::AreFieldTagsUnique<proto::Optional<0, int>,
                                        proto::Required<1, std::string>>());
static_assert(!proto::AreFieldTagsUnique<proto::Optional<1, int>,
                                         proto::Required<1, std::string>>());
static_assert(!proto::AreFieldTagsUnique<proto::Optional<0, int>,
                                         proto::Optional<2, float>,
                                         proto::Repeated<0, std::string>>());

TEST_F(ProtoTest, ConstructsMessageWithTargetCtadSyntaxAndSetsFields) {
  const auto* fake_desc = Fake<const google::protobuf::Descriptor*>();
  const auto* fake_int_field =
      Fake<const google::protobuf::FieldDescriptor*>(10);
  const auto* fake_str_field =
      Fake<const google::protobuf::FieldDescriptor*>(20);
  auto* fake_msg = Fake<google::protobuf::Message*>();

  EXPECT_CALL(*env_, FindMessageTypeByName(_)).WillOnce(Return(fake_desc));
  EXPECT_CALL(*env_, NewMessage(fake_desc)).WillOnce(Return(fake_msg));
  EXPECT_CALL(
      *env_, GetFieldDescriptor(fake_desc, 0, StrEq("some_optional_int_field")))
      .WillOnce(Return(fake_int_field));
  EXPECT_CALL(*env_, SetInt32Field(fake_msg, fake_int_field, 123));
  EXPECT_CALL(*env_,
              GetFieldDescriptor(fake_desc, 1, StrEq("some_string_field")))
      .WillOnce(Return(fake_str_field));
  EXPECT_CALL(*env_, SetStringField(fake_msg, fake_str_field, "string_val"));
  EXPECT_CALL(*env_, DeleteMessage(fake_msg));

  proto::Message<kCtadTemplate> runtime_msg{123, "string_val"};
}

TEST_F(ProtoTest, ConstructsMessageWithExplicitTemplateSyntax) {
  const auto* fake_desc = Fake<const google::protobuf::Descriptor*>();
  const auto* fake_int_field =
      Fake<const google::protobuf::FieldDescriptor*>(11);
  const auto* fake_str_field =
      Fake<const google::protobuf::FieldDescriptor*>(21);
  auto* fake_msg = Fake<google::protobuf::Message*>();

  EXPECT_CALL(*env_, FindMessageTypeByName(_)).WillOnce(Return(fake_desc));
  EXPECT_CALL(*env_, NewMessage(fake_desc)).WillOnce(Return(fake_msg));
  EXPECT_CALL(
      *env_, GetFieldDescriptor(fake_desc, 0, StrEq("some_optional_int_field")))
      .WillOnce(Return(fake_int_field));
  EXPECT_CALL(*env_, SetInt32Field(fake_msg, fake_int_field, 123));
  EXPECT_CALL(*env_,
              GetFieldDescriptor(fake_desc, 1, StrEq("some_string_field")))
      .WillOnce(Return(fake_str_field));
  EXPECT_CALL(*env_, SetStringField(fake_msg, fake_str_field, "string_val"));
  EXPECT_CALL(*env_, DeleteMessage(fake_msg));

  proto::Message<kExplicitTemplate> runtime_msg{123, "string_val"};
}

TEST_F(ProtoTest, ConstructsMessageWithNttpNamedSyntax) {
  const auto* fake_desc = Fake<const google::protobuf::Descriptor*>();
  const auto* fake_int_field =
      Fake<const google::protobuf::FieldDescriptor*>(12);
  const auto* fake_str_field =
      Fake<const google::protobuf::FieldDescriptor*>(22);
  auto* fake_msg = Fake<google::protobuf::Message*>();

  EXPECT_CALL(*env_, FindMessageTypeByName(_)).WillOnce(Return(fake_desc));
  EXPECT_CALL(*env_, NewMessage(fake_desc)).WillOnce(Return(fake_msg));
  EXPECT_CALL(
      *env_, GetFieldDescriptor(fake_desc, 0, StrEq("some_optional_int_field")))
      .WillOnce(Return(fake_int_field));
  EXPECT_CALL(*env_, SetInt32Field(fake_msg, fake_int_field, 456));
  EXPECT_CALL(*env_,
              GetFieldDescriptor(fake_desc, 1, StrEq("some_string_field")))
      .WillOnce(Return(fake_str_field));
  EXPECT_CALL(*env_, SetStringField(fake_msg, fake_str_field, "nttp_val"));
  EXPECT_CALL(*env_, DeleteMessage(fake_msg));

  proto::Message<kNttpNamedTemplate> runtime_msg{456, "nttp_val"};
}

TEST_F(ProtoTest, OptionalFieldAcceptsNulloptDuringConstruction) {
  const auto* fake_desc = Fake<const google::protobuf::Descriptor*>();
  const auto* fake_str_field =
      Fake<const google::protobuf::FieldDescriptor*>(20);
  auto* fake_msg = Fake<google::protobuf::Message*>();

  EXPECT_CALL(*env_, FindMessageTypeByName(_)).WillOnce(Return(fake_desc));
  EXPECT_CALL(*env_, NewMessage(fake_desc)).WillOnce(Return(fake_msg));
  // Field 0 is passed std::nullopt, so SetInt32Field must NOT be called.
  EXPECT_CALL(*env_, SetInt32Field(_, _, _)).Times(0);
  EXPECT_CALL(*env_,
              GetFieldDescriptor(fake_desc, 1, StrEq("some_string_field")))
      .WillOnce(Return(fake_str_field));
  EXPECT_CALL(*env_, SetStringField(fake_msg, fake_str_field, "only_required"));
  EXPECT_CALL(*env_, DeleteMessage(fake_msg));

  proto::Message<kCtadTemplate> runtime_msg{std::nullopt, "only_required"};
}

TEST_F(ProtoTest, FieldGetSetHasClearViaSubscriptAccessAndTag) {
  const auto* fake_desc = Fake<const google::protobuf::Descriptor*>();
  const auto* fake_int_field =
      Fake<const google::protobuf::FieldDescriptor*>(10);
  const auto* fake_str_field =
      Fake<const google::protobuf::FieldDescriptor*>(20);
  auto* fake_msg = Fake<google::protobuf::Message*>();

  EXPECT_CALL(*env_, FindMessageTypeByName(_)).WillOnce(Return(fake_desc));
  EXPECT_CALL(*env_, NewMessage(fake_desc)).WillOnce(Return(fake_msg));
  EXPECT_CALL(
      *env_, GetFieldDescriptor(fake_desc, 0, StrEq("some_optional_int_field")))
      .WillOnce(Return(fake_int_field));
  EXPECT_CALL(*env_,
              GetFieldDescriptor(fake_desc, 1, StrEq("some_string_field")))
      .WillOnce(Return(fake_str_field));

  proto::Message<kCtadTemplate> msg{};

  // Subscript operator[] (via QueryableMap).
  EXPECT_CALL(*env_, SetInt32Field(fake_msg, fake_int_field, 777));
  msg["some_optional_int_field"].Set(777);

  EXPECT_CALL(*env_, GetInt32Field(fake_msg, fake_int_field))
      .WillOnce(Return(777));
  EXPECT_EQ(msg["some_optional_int_field"].Get(), 777);

  // C++20 .Access<"field">() (via QueryableMap20).
  EXPECT_CALL(*env_, SetStringField(fake_msg, fake_str_field, "updated_str"));
  msg.Access<"some_string_field">().Set("updated_str");

  EXPECT_CALL(*env_, GetStringField(fake_msg, fake_str_field))
      .WillOnce(Return("updated_str"));
  EXPECT_EQ(msg.Access<"some_string_field">().Get(), "updated_str");

  // Tag-based .AccessByTag<Tag>().
  EXPECT_CALL(*env_, HasField(fake_msg, fake_int_field)).WillOnce(Return(true));
  EXPECT_CALL(*env_, GetInt32Field(fake_msg, fake_int_field))
      .WillOnce(Return(777));
  std::optional<int32_t> opt_val = msg.AccessByTag<0>().GetOptional();
  ASSERT_TRUE(opt_val.has_value());
  EXPECT_EQ(*opt_val, 777);

  // Setting std::nullopt on an Optional field clears it.
  EXPECT_CALL(*env_, ClearField(fake_msg, fake_int_field));
  msg["some_optional_int_field"].Set(std::nullopt);

  EXPECT_CALL(*env_, HasField(fake_msg, fake_int_field))
      .WillOnce(Return(false));
  EXPECT_EQ(msg["some_optional_int_field"].GetOptional(), std::nullopt);

  EXPECT_CALL(*env_, DeleteMessage(fake_msg));
}

TEST_F(ProtoTest, DescriptorCachingLookupsOnlyOncePerTemplate) {
  const auto* fake_desc = Fake<const google::protobuf::Descriptor*>();
  const auto* fake_int_field =
      Fake<const google::protobuf::FieldDescriptor*>(10);
  auto* fake_msg1 = Fake<google::protobuf::Message*>(1);
  auto* fake_msg2 = Fake<google::protobuf::Message*>(2);

  // Even across multiple Message instances and multiple field accesses,
  // FindMessageTypeByName and GetFieldDescriptor are called only ONCE thanks
  // to DoubleLockedValue caching.
  EXPECT_CALL(*env_, FindMessageTypeByName(_)).WillOnce(Return(fake_desc));
  EXPECT_CALL(
      *env_, GetFieldDescriptor(fake_desc, 0, StrEq("some_optional_int_field")))
      .WillOnce(Return(fake_int_field));

  EXPECT_CALL(*env_, NewMessage(fake_desc))
      .WillOnce(Return(fake_msg1))
      .WillOnce(Return(fake_msg2));

  proto::Message<kCtadTemplate> msg1{};
  proto::Message<kCtadTemplate> msg2{};

  EXPECT_CALL(*env_, SetInt32Field(fake_msg1, fake_int_field, 10));
  EXPECT_CALL(*env_, SetInt32Field(fake_msg2, fake_int_field, 20));
  msg1["some_optional_int_field"].Set(10);
  msg2["some_optional_int_field"].Set(20);

  EXPECT_CALL(*env_, DeleteMessage(fake_msg2));
  EXPECT_CALL(*env_, DeleteMessage(fake_msg1));
}

TEST_F(ProtoTest, LifecycleAdoptNewRefUnownedAndMove) {
  auto* raw_msg = Fake<google::protobuf::Message*>(5);

  {
    // Unowned wrapper must not delete the underlying message on scope exit.
    EXPECT_CALL(*env_, DeleteMessage(_)).Times(0);
    proto::Message<kCtadTemplate> unowned{Unowned{}, raw_msg};
    EXPECT_EQ(unowned.RawMessage(), raw_msg);
  }

  {
    // AdoptLocal takes ownership without calling NewMessage/CloneMessage, and
    // deletes once even after move construction.
    EXPECT_CALL(*env_, NewMessage(_)).Times(0);
    EXPECT_CALL(*env_, CloneMessage(_)).Times(0);
    EXPECT_CALL(*env_, DeleteMessage(raw_msg)).Times(1);

    proto::Message<kCtadTemplate> adopted{AdoptLocal{}, raw_msg};
    proto::Message<kCtadTemplate> moved{std::move(adopted)};
    EXPECT_EQ(moved.RawMessage(), raw_msg);
  }

  {
    // NewRef clones the message and deletes the clone on scope exit.
    auto* cloned_msg = AsClonedMessage(raw_msg);
    EXPECT_CALL(*env_, CloneMessage(raw_msg)).WillOnce(Return(cloned_msg));
    EXPECT_CALL(*env_, DeleteMessage(cloned_msg)).Times(1);

    proto::Message<kCtadTemplate> cloned{NewRef{}, raw_msg};
    EXPECT_EQ(cloned.RawMessage(), cloned_msg);
  }
}

static constexpr proto::Template kSubMessageTemplate{
    "pkg.SubProto",
    proto::Required<1, int>{"sub_id"},
};

static constexpr proto::Template kParentWithNestedAndRepeated{
    "pkg.ParentProto",
    proto::Optional<1, proto::SubMessage<kSubMessageTemplate>>{"child"},
    proto::Repeated<2, std::string>{"tags"},
};

TEST_F(ProtoTest, NestedMessageAndRepeatedFieldOperations) {
  const auto* parent_desc = Fake<const google::protobuf::Descriptor*>(1);
  const auto* child_field = Fake<const google::protobuf::FieldDescriptor*>(11);
  const auto* sub_id_field = Fake<const google::protobuf::FieldDescriptor*>(12);
  const auto* tags_field = Fake<const google::protobuf::FieldDescriptor*>(22);
  auto* parent_msg = Fake<google::protobuf::Message*>(1);
  auto* mutable_sub_msg = Fake<google::protobuf::Message*>(2);

  EXPECT_CALL(*env_, FindMessageTypeByName(StrEq("pkg.ParentProto")))
      .WillOnce(Return(parent_desc));
  EXPECT_CALL(*env_, NewMessage(parent_desc)).WillOnce(Return(parent_msg));

  proto::Message<kParentWithNestedAndRepeated> parent{};

  // Accessing nested message returns an Unowned Message<kSubMessageTemplate>.
  const auto* sub_desc = Fake<const google::protobuf::Descriptor*>(2);
  EXPECT_CALL(*env_, GetFieldDescriptor(parent_desc, 1, StrEq("child")))
      .WillOnce(Return(child_field));
  EXPECT_CALL(*env_, MutableMessageField(parent_msg, child_field))
      .WillOnce(Return(mutable_sub_msg));
  EXPECT_CALL(*env_, FindMessageTypeByName(StrEq("pkg.SubProto")))
      .WillOnce(Return(sub_desc));
  EXPECT_CALL(*env_, GetFieldDescriptor(sub_desc, 1, StrEq("sub_id")))
      .WillOnce(Return(sub_id_field));
  EXPECT_CALL(*env_, SetInt32Field(mutable_sub_msg, sub_id_field, 99));

  parent["child"].Get()["sub_id"].Set(99);

  // Repeated field operations: Add, Size, At.
  EXPECT_CALL(*env_, GetFieldDescriptor(parent_desc, 2, StrEq("tags")))
      .WillOnce(Return(tags_field));
  EXPECT_CALL(*env_, AddStringField(parent_msg, tags_field, "alpha"));
  EXPECT_CALL(*env_, AddStringField(parent_msg, tags_field, "beta"));
  parent["tags"].Add("alpha");
  parent["tags"].Add("beta");

  EXPECT_CALL(*env_, FieldSize(parent_msg, tags_field)).WillOnce(Return(2));
  EXPECT_EQ(parent["tags"].Size(), 2);

  EXPECT_CALL(*env_, GetRepeatedStringField(parent_msg, tags_field, 1))
      .WillOnce(Return("beta"));
  EXPECT_EQ(parent["tags"].At(1), "beta");

  // Only parent_msg is deleted (mutable_sub_msg is unowned).
  EXPECT_CALL(*env_, DeleteMessage(parent_msg)).Times(1);
}

static constexpr proto::Template kEmptyTemplate{"pkg.EmptyProto"};

TEST_F(ProtoTest, EmptyTemplateAndContainsEdgeCases) {
  const auto* empty_desc = Fake<const google::protobuf::Descriptor*>(9);
  auto* empty_msg = Fake<google::protobuf::Message*>(9);

  EXPECT_CALL(*env_, FindMessageTypeByName(StrEq("pkg.EmptyProto")))
      .WillOnce(Return(empty_desc));
  EXPECT_CALL(*env_, NewMessage(empty_desc)).WillOnce(Return(empty_msg));
  EXPECT_CALL(*env_, DeleteMessage(empty_msg)).Times(1);

  proto::Message<kEmptyTemplate> msg{};
  EXPECT_FALSE(msg.Contains("non_existent_field"));

  // Verify Contains on non-empty template.
  const auto* ctad_desc = Fake<const google::protobuf::Descriptor*>(10);
  auto* ctad_msg = Fake<google::protobuf::Message*>(10);
  EXPECT_CALL(*env_, FindMessageTypeByName(_)).WillOnce(Return(ctad_desc));
  EXPECT_CALL(*env_, NewMessage(ctad_desc)).WillOnce(Return(ctad_msg));
  EXPECT_CALL(*env_, DeleteMessage(ctad_msg)).Times(1);

  proto::Message<kCtadTemplate> ctad{};
  EXPECT_TRUE(ctad.Contains("some_optional_int_field"));
  EXPECT_TRUE(ctad.Contains("some_string_field"));
  EXPECT_FALSE(ctad.Contains("unknown_field"));
}

TEST_F(ProtoTest, DynamicDescriptorFallbackWhenFindReturnsNullptr) {
  const auto* synth_desc = Fake<const google::protobuf::Descriptor*>(42);
  auto* synth_msg = Fake<google::protobuf::Message*>(42);

  // Simulate FindMessageTypeByName returning nullptr so DescriptorRef
  // synthesizes a FileDescriptorProto and calls RegisterDynamicDescriptor.
  EXPECT_CALL(*env_, FindMessageTypeByName(_)).WillOnce(Return(nullptr));
  EXPECT_CALL(*env_, RegisterDynamicDescriptor(_, _))
      .WillOnce(Return(synth_desc));
  EXPECT_CALL(*env_, NewMessage(synth_desc)).WillOnce(Return(synth_msg));
  EXPECT_CALL(*env_, DeleteMessage(synth_msg)).Times(1);

  proto::Message<kCtadTemplate> msg{};
  EXPECT_EQ(msg.RawMessage(), synth_msg);
}

TEST_F(ProtoTest,
       DynamicDescriptorFallbackWithNestedSubMessageDoesNotDereferenceFakePtr) {
  const auto* synth_sub_desc = Fake<const google::protobuf::Descriptor*>(43);
  const auto* synth_parent_desc = Fake<const google::protobuf::Descriptor*>(44);
  auto* synth_parent_msg = Fake<google::protobuf::Message*>(44);

  // When `FindMessageTypeByName` returns nullptr for both parent and child,
  // `DescriptorRef` synthesizes both without dereferencing `synth_sub_desc`.
  EXPECT_CALL(*env_, FindMessageTypeByName(StrEq("pkg.ParentProto")))
      .WillOnce(Return(nullptr));
  EXPECT_CALL(*env_, FindMessageTypeByName(StrEq("pkg.SubProto")))
      .WillOnce(Return(nullptr));
  EXPECT_CALL(*env_, RegisterDynamicDescriptor(_, StrEq("pkg.SubProto")))
      .WillOnce(Return(synth_sub_desc));
  EXPECT_CALL(*env_, RegisterDynamicDescriptor(_, StrEq("pkg.ParentProto")))
      .WillOnce(Return(synth_parent_desc));
  EXPECT_CALL(*env_, NewMessage(synth_parent_desc))
      .WillOnce(Return(synth_parent_msg));
  EXPECT_CALL(*env_, DeleteMessage(synth_parent_msg)).Times(1);

  proto::Message<kParentWithNestedAndRepeated> parent{};
  EXPECT_EQ(parent.RawMessage(), synth_parent_msg);
}

TEST_F(ProtoTest, RvalueSubMessageSetAndConstructDoesNotLeakTemporary) {
  const auto* parent_desc = Fake<const google::protobuf::Descriptor*>(50);
  const auto* sub_desc = Fake<const google::protobuf::Descriptor*>(51);
  const auto* child_field = Fake<const google::protobuf::FieldDescriptor*>(52);
  const auto* sub_id_field = Fake<const google::protobuf::FieldDescriptor*>(53);
  auto* parent_msg = Fake<google::protobuf::Message*>(50);
  auto* temp_sub_msg = Fake<google::protobuf::Message*>(51);

  EXPECT_CALL(*env_, FindMessageTypeByName(StrEq("pkg.ParentProto")))
      .WillOnce(Return(parent_desc));
  EXPECT_CALL(*env_, NewMessage(parent_desc)).WillOnce(Return(parent_msg));

  proto::Message<kParentWithNestedAndRepeated> parent{};

  // Constructing a temporary `proto::Message<kSubMessageTemplate>{77}` and
  // passing it as an rvalue to `Set(...)` must copy via `SetMessageField` AND
  // delete `temp_sub_msg` when the temporary goes out of scope (no `.Release()`
  // leak).
  EXPECT_CALL(*env_, FindMessageTypeByName(StrEq("pkg.SubProto")))
      .WillOnce(Return(sub_desc));
  EXPECT_CALL(*env_, NewMessage(sub_desc)).WillOnce(Return(temp_sub_msg));
  EXPECT_CALL(*env_, GetFieldDescriptor(sub_desc, 1, StrEq("sub_id")))
      .WillOnce(Return(sub_id_field));
  EXPECT_CALL(*env_, SetInt32Field(temp_sub_msg, sub_id_field, 77));
  EXPECT_CALL(*env_, GetFieldDescriptor(parent_desc, 1, StrEq("child")))
      .WillOnce(Return(child_field));
  EXPECT_CALL(*env_, SetMessageField(parent_msg, child_field, temp_sub_msg));
  EXPECT_CALL(*env_, DeleteMessage(temp_sub_msg)).Times(1);
  EXPECT_CALL(*env_, DeleteMessage(parent_msg)).Times(1);

  parent["child"].Set(proto::Message<kSubMessageTemplate>{77});
}

TEST_F(ProtoTest, SerializeAndParseRouteThroughMockProtoEnv) {
  const auto* fake_desc = Fake<const google::protobuf::Descriptor*>(60);
  auto* fake_msg = Fake<google::protobuf::Message*>(60);

  EXPECT_CALL(*env_, FindMessageTypeByName(_)).WillOnce(Return(fake_desc));
  EXPECT_CALL(*env_, NewMessage(fake_desc)).WillOnce(Return(fake_msg));
  EXPECT_CALL(*env_, SerializeAsString(fake_msg))
      .WillOnce(Return("encoded_payload"));
  EXPECT_CALL(*env_, ParseFromString(fake_msg, StrEq("encoded_payload")))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, DeleteMessage(fake_msg)).Times(1);

  proto::Message<kCtadTemplate> msg{};
  EXPECT_EQ(msg.SerializeAsString(), "encoded_payload");
  EXPECT_TRUE(msg.ParseFromString("encoded_payload"));
}

static constexpr proto::Template kAllRepeatedTemplate{
    "pkg.AllRepeatedProto",
    proto::Repeated<1, bool>{"bools"},
    proto::Repeated<2, int32_t>{"i32s"},
    proto::Repeated<3, uint32_t>{"u32s"},
    proto::Repeated<4, int64_t>{"i64s"},
    proto::Repeated<5, uint64_t>{"u64s"},
    proto::Repeated<6, float>{"floats"},
    proto::Repeated<7, double>{"doubles"},
    proto::Repeated<8, std::string>{"strings"},
    proto::Repeated<9, proto::SubMessage<kSubMessageTemplate>>{"subs"},
};

TEST_F(ProtoTest, RepeatedFieldsSupportAllTypesSubscriptSetAndVariadicInit) {
  const auto* desc = Fake<const google::protobuf::Descriptor*>(70);
  const auto* sub_desc = Fake<const google::protobuf::Descriptor*>(71);
  auto* msg = Fake<google::protobuf::Message*>(70);
  auto* added_sub = Fake<google::protobuf::Message*>(72);
  auto* elem_sub = Fake<google::protobuf::Message*>(73);
  auto* rval_sub = Fake<google::protobuf::Message*>(74);

  EXPECT_CALL(*env_, FindMessageTypeByName(StrEq("pkg.AllRepeatedProto")))
      .WillOnce(Return(desc));
  EXPECT_CALL(*env_, NewMessage(desc)).WillOnce(Return(msg));

  // Variadic constructor with range values and std::nullopt for Repeated
  // fields.
  EXPECT_CALL(*env_, AddBoolField(msg, _, true));
  EXPECT_CALL(*env_, AddBoolField(msg, _, false));
  EXPECT_CALL(*env_, AddInt32Field(msg, _, 10));
  EXPECT_CALL(*env_, AddInt32Field(msg, _, 20));

  proto::Message<kAllRepeatedTemplate> m{
      std::vector<bool>{true, false},
      std::vector<int>{10, 20},
      std::nullopt,
      std::nullopt,
      std::nullopt,
      std::nullopt,
      std::nullopt,
      std::vector<std::string>{"x"},
      std::nullopt,
  };

  // Verify Set(index, val) and operator[](index) on repeated scalar fields.
  EXPECT_CALL(*env_, SetRepeatedUInt32Field(msg, _, 0, 111u));
  m["u32s"].Set(0, 111u);
  EXPECT_CALL(*env_, GetRepeatedUInt32Field(msg, _, 0)).WillOnce(Return(111u));
  EXPECT_EQ(m["u32s"][0], 111u);

  EXPECT_CALL(*env_, SetRepeatedInt64Field(msg, _, 0, -222LL));
  m["i64s"].Set(0, -222LL);
  EXPECT_CALL(*env_, GetRepeatedInt64Field(msg, _, 0)).WillOnce(Return(-222LL));
  EXPECT_EQ(m["i64s"][0], -222LL);

  EXPECT_CALL(*env_, SetRepeatedUInt64Field(msg, _, 0, 333ULL));
  m["u64s"].Set(0, 333ULL);
  EXPECT_CALL(*env_, GetRepeatedUInt64Field(msg, _, 0))
      .WillOnce(Return(333ULL));
  EXPECT_EQ(m["u64s"][0], 333ULL);

  EXPECT_CALL(*env_, SetRepeatedFloatField(msg, _, 0, 1.5f));
  m["floats"].Set(0, 1.5f);
  EXPECT_CALL(*env_, GetRepeatedFloatField(msg, _, 0)).WillOnce(Return(1.5f));
  EXPECT_FLOAT_EQ(m["floats"][0], 1.5f);

  EXPECT_CALL(*env_, SetRepeatedDoubleField(msg, _, 0, 2.5));
  m["doubles"].Set(0, 2.5);
  EXPECT_CALL(*env_, GetRepeatedDoubleField(msg, _, 0)).WillOnce(Return(2.5));
  EXPECT_DOUBLE_EQ(m["doubles"][0], 2.5);

  // Repeated SubMessage: zero-arg Add(), At(index) / operator[](index), and
  // Add(rvalue Message).
  EXPECT_CALL(*env_, AddMutableMessageField(msg, _))
      .WillOnce(Return(added_sub));
  EXPECT_CALL(*env_, FindMessageTypeByName(StrEq("pkg.SubProto")))
      .WillOnce(Return(sub_desc));
  EXPECT_CALL(*env_, SetInt32Field(added_sub, _, 404));
  m["subs"].Add()["sub_id"].Set(404);

  EXPECT_CALL(*env_, MutableRepeatedMessageField(msg, _, 0))
      .WillOnce(Return(elem_sub));
  EXPECT_CALL(*env_, GetInt32Field(elem_sub, _)).WillOnce(Return(404));
  EXPECT_EQ(m["subs"][0]["sub_id"].Get(), 404);

  EXPECT_CALL(*env_, NewMessage(sub_desc)).WillOnce(Return(rval_sub));
  EXPECT_CALL(*env_, SetInt32Field(rval_sub, _, 505));
  EXPECT_CALL(*env_, AddMessageField(msg, _, rval_sub));
  EXPECT_CALL(*env_, DeleteMessage(rval_sub)).Times(1);
  m["subs"].Add(proto::Message<kSubMessageTemplate>{505});

  EXPECT_CALL(*env_, DeleteMessage(msg)).Times(1);
}

TEST_F(ProtoTest, MoveAssignmentAndFieldAliases) {
  const auto* fake_desc = Fake<const google::protobuf::Descriptor*>(80);
  auto* msg1_ptr = Fake<google::protobuf::Message*>(81);
  auto* msg2_ptr = Fake<google::protobuf::Message*>(82);

  EXPECT_CALL(*env_, FindMessageTypeByName(_)).WillOnce(Return(fake_desc));
  EXPECT_CALL(*env_, NewMessage(fake_desc))
      .WillOnce(Return(msg1_ptr))
      .WillOnce(Return(msg2_ptr));

  proto::Message<kCtadTemplate> msg1{};
  proto::Message<kCtadTemplate> msg2{};

  // Move-assigning msg2 into msg1 deletes msg1's old message immediately and
  // transfers ownership of msg2_ptr.
  EXPECT_CALL(*env_, DeleteMessage(msg1_ptr)).Times(1);
  msg1 = std::move(msg2);
  EXPECT_EQ(msg1.RawMessage(), msg2_ptr);
  EXPECT_EQ(msg2.RawMessage(), nullptr);  // NOLINT(bugprone-use-after-move)

  EXPECT_CALL(*env_, SetInt32Field(msg2_ptr, _, 321));
  msg1.field<0>().Set(321);
  EXPECT_CALL(*env_, SetStringField(msg2_ptr, _, "via_field_at"));
  msg1.field_at<1>().Set("via_field_at");

  EXPECT_CALL(*env_, DeleteMessage(msg2_ptr)).Times(1);
}

}  // namespace
