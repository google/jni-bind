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
using ::jni::ClassLoader;
using ::jni::Constructor;
using ::jni::Field;
using ::jni::Id;
using ::jni::IdType;
using ::jni::JniT;
using ::jni::kDefaultClassLoader;
using ::jni::kNoIdx;
using ::jni::LoadedBy;
using ::jni::Method;
using ::jni::Overload;
using ::jni::Params;
using ::jni::Rank;
using ::jni::Return;
using ::jni::Static;
using ::jni::SupportedClassSet;

static constexpr Class kOtherClass{"kOtherClass"};
static constexpr ClassLoader kClassLoader{kDefaultClassLoader,
                                          SupportedClassSet{kOtherClass}};

static constexpr Class kClass1{
    "com/google/kClass1",
    Constructor{},
    Constructor{jint{}},
    Constructor{jfloat{}, jboolean{}},
    Constructor{Array{jint{}}},

    Method{"m0", jni::Return<void>{}},
    Method{"m1", jni::Return<jshort>{}, Params<jint>{}},
    Method{"m2", jni::Return{Class{"kClass2"}}, Params<jfloat, jboolean>{}},
    Method{
        "m3",
        Overload{jni::Return<void>{}},
        Overload{jni::Return<jint>{}, Params<jboolean>{}},
        Overload{jni::Return<jfloat>{}, Params<jshort, jdouble>{}},
        Overload{jni::Return{Array{Class{"kClass2"}}},
                 Params<jshort, jdouble>{}},
    },
    Method{
        "m4",
        Overload{jni::Return{Array{jboolean{}}}, Params{Array{jint{}}}},
        Overload{jni::Return{Array<jboolean, 2>{}}, Params{Array<jfloat, 2>{}}},
        Overload{jni::Return{Array<jboolean, 3>{}}, Params{Array<jshort, 3>{}}},
    },
    Method{"m5", jni::Return{Array{Class{"kClass2"}}}},
    Method{"m6", jni::Return{Array{Class{"kClass2"}, Rank<2>{}}}},
    Method{"m7", jni::Return{LoadedBy{kClassLoader, kOtherClass}}},

    Field{"f0", int{}},
    Field{"f1", Class{"kClass2"}}};

using JT = JniT<jobject, kClass1>;

////////////////////////////////////////////////////////////////////////////////
// Class.
////////////////////////////////////////////////////////////////////////////////
using ClassIdT = Id<JT, IdType::CLASS>;

static_assert(ClassIdT::Name() == std::string_view{"com/google/kClass1"});
static_assert(ClassIdT::kNameUsingDots ==
              std::string_view{"com.google.kClass1"});

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
using kMethod4 = Id<JT, IdType::OVERLOAD_SET, 4>;
using kMethod5 = Id<JT, IdType::OVERLOAD_SET, 5>;
using kMethod6 = Id<JT, IdType::OVERLOAD_SET, 6>;
using kMethod7 = Id<JT, IdType::OVERLOAD_SET, 7>;

static_assert(kMethod0::Name() == std::string_view{"m0"});
static_assert(kMethod1::Name() == std::string_view{"m1"});
static_assert(kMethod2::Name() == std::string_view{"m2"});
static_assert(kMethod3::Name() == std::string_view{"m3"});
static_assert(kMethod4::Name() == std::string_view{"m4"});
static_assert(kMethod5::Name() == std::string_view{"m5"});
static_assert(kMethod6::Name() == std::string_view{"m6"});
static_assert(kMethod7::Name() == std::string_view{"m7"});

using kMethod0Overload0 = Id<JT, IdType::OVERLOAD, 0, 0>;
using kMethod1Overload0 = Id<JT, IdType::OVERLOAD, 1, 0>;
using kMethod2Overload0 = Id<JT, IdType::OVERLOAD, 2, 0>;
using kMethod3Overload0 = Id<JT, IdType::OVERLOAD, 3, 0>;
using kMethod5Overload0 = Id<JT, IdType::OVERLOAD, 5, 0>;
using kMethod6Overload0 = Id<JT, IdType::OVERLOAD, 6, 0>;
using kMethod7Overload0 = Id<JT, IdType::OVERLOAD, 7, 0>;

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

using kMethod5Overload0Return = Id<JT, IdType::OVERLOAD_PARAM, 5, 0>;
using kMethod5Overload0Param0 = Id<JT, IdType::OVERLOAD_PARAM, 5, 0>;
using kMethod6Overload0Return = Id<JT, IdType::OVERLOAD_PARAM, 6, 0>;
using kMethod6Overload0Param0 = Id<JT, IdType::OVERLOAD_PARAM, 6, 0>;

static_assert(std::string_view{"m5"} == kMethod5Overload0::Name());
static_assert(std::string_view{"m6"} == kMethod6Overload0::Name());

static_assert(1 == kMethod5Overload0Return::kRank);
static_assert(2 == kMethod6Overload0Return::kRank);

using kMethod7Overload0Return = Id<JT, IdType::OVERLOAD_PARAM, 7, 0>;

static_assert(std::string_view{"m7"} == kMethod7Overload0::Name());

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
        Method{"sm0", jni::Return<void>{}},
        Method{"sm1", jni::Return<jshort>{}, Params<jint>{}},
        Method{"sm2", jni::Return{Class{"kClass2"}},
               Params<jfloat, jboolean>{}},
        Method{
            "sm3",
            Overload{jni::Return<void>{}},
            Overload{jni::Return<jint>{}, Params<jboolean>{}},
            Overload{jni::Return<jfloat>{}, Params<jshort, jdouble>{}},
        },
        Method{
            "sm4",
            Overload{jni::Return{Array<jboolean>{}}, Params{Array<jint>{}}},
            Overload{jni::Return{Array<jboolean, 2>{}},
                     Params{Array<jfloat, 2>{}}},
            Overload{jni::Return{Array<jboolean, 3>{}},
                     Params{Array<jshort, 3>{}}},
        },

        Field{"sf0", int{}},
        Field{"sf1", Class{"kClass2"}},
        Field{"sf2", Array{Class{"kClass2"}}},
        Field{"sf3", Array{Class{"kClass2"}, Rank<2>{}}},
        Field{"sf4", float{}},
    },
};

using JTS = JniT<jobject, kStaticClass>;

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

static_assert(kStaticMethod0Overload0::kIsStatic);
static_assert(kStaticMethod1Overload0::kIsStatic);
static_assert(kStaticMethod2Overload0::kIsStatic);
static_assert(kStaticMethod3Overload0::kIsStatic);

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

static_assert(kStaticField0::kIsStatic);
static_assert(kStaticField1::kIsStatic);
static_assert(kStaticField2::kIsStatic);

static_assert(kStaticField0::kRank == 0);
static_assert(kStaticField1::kRank == 0);
static_assert(kStaticField2::kRank == 1);
static_assert(kStaticField3::kRank == 2);

}  // namespace
