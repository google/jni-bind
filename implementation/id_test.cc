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
using ::jni::kNoIdx;
using ::jni::Method;
using ::jni::Overload;
using ::jni::Params;
using ::jni::Return;
using ::jni::Static;

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
using kCtor0 = Id<JT, IdType::OVERLOAD, kNoIdx, 0>;
using kCtor1 = Id<JT, IdType::OVERLOAD, kNoIdx, 1>;
using kCtor2 = Id<JT, IdType::OVERLOAD, kNoIdx, 2>;
using kCtor3 = Id<JT, IdType::OVERLOAD, kNoIdx, 3>;
using kCtor4 = Id<JT, IdType::OVERLOAD, kNoIdx, 4>;

static_assert(kCtor0::NumParams() == 0);
static_assert(kCtor1::NumParams() == 1);
static_assert(kCtor2::NumParams() == 2);
static_assert(kCtor3::NumParams() == 1);

static_assert(kCtor0::Name() == std::string_view{"<init>"});
static_assert(kCtor1::Name() == std::string_view{"<init>"});
static_assert(kCtor2::Name() == std::string_view{"<init>"});
static_assert(kCtor3::Name() == std::string_view{"<init>"});

static_assert(0 == Id<JT, IdType::OVERLOAD_PARAM, kNoIdx, 1, 0>::kRank);
static_assert(0 == Id<JT, IdType::OVERLOAD_PARAM, kNoIdx, 2, 0>::kRank);
static_assert(0 == Id<JT, IdType::OVERLOAD_PARAM, kNoIdx, 2, 0>::kRank);
static_assert(1 == Id<JT, IdType::OVERLOAD_PARAM, kNoIdx, 3, 0>::kRank);

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

////////////////////////////////////////////////////////////////////////////////
// Fields (Overload sets with only one overload)
////////////////////////////////////////////////////////////////////////////////
using kField0 = Id<JT, IdType::FIELD, 0>;
using kField1 = Id<JT, IdType::FIELD, 1>;

static_assert(std::string_view{"f0"} == kField0::Name());
static_assert(std::string_view{"f1"} == kField1::Name());

////////////////////////////////////////////////////////////////////////////////
// Statics.
////////////////////////////////////////////////////////////////////////////////
static constexpr Class kStaticClass{
    "kStaticClass",

    Static{
        Method{"sm0", jni::Return<void>{}, Params{}},
        Method{"sm1", jni::Return<jshort>{}, Params<jint>{}},
        Method{"sm2", jni::Return{Class{"kClass2"}},
               Params<jfloat, jboolean>{}},
        Method{
            "sm3",
            Overload{jni::Return<void>{}, Params{}},
            Overload{jni::Return<jint>{}, Params<jboolean>{}},
            Overload{jni::Return<jfloat>{}, Params<jshort, jdouble>{}},
        },
        Method{
            "sm4",
            Overload{jni::Return{Array{jboolean{}}}, Params{Array{jint{}}}},
            Overload{jni::Return{Array{Array{jboolean{}}}},
                     Params{Array{Array{jfloat{}}}}},
            Overload{jni::Return{Array{Array{Array{jboolean{}}}}},
                     Params{Array{Array{Array{jshort{}}}}}},
        },

        Field{"sf0", int{}},
        Field{"sf1", Class{"kClass2"}},
        Field{"sf2", Array{Class{"kClass2"}}},
        Field{"sf3", Array{Array{Class{"kClass2"}}}},
        Field{"sf4", float{}},
    },
};

using JTS = JniType<jobject, kStaticClass>;

////////////////////////////////////////////////////////////////////////////////
// Static Methods.
////////////////////////////////////////////////////////////////////////////////
using kStaticMethod0 = Id<JTS, IdType::STATIC_OVERLOAD_SET, 0>;
using kStaticMethod1 = Id<JTS, IdType::STATIC_OVERLOAD_SET, 1>;
using kStaticMethod2 = Id<JTS, IdType::STATIC_OVERLOAD_SET, 2>;
using kStaticMethod3 = Id<JTS, IdType::STATIC_OVERLOAD_SET, 3>;

static_assert(kStaticMethod0::Name() == std::string_view{"sm0"});
static_assert(kStaticMethod1::Name() == std::string_view{"sm1"});
static_assert(kStaticMethod2::Name() == std::string_view{"sm2"});
static_assert(kStaticMethod3::Name() == std::string_view{"sm3"});

using kStaticMethod0Overload0 = Id<JTS, IdType::STATIC_OVERLOAD, 0, 0>;
using kStaticMethod1Overload0 = Id<JTS, IdType::STATIC_OVERLOAD, 1, 0>;
using kStaticMethod2Overload0 = Id<JTS, IdType::STATIC_OVERLOAD, 2, 0>;
using kStaticMethod3Overload0 = Id<JTS, IdType::STATIC_OVERLOAD, 3, 0>;

static_assert(kStaticMethod0Overload0::NumParams() == 0);
static_assert(kStaticMethod1Overload0::NumParams() == 1);
static_assert(kStaticMethod2Overload0::NumParams() == 2);

static_assert(kStaticMethod0Overload0::Name() == std::string_view{"sm0"});
static_assert(kStaticMethod1Overload0::Name() == std::string_view{"sm1"});
static_assert(kStaticMethod2Overload0::Name() == std::string_view{"sm2"});
static_assert(kStaticMethod3Overload0::Name() == std::string_view{"sm3"});

static_assert(0 == Id<JTS, IdType::STATIC_OVERLOAD_PARAM, 0, 0>::kRank);
static_assert(0 == Id<JTS, IdType::STATIC_OVERLOAD_PARAM, 1, 0>::kRank);
static_assert(0 == Id<JTS, IdType::STATIC_OVERLOAD_PARAM, 1, 0, 0>::kRank);
static_assert(0 == Id<JTS, IdType::STATIC_OVERLOAD_PARAM, 2, 0, 0>::kRank);
static_assert(0 == Id<JTS, IdType::STATIC_OVERLOAD_PARAM, 2, 0, 1>::kRank);

////////////////////////////////////////////////////////////////////////////////
// Static Fields.
////////////////////////////////////////////////////////////////////////////////
using kStaticField0 = Id<JTS, IdType::STATIC_FIELD, 0>;
using kStaticField1 = Id<JTS, IdType::STATIC_FIELD, 1>;
using kStaticField2 = Id<JTS, IdType::STATIC_FIELD, 2>;
using kStaticField3 = Id<JTS, IdType::STATIC_FIELD, 3>;

static_assert(kStaticField0::Name() == std::string_view{"sf0"});
static_assert(kStaticField1::Name() == std::string_view{"sf1"});
static_assert(kStaticField2::Name() == std::string_view{"sf2"});
static_assert(kStaticField3::Name() == std::string_view{"sf3"});

static_assert(kStaticField0::NumParams() == 1);
static_assert(kStaticField1::NumParams() == 1);
static_assert(kStaticField2::NumParams() == 1);

static_assert(kStaticField0::kRank == 0);
static_assert(kStaticField1::kRank == 0);
static_assert(kStaticField2::kRank == 1);
static_assert(kStaticField3::kRank == 2);

}  // namespace
