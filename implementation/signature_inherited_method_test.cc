/*
 * Copyright 2024 Google LLC
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

#include "jni_bind.h"

namespace {

using ::jni::Array;
using ::jni::Class;
using ::jni::Constructor;
using ::jni::Extends;
using ::jni::Id;
using ::jni::IdType;
using ::jni::JniT;
using ::jni::kNoIdx;
using ::jni::Method;
using ::jni::Overload;
using ::jni::Params;
using ::jni::Rank;
using ::jni::Return;
using ::jni::Self;
using ::jni::Signature_v;

static constexpr Class kParent{
    "kParent",
    Constructor{},
    Constructor{jint{}},
    Constructor{jfloat{}, jboolean{}},
    Constructor{Array{jint{}}},
    Constructor{Array{jint{}, Rank<2>{}}},
    Method{"m0", jni::Return<void>{}},
    Method{"m1", jni::Return<jshort>{}, Params<jint>{}},
    Method{"m2", jni::Return{Class{"kClass2"}}, Params<jfloat, jboolean>{}},
    Method{
        "m3",
        Overload{jni::Return<void>{}},
        Overload{jni::Return<jint>{}, Params<jboolean>{}},
        Overload{jni::Return<jfloat>{}, Params<jshort, jdouble>{}},
        Overload{jni::Return<jfloat>{}, Params{Class{"kClass2"}}},
        Overload{jni::Return{Class{"kClass3"}}},
    },
    Method{
        "m4",
        Overload{jni::Return{Array{jboolean{}}}, Params{Array{jint{}}}},
        Overload{jni::Return{Array<jboolean, 2>{}}, Params{Array<jfloat, 2>{}}},
        Overload{jni::Return{Array<jboolean, 3>{}}, Params{Array<jshort, 3>{}}},
        Overload{jni::Return{Array{Class{"kClass2"}, Rank<2>{}}}},
    },
    Method{"m5", jni::Return{Self{}}},
    Method{"m6", jni::Return{}, Params{Self{}}},
    Method{"m7", jni::Return{}, Params{Self{}, Self{}}},
    Method{"m8", jni::Return{Self{}}, Params{Self{}, Self{}}},

    Method{"shadowed_method", jni::Return{int{}}, Params{}},
};

static constexpr Class kChild{
    "kChild",
    Extends{kParent},
    Method{"shadowed_method", jni::Return{jboolean{}}},
};

static constexpr Class kGrandchild{
    "kGrandchild",
    Extends{kChild},
    Method{"shadowed_method", jni::Return{float{}}},
};

using JT_Child = JniT<jobject, kChild>;
using JT_Grandchild = JniT<jobject, kGrandchild>;

//******************************************************************************
// Direct Ancestor (child of parent).
//******************************************************************************

////////////////////////////////////////////////////////////////////////////////
// Constructors.
////////////////////////////////////////////////////////////////////////////////
static_assert(
    std::string_view{"I"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, kNoIdx, 1, 0, 1>>);
static_assert(
    std::string_view{"F"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, kNoIdx, 2, 0, 1>>);
static_assert(
    std::string_view{"Z"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, kNoIdx, 2, 1, 1>>);
static_assert(
    std::string_view{"[I"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, kNoIdx, 3, 0, 1>>);

static_assert(
    std::string_view{"()V"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD, kNoIdx, 0, kNoIdx, 1>>);
static_assert(
    std::string_view{"(I)V"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD, kNoIdx, 1, kNoIdx, 1>>);
static_assert(
    std::string_view{"(FZ)V"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD, kNoIdx, 2, kNoIdx, 1>>);
static_assert(
    std::string_view{"([I)V"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD, kNoIdx, 3, kNoIdx, 1>>);
static_assert(
    std::string_view{"([[I)V"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD, kNoIdx, 4, kNoIdx, 1>>);

////////////////////////////////////////////////////////////////////////////////
// Methods (Overload sets with only one overload).
////////////////////////////////////////////////////////////////////////////////
static_assert(
    std::string_view{"S"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, 1, 0, kNoIdx, 1>>);
static_assert(std::string_view{"I"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, 1, 0, 0, 1>>);
static_assert(
    std::string_view{"LkClass2;"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, 2, 0, kNoIdx, 1>>);
static_assert(std::string_view{"F"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, 2, 0, 0, 1>>);
static_assert(std::string_view{"Z"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, 2, 0, 1, 1>>);

static_assert(
    std::string_view{"V"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, 0, 0, kNoIdx, 1>>);
static_assert(
    std::string_view{"S"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, 1, 0, kNoIdx, 1>>);
static_assert(std::string_view{"I"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, 1, 0, 0, 1>>);
static_assert(
    std::string_view{"LkClass2;"} ==
    Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, 2, 0, kNoIdx, 1>>);
static_assert(std::string_view{"F"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, 2, 0, 0, 1>>);
static_assert(std::string_view{"Z"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD_PARAM, 2, 0, 1, 1>>);

static_assert(std::string_view{"()V"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD, 0, 0, kNoIdx, 1>>);
static_assert(std::string_view{"(I)S"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD, 1, 0, kNoIdx, 1>>);
static_assert(std::string_view{"(FZ)LkClass2;"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD, 2, 0, kNoIdx, 1>>);

////////////////////////////////////////////////////////////////////////////////
// Overloaded Method (smoke test, they should behave the same).
////////////////////////////////////////////////////////////////////////////////
static_assert(std::string_view{"()V"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD, 3, 0, kNoIdx, 1>>);
static_assert(std::string_view{"(Z)I"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD, 3, 1, kNoIdx, 1>>);
static_assert(std::string_view{"(SD)F"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD, 3, 2, kNoIdx, 1>>);
static_assert(std::string_view{"(LkClass2;)F"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD, 3, 3, kNoIdx, 1>>);
static_assert(std::string_view{"()LkClass3;"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD, 3, 4, kNoIdx, 1>>);

static_assert(std::string_view{"([I)[Z"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD, 4, 0, kNoIdx, 1>>);
static_assert(std::string_view{"([[F)[[Z"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD, 4, 1, kNoIdx, 1>>);
static_assert(std::string_view{"([[[S)[[[Z"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD, 4, 2, kNoIdx, 1>>);
static_assert(std::string_view{"()[[LkClass2;"} ==
              Signature_v<Id<JT_Child, IdType::OVERLOAD, 4, 3, kNoIdx, 1>>);

////////////////////////////////////////////////////////////////////////////////
// Self Referencing Method (gives richly decorated self back).
////////////////////////////////////////////////////////////////////////////////
using kMethod5 = Id<JT_Child, IdType::OVERLOAD, 5, 0, kNoIdx, 1>;
static_assert(std::string_view{"()LkParent;"} == Signature_v<kMethod5>.data());

using kMethod6 = Id<JT_Child, IdType::OVERLOAD, 6, 0, kNoIdx, 1>;
static_assert(std::string_view{"(LkParent;)V"} == Signature_v<kMethod6>.data());

using kMethod7 = Id<JT_Child, IdType::OVERLOAD, 7, 0, kNoIdx, 1>;
static_assert(std::string_view{"(LkParent;LkParent;)V"} ==
              Signature_v<kMethod7>.data());

using kMethod8 = Id<JT_Child, IdType::OVERLOAD, 8, 0, kNoIdx, 1>;
static_assert(std::string_view{"(LkParent;LkParent;)LkParent;"} ==
              Signature_v<kMethod8>.data());

////////////////////////////////////////////////////////////////////////////////
// Shadowed Method.
////////////////////////////////////////////////////////////////////////////////
using kMethod9_ancestor = Id<JT_Child, IdType::OVERLOAD, 9, 0, kNoIdx, 1>;
static_assert(std::string_view{"()I"} == Signature_v<kMethod9_ancestor>.data());

using kMethod9 = Id<JT_Child, IdType::OVERLOAD, 0, 0, kNoIdx, 0>;
static_assert(std::string_view{"()Z"} == Signature_v<kMethod9>.data());

//******************************************************************************
// Second Ancestor (child of child of parent).
//******************************************************************************

////////////////////////////////////////////////////////////////////////////////
// Constructors.
////////////////////////////////////////////////////////////////////////////////
static_assert(
    std::string_view{"I"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, kNoIdx, 1, 0, 2>>);
static_assert(
    std::string_view{"F"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, kNoIdx, 2, 0, 2>>);
static_assert(
    std::string_view{"Z"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, kNoIdx, 2, 1, 2>>);
static_assert(
    std::string_view{"[I"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, kNoIdx, 3, 0, 2>>);

static_assert(
    std::string_view{"()V"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, kNoIdx, 0, kNoIdx, 2>>);
static_assert(
    std::string_view{"(I)V"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, kNoIdx, 1, kNoIdx, 2>>);
static_assert(
    std::string_view{"(FZ)V"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, kNoIdx, 2, kNoIdx, 2>>);
static_assert(
    std::string_view{"([I)V"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, kNoIdx, 3, kNoIdx, 2>>);
static_assert(
    std::string_view{"([[I)V"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, kNoIdx, 4, kNoIdx, 2>>);

////////////////////////////////////////////////////////////////////////////////
// Methods (Overload sets with only one overload).
////////////////////////////////////////////////////////////////////////////////
static_assert(
    std::string_view{"S"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, 1, 0, kNoIdx, 2>>);
static_assert(
    std::string_view{"I"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, 1, 0, 0, 2>>);
static_assert(
    std::string_view{"LkClass2;"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, 2, 0, kNoIdx, 2>>);
static_assert(
    std::string_view{"F"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, 2, 0, 0, 2>>);
static_assert(
    std::string_view{"Z"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, 2, 0, 1, 2>>);

static_assert(
    std::string_view{"V"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, 0, 0, kNoIdx, 2>>);
static_assert(
    std::string_view{"S"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, 1, 0, kNoIdx, 2>>);
static_assert(
    std::string_view{"I"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, 1, 0, 0, 2>>);
static_assert(
    std::string_view{"LkClass2;"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, 2, 0, kNoIdx, 2>>);
static_assert(
    std::string_view{"F"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, 2, 0, 0, 2>>);
static_assert(
    std::string_view{"Z"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD_PARAM, 2, 0, 1, 2>>);

static_assert(
    std::string_view{"()V"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, 0, 0, kNoIdx, 2>>);
static_assert(
    std::string_view{"(I)S"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, 1, 0, kNoIdx, 2>>);
static_assert(
    std::string_view{"(FZ)LkClass2;"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, 2, 0, kNoIdx, 2>>);

////////////////////////////////////////////////////////////////////////////////
// Overloaded Method (smoke test, they should behave the same).
////////////////////////////////////////////////////////////////////////////////
static_assert(
    std::string_view{"()V"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, 3, 0, kNoIdx, 2>>);
static_assert(
    std::string_view{"(Z)I"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, 3, 1, kNoIdx, 2>>);
static_assert(
    std::string_view{"(SD)F"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, 3, 2, kNoIdx, 2>>);
static_assert(
    std::string_view{"(LkClass2;)F"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, 3, 3, kNoIdx, 2>>);
static_assert(
    std::string_view{"()LkClass3;"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, 3, 4, kNoIdx, 2>>);

static_assert(
    std::string_view{"([I)[Z"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, 4, 0, kNoIdx, 2>>);
static_assert(
    std::string_view{"([[F)[[Z"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, 4, 1, kNoIdx, 2>>);
static_assert(
    std::string_view{"([[[S)[[[Z"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, 4, 2, kNoIdx, 2>>);
static_assert(
    std::string_view{"()[[LkClass2;"} ==
    Signature_v<Id<JT_Grandchild, IdType::OVERLOAD, 4, 3, kNoIdx, 2>>);

////////////////////////////////////////////////////////////////////////////////
// Self Referencing Method (gives richly decorated self back).
////////////////////////////////////////////////////////////////////////////////
using kGrandchildMethod5 = Id<JT_Grandchild, IdType::OVERLOAD, 5, 0, kNoIdx, 2>;
static_assert(std::string_view{"()LkParent;"} ==
              Signature_v<kGrandchildMethod5>.data());

using kGrandchildMethod6 = Id<JT_Grandchild, IdType::OVERLOAD, 6, 0, kNoIdx, 2>;
static_assert(std::string_view{"(LkParent;)V"} ==
              Signature_v<kGrandchildMethod6>.data());

using kGrandchildMethod7 = Id<JT_Grandchild, IdType::OVERLOAD, 7, 0, kNoIdx, 2>;
static_assert(std::string_view{"(LkParent;LkParent;)V"} ==
              Signature_v<kGrandchildMethod7>.data());

using kGrandchildMethod8 = Id<JT_Grandchild, IdType::OVERLOAD, 8, 0, kNoIdx, 2>;
static_assert(std::string_view{"(LkParent;LkParent;)LkParent;"} ==
              Signature_v<kGrandchildMethod8>.data());

////////////////////////////////////////////////////////////////////////////////
// Shadowed Method.
////////////////////////////////////////////////////////////////////////////////
using kGrandchildMethod9_GrandParent =
    Id<JT_Grandchild, IdType::OVERLOAD, 9, 0, kNoIdx, 2>;
static_assert(std::string_view{"()I"} ==
              Signature_v<kGrandchildMethod9_GrandParent>.data());

using kGrandchildMethod9_Parent =
    Id<JT_Grandchild, IdType::OVERLOAD, 0, 0, kNoIdx, 1>;
static_assert(std::string_view{"()Z"} ==
              Signature_v<kGrandchildMethod9_Parent>.data());

using kGrandchildMethod9_Self =
    Id<JT_Grandchild, IdType::OVERLOAD, 0, 0, kNoIdx, 0>;
static_assert(std::string_view{"()F"} ==
              Signature_v<kGrandchildMethod9_Self>.data());

}  // namespace
