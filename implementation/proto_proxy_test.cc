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
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>
#include "proto_bind.h"
#include "proto_dep.h"

namespace {

using ::proto::Optional;
using ::proto::ProtoAsDecl_t;
using ::proto::ProtoCDecl_t;
using ::proto::ProtoId;
using ::proto::ProtoProxy_t;
using ::proto::ProtoReturn_t;
using ::proto::ProtoT;
using ::proto::Required;
using ::proto::SubMessage;
using ::proto::Template;

// Static tests verifying that `ProtoProxy` shares `GenericProxyHelper`,
// `ConvertibleIntegralProxyBase`, and `ObjectProxyBase` with `jni::Proxy`.
static_assert(std::is_same_v<ProtoCDecl_t<void>, void>);
static_assert(std::is_same_v<ProtoCDecl_t<bool>, bool>);
static_assert(std::is_same_v<ProtoCDecl_t<int>, int32_t>);
static_assert(std::is_same_v<ProtoCDecl_t<int32_t>, int32_t>);
static_assert(std::is_same_v<ProtoCDecl_t<uint32_t>, uint32_t>);
static_assert(std::is_same_v<ProtoCDecl_t<int64_t>, int64_t>);
static_assert(
    std::is_same_v<ProtoCDecl_t<long>, int64_t>);  // NOLINT(runtime/int)
static_assert(
    std::is_same_v<ProtoCDecl_t<long long>, int64_t>);  // NOLINT(runtime/int)
static_assert(std::is_same_v<ProtoCDecl_t<uint64_t>, uint64_t>);
static_assert(std::is_same_v<ProtoCDecl_t<unsigned long>,  // NOLINT
                             uint64_t>);
static_assert(std::is_same_v<ProtoCDecl_t<unsigned long long>,  // NOLINT
                             uint64_t>);
static_assert(std::is_same_v<ProtoCDecl_t<float>, float>);
static_assert(std::is_same_v<ProtoCDecl_t<double>, double>);

// String declarations map `std::string`, `const char*`, and `std::string_view`
// to canonical `std::string`.
static_assert(std::is_same_v<ProtoCDecl_t<std::string>, std::string>);
static_assert(std::is_same_v<ProtoCDecl_t<const char*>, std::string>);
static_assert(std::is_same_v<ProtoCDecl_t<std::string_view>, std::string>);
static_assert(
    std::is_same_v<ProtoAsDecl_t<std::string>,
                   std::tuple<std::string, const char*, std::string_view>>);

// Nested `proto::Template` declarations map to `google::protobuf::Message*` via
// the shared `bind_lang::ObjectProxyBase`.
static constexpr Template kNestedSubTemplate{
    "pkg.SubMessage",
    Optional<1, int>{"sub_int"},
};

static constexpr Template kOuterTemplate{
    "pkg.OuterMessage",
    Optional<0, int>{"some_optional_int_field"},
    Required<1, std::string>{"some_string_field"},
    Optional<2, SubMessage<kNestedSubTemplate>>{"nested_msg"},
};

static_assert(std::is_same_v<ProtoCDecl_t<SubMessage<kNestedSubTemplate>>,
                             google::protobuf::Message*>);
static_assert(std::is_same_v<ProtoCDecl_t<decltype(kNestedSubTemplate)>,
                             google::protobuf::Message*>);
static_assert(std::is_same_v<ProtoCDecl_t<google::protobuf::Message*>,
                             google::protobuf::Message*>);

using OuterProtoT = ProtoT<kOuterTemplate>;
using Field0Id = ProtoId<OuterProtoT, bind_lang::IdType::FIELD, 0>;
using Field1Id = ProtoId<OuterProtoT, bind_lang::IdType::FIELD, 1>;
using Field2Id = ProtoId<OuterProtoT, bind_lang::IdType::FIELD, 2>;

static_assert(std::is_same_v<ProtoReturn_t<int, Field0Id>, int32_t>);
static_assert(
    std::is_same_v<ProtoReturn_t<std::string, Field1Id>, std::string>);
static_assert(std::is_same_v<
              ProtoReturn_t<google::protobuf::Message*, Field2Id>::SpanType,
              google::protobuf::Message*>);

TEST(ProtoProxyTest, ConvertsStringArgumentsProperly) {
  EXPECT_EQ(ProtoProxy_t<const char*>::ProxyAsArg("hello"), "hello");
  EXPECT_EQ(
      ProtoProxy_t<std::string_view>::ProxyAsArg(std::string_view{"world"}),
      "world");
  EXPECT_EQ(ProtoProxy_t<std::string>::ProxyAsArg(std::string{"owned"}),
            "owned");
}

TEST(ProtoProxyTest, Converts64BitIntegralArgumentsViaSharedBase) {
  long l_val = 42L;         // NOLINT(runtime/int)
  long long ll_val = 99LL;  // NOLINT(runtime/int)
  int16_t s_val = 12;
  int32_t i32_val = 34;
  float f_val = 1.5f;
  EXPECT_EQ(ProtoProxy_t<long>::ProxyAsArg(l_val), 42);        // NOLINT
  EXPECT_EQ(ProtoProxy_t<long long>::ProxyAsArg(ll_val), 99);  // NOLINT
  EXPECT_EQ(ProtoProxy_t<int32_t>::ProxyAsArg(s_val), 12);
  EXPECT_EQ(ProtoProxy_t<int64_t>::ProxyAsArg(i32_val), 34);
  EXPECT_DOUBLE_EQ(ProtoProxy_t<double>::ProxyAsArg(f_val), 1.5);
}

// Verify `Template::operator==` properly distinguishes distinct anonymous
// templates while treating identical anonymous templates as equal.
static constexpr Template kAnonA1{
    Optional<0, int>{"field_a"},
};
static constexpr Template kAnonA2{
    Optional<0, int>{"field_a"},
};
static constexpr Template kAnonDiffName{
    Optional<0, int>{"field_b"},
};
static constexpr Template kAnonDiffType{
    Optional<0, std::string>{"field_a"},
};
static constexpr Template kAnonDiffTag{
    Optional<1, int>{"field_a"},
};
static constexpr Template kAnonDiffLabel{
    Required<0, int>{"field_a"},
};
static constexpr Template kAnonRepeated{
    proto::Repeated<0, int>{"field_a"},
};
static constexpr Template kAnonParentWithA1{
    Optional<0, SubMessage<kAnonA1>>{"sub"},
};
static constexpr Template kAnonParentWithDiffType{
    Optional<0, SubMessage<kAnonDiffType>>{"sub"},
};

static_assert(kAnonA1 == kAnonA2);
static_assert(kAnonA1 != kAnonDiffName);
static_assert(kAnonA1 != kAnonDiffType);
static_assert(kAnonA1 != kAnonDiffTag);
static_assert(kAnonA1 != kAnonDiffLabel);
static_assert(kAnonA1 != kAnonRepeated);
static_assert(kAnonParentWithA1 != kAnonParentWithDiffType);

// Verify `ProtoFieldSelection` viability for nested `SubMessage` fields:
// accepts matching `proto::Message`, rejects mismatched `proto::Message` (even
// when both are anonymous!), and accepts raw `google::protobuf::Message*`.
using AnonParentProtoT = ProtoT<kAnonParentWithA1>;
static_assert(proto::ProtoFieldSelection<AnonParentProtoT, 0>::Valable<
              proto::Message<kAnonA1>>());
static_assert(proto::ProtoFieldSelection<AnonParentProtoT, 0>::Valable<
              proto::Message<kAnonA2>>());
static_assert(!proto::ProtoFieldSelection<AnonParentProtoT, 0>::Valable<
              proto::Message<kAnonDiffType>>());
static_assert(proto::ProtoFieldSelection<AnonParentProtoT, 0>::Valable<
              google::protobuf::Message*>());

// Verify `SchemaHash` produces distinct dynamic message names for anonymous
// schemas that differ only in label or nested sub-message schema.
static_assert(proto::DescriptorRef<ProtoT<kAnonA1>>::SchemaHash() !=
              proto::DescriptorRef<ProtoT<kAnonRepeated>>::SchemaHash());
static_assert(
    proto::DescriptorRef<ProtoT<kAnonParentWithA1>>::SchemaHash() !=
    proto::DescriptorRef<ProtoT<kAnonParentWithDiffType>>::SchemaHash());

}  // namespace
