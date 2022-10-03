/*
 * Copyright 2022 Google LLC
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

#include <string_view>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"

namespace {

using ::jni::Array;
using ::jni::Class;
using ::jni::Constructor;
using ::jni::Field;
using ::jni::Id;
using ::jni::IdType;
using ::jni::JniType;
using ::jni::kNoIdxSpecified;
using ::jni::Method;
using ::jni::Overload;
using ::jni::Params;
using ::jni::Return;

static constexpr Class kClass1{
    "kClass1",
    Constructor{},
    Constructor{jint{}},
    Constructor{jfloat{}, jboolean{}},
    Constructor{Array{jint{}}},
    Method{"m0", jni::Return<void>{}, Params{}},
    Method{"m1", jni::Return<jshort>{}, Params<jint>{}},
    Method{"m2", jni::Return{Class{"kClass2"}}, Params<jfloat, jboolean>{}},
    Method{
        "m3",
        Overload{jni::Return<void>{}, Params{}},
        Overload{jni::Return<jint>{}, Params<jboolean>{}},
        Overload{jni::Return<jfloat>{}, Params<jshort, jdouble>{}},
    },
    Method{
        "m4",
        Overload{jni::Return{Array{jboolean{}}}, Params{Array{jint{}}}},
        Overload{jni::Return{Array{Array{jboolean{}}}},
                 Params{Array{Array{jfloat{}}}}},
        Overload{jni::Return{Array{Array{Array{jboolean{}}}}},
                 Params{Array{Array{Array{jshort{}}}}}},
    },
    Field{"f0", int{}},
    Field{"f1", Class{"kClass2"}}};

using JT = JniType<jobject, kClass1>;

////////////////////////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////////////////////////
using kCtor0 = Id<JT, IdType::OVERLOAD, kNoIdxSpecified, 0>;
using kCtor1 = Id<JT, IdType::OVERLOAD, kNoIdxSpecified, 1>;
using kCtor2 = Id<JT, IdType::OVERLOAD, kNoIdxSpecified, 2>;
using kCtor3 = Id<JT, IdType::OVERLOAD, kNoIdxSpecified, 3>;
using kCtor4 = Id<JT, IdType::OVERLOAD, kNoIdxSpecified, 4>;

static_assert(kCtor0::NumParams() == 0);
static_assert(kCtor1::NumParams() == 1);
static_assert(kCtor2::NumParams() == 2);
static_assert(kCtor3::NumParams() == 1);

static_assert(kCtor0::Name() == std::string_view{"<init>"});
static_assert(kCtor1::Name() == std::string_view{"<init>"});
static_assert(kCtor2::Name() == std::string_view{"<init>"});
static_assert(kCtor3::Name() == std::string_view{"<init>"});

static_assert(0 ==
              Id<JT, IdType::OVERLOAD_PARAM, kNoIdxSpecified, 1, 0>::kRank);
static_assert(0 ==
              Id<JT, IdType::OVERLOAD_PARAM, kNoIdxSpecified, 2, 0>::kRank);
static_assert(0 ==
              Id<JT, IdType::OVERLOAD_PARAM, kNoIdxSpecified, 2, 0>::kRank);
static_assert(1 ==
              Id<JT, IdType::OVERLOAD_PARAM, kNoIdxSpecified, 3, 0>::kRank);

static_assert(
    std::string_view{"I"} ==
    Id<JT, IdType::OVERLOAD_PARAM, kNoIdxSpecified, 1, 0>::Signature());
static_assert(
    std::string_view{"F"} ==
    Id<JT, IdType::OVERLOAD_PARAM, kNoIdxSpecified, 2, 0>::Signature());
static_assert(
    std::string_view{"Z"} ==
    Id<JT, IdType::OVERLOAD_PARAM, kNoIdxSpecified, 2, 1>::Signature());
static_assert(
    std::string_view{"[I"} ==
    Id<JT, IdType::OVERLOAD_PARAM, kNoIdxSpecified, 3, 0>::Signature());

static_assert(std::string_view{"()V"} == kCtor0::Signature());
static_assert(std::string_view{"(I)V"} == kCtor1::Signature());
static_assert(std::string_view{"(FZ)V"} == kCtor2::Signature());

////////////////////////////////////////////////////////////////////////////////
// Methods (Overload sets with only one overload)
////////////////////////////////////////////////////////////////////////////////
using kMethod0 = Id<JT, IdType::OVERLOAD_SET, 0>;
using kMethod1 = Id<JT, IdType::OVERLOAD_SET, 1>;
using kMethod2 = Id<JT, IdType::OVERLOAD_SET, 2>;
using kMethod3 = Id<JT, IdType::OVERLOAD_SET, 3>;

static_assert(kMethod0::Name() == std::string_view{"m0"});
static_assert(kMethod1::Name() == std::string_view{"m1"});
static_assert(kMethod2::Name() == std::string_view{"m2"});
static_assert(kMethod3::Name() == std::string_view{"m3"});

using kMethod0Overload0 = Id<JT, IdType::OVERLOAD, 0, 0>;
using kMethod1Overload0 = Id<JT, IdType::OVERLOAD, 1, 0>;
using kMethod2Overload0 = Id<JT, IdType::OVERLOAD, 2, 0>;
using kMethod3Overload0 = Id<JT, IdType::OVERLOAD, 3, 0>;

static_assert(kMethod0Overload0::NumParams() == 0);
static_assert(kMethod1Overload0::NumParams() == 1);
static_assert(kMethod2Overload0::NumParams() == 2);

static_assert(0 == Id<JT, IdType::OVERLOAD_PARAM, 0, 0>::kRank);
static_assert(0 == Id<JT, IdType::OVERLOAD_PARAM, 1, 0>::kRank);
static_assert(0 == Id<JT, IdType::OVERLOAD_PARAM, 1, 0, 0>::kRank);
static_assert(0 == Id<JT, IdType::OVERLOAD_PARAM, 2, 0, 0>::kRank);
static_assert(0 == Id<JT, IdType::OVERLOAD_PARAM, 2, 0, 1>::kRank);

static_assert(std::string_view{"S"} ==
              Id<JT, IdType::OVERLOAD_PARAM, 1, 0>::Signature());
static_assert(std::string_view{"I"} ==
              Id<JT, IdType::OVERLOAD_PARAM, 1, 0, 0>::Signature());
static_assert(
    std::string_view{"LkClass2;"} ==
    Id<JT, IdType::OVERLOAD_PARAM, 2, 0, kNoIdxSpecified>::Signature());
static_assert(std::string_view{"F"} ==
              Id<JT, IdType::OVERLOAD_PARAM, 2, 0, 0>::Signature());
static_assert(std::string_view{"Z"} ==
              Id<JT, IdType::OVERLOAD_PARAM, 2, 0, 1>::Signature());

static_assert(std::string_view{"V"} ==
              Id<JT, IdType::OVERLOAD_PARAM, 0, 0>::Signature());
static_assert(std::string_view{"S"} ==
              Id<JT, IdType::OVERLOAD_PARAM, 1, 0>::Signature());
static_assert(std::string_view{"I"} ==
              Id<JT, IdType::OVERLOAD_PARAM, 1, 0, 0>::Signature());
static_assert(
    std::string_view{"LkClass2;"} ==
    Id<JT, IdType::OVERLOAD_PARAM, 2, 0, kNoIdxSpecified>::Signature());
static_assert(std::string_view{"F"} ==
              Id<JT, IdType::OVERLOAD_PARAM, 2, 0, 0>::Signature());
static_assert(std::string_view{"Z"} ==
              Id<JT, IdType::OVERLOAD_PARAM, 2, 0, 1>::Signature());

static_assert(std::string_view{"()V"} == kMethod0Overload0::Signature());
static_assert(std::string_view{"(I)S"} == kMethod1Overload0::Signature());
static_assert(std::string_view{"(FZ)LkClass2;"} ==
              kMethod2Overload0::Signature());

////////////////////////////////////////////////////////////////////////////////
// Overloaded Method (smoke test, they should behave the same).
////////////////////////////////////////////////////////////////////////////////
using kMethod3Overload0 = Id<JT, IdType::OVERLOAD, 3, 0>;
using kMethod3Overload1 = Id<JT, IdType::OVERLOAD, 3, 1>;
using kMethod3Overload2 = Id<JT, IdType::OVERLOAD, 3, 2>;

using kMethod4Overload0 = Id<JT, IdType::OVERLOAD, 4, 0>;
using kMethod4Overload1 = Id<JT, IdType::OVERLOAD, 4, 1>;
using kMethod4Overload2 = Id<JT, IdType::OVERLOAD, 4, 2>;

static_assert(kMethod3Overload0::NumParams() == 0);
static_assert(kMethod3Overload1::NumParams() == 1);
static_assert(kMethod3Overload2::NumParams() == 2);

static_assert(std::string_view{"m3"} == kMethod3Overload0::Name());
static_assert(std::string_view{"m4"} == kMethod4Overload0::Name());

using kMethod4Overload0Return = Id<JT, IdType::OVERLOAD_PARAM, 4, 0>;
using kMethod4Overload0Param0 = Id<JT, IdType::OVERLOAD_PARAM, 4, 0, 0>;

using kMethod4Overload1Return = Id<JT, IdType::OVERLOAD_PARAM, 4, 1>;
using kMethod4Overload1Param0 = Id<JT, IdType::OVERLOAD_PARAM, 4, 1, 0>;

using kMethod4Overload2Return = Id<JT, IdType::OVERLOAD_PARAM, 4, 2>;
using kMethod4Overload2Param0 = Id<JT, IdType::OVERLOAD_PARAM, 4, 2, 0>;

static_assert(1 == kMethod4Overload0Param0::kRank);
static_assert(1 == kMethod4Overload0Return::kRank);

static_assert(2 == kMethod4Overload1Param0::kRank);
static_assert(2 == kMethod4Overload1Return::kRank);

static_assert(3 == kMethod4Overload2Param0::kRank);
static_assert(3 == kMethod4Overload2Return::kRank);

static_assert(std::string_view{"()V"} == kMethod3Overload0::Signature());
static_assert(std::string_view{"(Z)I"} == kMethod3Overload1::Signature());
static_assert(std::string_view{"(SD)F"} == kMethod3Overload2::Signature());

static_assert(std::string_view{"([I)[Z"} == kMethod4Overload0::Signature());
static_assert(std::string_view{"([[F)[[Z"} == kMethod4Overload1::Signature());
static_assert(std::string_view{"([[[S)[[[Z"} == kMethod4Overload2::Signature());

////////////////////////////////////////////////////////////////////////////////
// Fields (Overload sets with only one overload)
////////////////////////////////////////////////////////////////////////////////
using kField0 = Id<JT, IdType::FIELD, 0>;
using kField1 = Id<JT, IdType::FIELD, 1>;

static_assert(std::string_view{"f0"} == kField0::Name());
static_assert(std::string_view{"f1"} == kField1::Name());

static_assert(std::string_view{"I"} == kField0::Signature());
static_assert(std::string_view{"LkClass2;"} == kField1::Signature());

}  // namespace
