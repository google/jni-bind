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

#include <algorithm>
#include <cstdint>
#include <optional>
#include <ranges>  // NOLINT(build/c++20)
#include <string>
#include <thread>  // NOLINT(build/c++11)
#include <vector>

#include <gtest/gtest.h>
#include "implementation/proto_e2e.pb.h"
#include "proto_bind.h"

namespace {

using ::proto::Message;
using ::proto::NewRef;
using ::proto::Optional;
using ::proto::Repeated;
using ::proto::Required;
using ::proto::SubMessage;
using ::proto::Template;
using ::proto::Unowned;
using ::proto_bind::test::NestedPayload;
using ::proto_bind::test::SampleMessage;

// Force linkage of the generated SampleMessage descriptor into
// DescriptorPool::generated_pool() at static initialization time.
const auto* const kAnchorSampleDescriptor = SampleMessage::descriptor();

// 1. Exact CTAD syntax from the task specification (inline anonymous schema,
// synthesized dynamically at runtime via `DynamicMessageFactory`).
static constexpr proto::Template kInlineCtadTemplate{
    proto::Optional<0, int>{"some_optional_int_field"},
    proto::Required<1, std::string>{"some_string_field"},
};

// 2. Explicit template-argument syntax from the task specification.
static constexpr proto::Template<proto::Optional<0, int>,
                                 proto::Required<1, std::string>>
    kInlineExplicitTemplate{"some_optional_int_field", "some_string_field"};

// 3. Schema bound to compiled `.proto` messages in `proto_e2e.proto`.
static constexpr proto::Template kNestedPayloadTemplate{
    "proto_bind.test.NestedPayload",
    proto::Optional<1, int>{"code"},
    proto::Optional<2, std::string>{"label"},
};

static constexpr proto::Template kCompiledSampleTemplate{
    "proto_bind.test.SampleMessage",
    proto::Optional<1, int>{"some_optional_int_field"},
    proto::Required<2, std::string>{"some_string_field"},
    proto::Optional<3, int64_t>{"timestamp_ms"},
    proto::Optional<4, double>{"score"},
    proto::Optional<5, bool>{"active"},
    proto::Optional<6, proto::SubMessage<kNestedPayloadTemplate>>{"payload"},
    proto::Repeated<7, std::string>{"tags"},
    proto::Repeated<8, int>{"counts"},
    proto::Repeated<9, proto::SubMessage<kNestedPayloadTemplate>>{"items"},
    proto::Optional<10, uint32_t>{"flags"},
    proto::Optional<11, uint64_t>{"u64_val"},
    proto::Optional<12, float>{"f32_val"},
};

TEST(ProtoE2ETest, InlineCtadTemplateConstructsMutatesAndSerializesEndToEnd) {
  ASSERT_NE(kAnchorSampleDescriptor, nullptr);

  proto::Message<kInlineCtadTemplate> runtime_msg{123, "string_val"};

  EXPECT_TRUE(runtime_msg["some_optional_int_field"].Has());
  EXPECT_EQ(runtime_msg["some_optional_int_field"].Get(), 123);
  EXPECT_EQ(runtime_msg.Access<"some_optional_int_field">().GetOptional(), 123);

  EXPECT_TRUE(runtime_msg["some_string_field"].Has());
  EXPECT_EQ(runtime_msg["some_string_field"].Get(), "string_val");
  EXPECT_EQ(runtime_msg.AccessByTag<1>().Get(), "string_val");

  // Mutate and verify wire round-trip into a compiled `SampleMessage`
  // (tags 0 and 1 in 0-based `kInlineCtadTemplate` map to wire tags 1 and 2).
  runtime_msg["some_optional_int_field"].Set(999);
  runtime_msg["some_string_field"].Set("wire_compat");

  const std::string wire_bytes = runtime_msg.SerializeAsString();
  ASSERT_FALSE(wire_bytes.empty());

  SampleMessage parsed_compiled;
  ASSERT_TRUE(parsed_compiled.ParseFromString(wire_bytes));
  EXPECT_TRUE(parsed_compiled.has_some_optional_int_field());
  EXPECT_EQ(parsed_compiled.some_optional_int_field(), 999);
  EXPECT_TRUE(parsed_compiled.has_some_string_field());
  EXPECT_EQ(parsed_compiled.some_string_field(), "wire_compat");
}

TEST(ProtoE2ETest, InlineExplicitTemplateSupportsNulloptAndRoundTrip) {
  proto::Message<kInlineExplicitTemplate> msg{std::nullopt, "required_only"};

  EXPECT_FALSE(msg["some_optional_int_field"].Has());
  EXPECT_EQ(msg["some_optional_int_field"].GetOptional(), std::nullopt);
  EXPECT_TRUE(msg["some_string_field"].Has());
  EXPECT_EQ(msg["some_string_field"].Get(), "required_only");

  const std::string serialized = msg.SerializeAsString();

  proto::Message<kInlineExplicitTemplate> decoded{};
  ASSERT_TRUE(decoded.ParseFromString(serialized));
  EXPECT_FALSE(decoded["some_optional_int_field"].Has());
  EXPECT_EQ(decoded["some_string_field"].Get(), "required_only");
}

TEST(ProtoE2ETest, CompiledProtoFullRoundTripWithNestedAndRepeatedFields) {
  proto::Message<kCompiledSampleTemplate> msg{};

  msg["some_optional_int_field"].Set(42);
  msg["some_string_field"].Set("e2e_hello");
  msg["timestamp_ms"].Set(1700000000000LL);
  msg["score"].Set(3.14159);
  msg["active"].Set(true);
  msg["flags"].Set(0xCAFEBABEu);
  msg["u64_val"].Set(987654321012345ULL);
  msg["f32_val"].Set(2.75f);

  // Mutate nested sub-message directly through `proto::Message` proxy.
  auto nested = msg["payload"].Get();
  nested["code"].Set(200);
  nested["label"].Set("OK");

  // Populate repeated fields and exercise Set(index, val) and
  // operator[](index).
  msg["tags"].Add("first");
  msg["tags"].Add("second");
  msg["tags"].Set(1, "second_updated");
  msg["counts"].Add(10);
  msg["counts"].Add(20);
  msg["counts"].Add(30);
  msg["counts"].Set(0, 15);

  // Populate repeated SubMessage field via zero-arg Add() and Add(rvalue).
  auto item0 = msg["items"].Add();
  item0["code"].Set(100);
  item0["label"].Set("Continue");
  msg["items"].Add(proto::Message<kNestedPayloadTemplate>{404, "NotFound"});

  EXPECT_EQ(msg["tags"].Size(), 2);
  EXPECT_EQ(msg["tags"][0], "first");
  EXPECT_EQ(msg["tags"][1], "second_updated");
  EXPECT_EQ(msg["counts"].Size(), 3);
  EXPECT_EQ(msg["counts"][0], 15);
  EXPECT_EQ(msg["counts"][2], 30);
  EXPECT_EQ(msg["items"].Size(), 2);
  EXPECT_EQ(msg["items"][0]["code"].Get(), 100);
  EXPECT_EQ(msg["items"][1]["code"].Get(), 404);
  EXPECT_EQ(msg["items"][1]["label"].Get(), "NotFound");

  // Serialize via Proto Bind and parse into generated C++ protobuf class.
  const std::string wire = msg.SerializeAsString();
  SampleMessage native_proto;
  ASSERT_TRUE(native_proto.ParseFromString(wire));

  EXPECT_EQ(native_proto.some_optional_int_field(), 42);
  EXPECT_EQ(native_proto.some_string_field(), "e2e_hello");
  EXPECT_EQ(native_proto.timestamp_ms(), 1700000000000LL);
  EXPECT_DOUBLE_EQ(native_proto.score(), 3.14159);
  EXPECT_TRUE(native_proto.active());
  EXPECT_EQ(native_proto.flags(), 0xCAFEBABEu);
  EXPECT_EQ(native_proto.u64_val(), 987654321012345ULL);
  EXPECT_FLOAT_EQ(native_proto.f32_val(), 2.75f);
  EXPECT_EQ(native_proto.payload().code(), 200);
  EXPECT_EQ(native_proto.payload().label(), "OK");
  ASSERT_EQ(native_proto.tags_size(), 2);
  EXPECT_EQ(native_proto.tags(0), "first");
  EXPECT_EQ(native_proto.tags(1), "second_updated");
  ASSERT_EQ(native_proto.counts_size(), 3);
  EXPECT_EQ(native_proto.counts(0), 15);
  EXPECT_EQ(native_proto.counts(1), 20);
  EXPECT_EQ(native_proto.counts(2), 30);
  ASSERT_EQ(native_proto.items_size(), 2);
  EXPECT_EQ(native_proto.items(0).code(), 100);
  EXPECT_EQ(native_proto.items(0).label(), "Continue");
  EXPECT_EQ(native_proto.items(1).code(), 404);
  EXPECT_EQ(native_proto.items(1).label(), "NotFound");
}

TEST(ProtoE2ETest, UnownedAndNewRefInteropWithNativeGeneratedProto) {
  SampleMessage native_proto;
  native_proto.set_some_optional_int_field(7);
  native_proto.set_some_string_field("initial");

  {
    // Wrap `native_proto` without ownership and mutate in place.
    proto::Message<kCompiledSampleTemplate> unowned_view{Unowned{},
                                                         &native_proto};
    EXPECT_EQ(unowned_view["some_optional_int_field"].Get(), 7);
    unowned_view["some_optional_int_field"].Set(88);
    unowned_view["some_string_field"].Set("mutated_via_proto_bind");
  }

  // `native_proto` is still alive and reflects the mutations.
  EXPECT_EQ(native_proto.some_optional_int_field(), 88);
  EXPECT_EQ(native_proto.some_string_field(), "mutated_via_proto_bind");

  // Clone via `NewRef` and verify deep copy independence.
  proto::Message<kCompiledSampleTemplate> cloned{NewRef{}, &native_proto};
  cloned["some_optional_int_field"].Set(555);
  EXPECT_EQ(cloned["some_optional_int_field"].Get(), 555);
  EXPECT_EQ(native_proto.some_optional_int_field(), 88);
}

TEST(ProtoE2ETest, BoundaryValuesBinaryStringsAndMalformedWireInput) {
  proto::Message<kCompiledSampleTemplate> msg{};

  // Int32 / Int64 boundary values and binary string with embedded NUL byte.
  const std::string binary_str(" embedded\0nul ", 14);
  msg["some_optional_int_field"].Set(INT32_MIN);
  msg["some_string_field"].Set(binary_str);
  msg["timestamp_ms"].Set(INT64_MIN);
  msg["score"].Set(-0.0);
  msg["active"].Set(false);

  const std::string wire = msg.SerializeAsString();
  proto::Message<kCompiledSampleTemplate> roundtrip{};
  ASSERT_TRUE(roundtrip.ParseFromString(wire));
  EXPECT_EQ(roundtrip["some_optional_int_field"].Get(), INT32_MIN);
  EXPECT_EQ(roundtrip["some_string_field"].Get(), binary_str);
  EXPECT_EQ(roundtrip["timestamp_ms"].Get(), INT64_MIN);
  EXPECT_FALSE(roundtrip["active"].Get());

  // Clear field and verify presence transitions to false.
  EXPECT_TRUE(roundtrip["some_optional_int_field"].Has());
  roundtrip["some_optional_int_field"].Clear();
  EXPECT_FALSE(roundtrip["some_optional_int_field"].Has());
  EXPECT_EQ(roundtrip["some_optional_int_field"].GetOptional(), std::nullopt);

  // Malformed wire input (truncated length-delimited field) returns false.
  const std::string malformed_wire("\x12\xff\xff\xff\x7f", 5);
  EXPECT_FALSE(roundtrip.ParseFromString(malformed_wire));
}

// Dynamic nested sub-message schemas (no compiled `.proto` counterpart) and
// dynamic parent with multiple fields of the same SubMessage type + a compiled
// SubMessage child resolved via the `generated_pool()` underlay.
static constexpr proto::Template kDynamicSubTemplate{
    proto::Optional<1, int>{"code"},
    proto::Optional<2, std::string>{"label"},
};

static constexpr proto::Template kDynamicOuterWithNestedAndCompiled{
    proto::Optional<1, SubMessage<kDynamicSubTemplate>>{"primary_sub"},
    proto::Optional<2, SubMessage<kDynamicSubTemplate>>{"backup_sub"},
    proto::Repeated<3, SubMessage<kDynamicSubTemplate>>{"sub_list"},
    proto::Optional<4, SubMessage<kNestedPayloadTemplate>>{"compiled_sub"},
};

TEST(ProtoE2ETest,
     DynamicNestedSubMessageSynthesisWithDeduplicatedAndUnderlayDeps) {
  proto::Message<kDynamicOuterWithNestedAndCompiled> outer{};

  // Set via rvalue `proto::Message<kDynamicSubTemplate>`.
  outer["primary_sub"].Set(
      proto::Message<kDynamicSubTemplate>{201, "PrimaryCreated"});

  // Set via mutable proxy access.
  outer["backup_sub"].Get()["code"].Set(202);
  outer["backup_sub"].Get()["label"].Set("BackupAccepted");

  // Repeated dynamic SubMessage.
  outer["sub_list"].Add(proto::Message<kDynamicSubTemplate>{301, "Moved"});
  auto second_sub = outer["sub_list"].Add();
  second_sub["code"].Set(302);
  second_sub["label"].Set("Found");

  // Set compiled SubMessage inside a dynamically synthesized parent message,
  // both from a native C++ `NestedPayload` object and via cross-pool copy from
  // `kDynamicSubTemplate`!
  NestedPayload native_payload;
  native_payload.set_code(500);
  native_payload.set_label("NativeError");
  outer["compiled_sub"].Set(native_payload);
  EXPECT_EQ(outer["compiled_sub"].Get()["code"].Get(), 500);
  EXPECT_EQ(outer["compiled_sub"].Get()["label"].Get(), "NativeError");

  // Cross-pool copy: copy dynamic `primary_sub` (synthesized in
  // `dynamic_pool_`) into `compiled_sub` (`NestedPayload` in
  // `generated_pool()`).
  outer["compiled_sub"].Set(outer["primary_sub"].Get().RawMessage());
  EXPECT_EQ(outer["compiled_sub"].Get()["code"].Get(), 201);
  EXPECT_EQ(outer["compiled_sub"].Get()["label"].Get(), "PrimaryCreated");

  // Wire round-trip of the entire dynamically synthesized hierarchy.
  const std::string wire = outer.SerializeAsString();
  proto::Message<kDynamicOuterWithNestedAndCompiled> decoded{};
  ASSERT_TRUE(decoded.ParseFromString(wire));
  EXPECT_EQ(decoded["primary_sub"].Get()["code"].Get(), 201);
  EXPECT_EQ(decoded["primary_sub"].Get()["label"].Get(), "PrimaryCreated");
  EXPECT_EQ(decoded["backup_sub"].Get()["code"].Get(), 202);
  EXPECT_EQ(decoded["backup_sub"].Get()["label"].Get(), "BackupAccepted");
  ASSERT_EQ(decoded["sub_list"].Size(), 2);
  EXPECT_EQ(decoded["sub_list"][0]["code"].Get(), 301);
  EXPECT_EQ(decoded["sub_list"][1]["code"].Get(), 302);
  EXPECT_EQ(decoded["compiled_sub"].Get()["code"].Get(), 201);
}

// Unnamed 0-based fields and sparse 0-based fields: verify that `AccessByTag`
// and variadic construction resolve the exact synthesized field (`field_<tag>`)
// rather than colliding on +1 shifted wire numbers.
static constexpr proto::Template<proto::Optional<0, int>,
                                 proto::Required<1, std::string>>
    kUnnamedZeroBasedTemplate{};

static constexpr proto::Template kSparseZeroBasedTemplate{
    proto::Optional<0, int>{"first"},
    proto::Required<5, std::string>{"sixth"},
};

TEST(ProtoE2ETest, UnnamedAndSparseZeroBasedTemplatesDoNotCollideOnFieldTags) {
  proto::Message<kUnnamedZeroBasedTemplate> unnamed_msg{77, "unnamed_ok"};
  EXPECT_EQ(unnamed_msg.AccessByTag<0>().Get(), 77);
  EXPECT_EQ(unnamed_msg.AccessByTag<1>().Get(), "unnamed_ok");

  unnamed_msg.AccessByTag<0>().Set(88);
  unnamed_msg.AccessByTag<1>().Set("updated_unnamed");
  EXPECT_EQ(unnamed_msg.AccessByTag<0>().Get(), 88);
  EXPECT_EQ(unnamed_msg.AccessByTag<1>().Get(), "updated_unnamed");

  proto::Message<kSparseZeroBasedTemplate> sparse_msg{11, "sparse_sixth"};
  EXPECT_EQ(sparse_msg["first"].Get(), 11);
  EXPECT_EQ(sparse_msg.AccessByTag<0>().Get(), 11);
  EXPECT_EQ(sparse_msg["sixth"].Get(), "sparse_sixth");
  EXPECT_EQ(sparse_msg.AccessByTag<5>().Get(), "sparse_sixth");
}

// Distinct anonymous templates that share the same field names/tags/types but
// differ in `FieldLabel` (`Optional` vs `Repeated`) must produce distinct
// dynamic descriptors and work side-by-side without colliding in
// `dynamic_pool_`.
static constexpr proto::Template kAnonOptionalX{
    proto::Optional<0, int>{"x"},
};
static constexpr proto::Template kAnonRepeatedX{
    proto::Repeated<0, int>{"x"},
};

TEST(ProtoE2ETest,
     DistinctAnonymousTemplatesWithSameFieldNamesDoNotCollideInDynamicPool) {
  proto::Message<kAnonOptionalX> opt_msg{42};
  proto::Message<kAnonRepeatedX> rep_msg{std::vector<int>{1, 2, 3}};

  EXPECT_EQ(opt_msg["x"].Get(), 42);
  ASSERT_EQ(rep_msg["x"].Size(), 3);
  EXPECT_EQ(rep_msg["x"][0], 1);
  EXPECT_EQ(rep_msg["x"][1], 2);
  EXPECT_EQ(rep_msg["x"][2], 3);
}

TEST(ProtoE2ETest, MultiThreadedConcurrentDescriptorAndFieldInitialization) {
  proto::ReleaseAllProtoDescriptorsForTests();

  constexpr int kNumThreads = 8;
  std::vector<std::thread> threads;
  threads.reserve(kNumThreads);

  std::ranges::for_each(  // NOLINT(build/c++20)
      std::views::iota(0, kNumThreads), [&](int thread_idx) {
        threads.emplace_back([thread_idx]() {
          proto::Message<kInlineCtadTemplate> msg{
              100 + thread_idx, "thread_" + std::to_string(thread_idx)};
          EXPECT_EQ(msg["some_optional_int_field"].Get(), 100 + thread_idx);
          EXPECT_EQ(msg["some_string_field"].Get(),
                    "thread_" + std::to_string(thread_idx));
        });
      });

  std::ranges::for_each(  // NOLINT(build/c++20)
      threads, [](std::thread& t) { t.join(); });
}

}  // namespace
