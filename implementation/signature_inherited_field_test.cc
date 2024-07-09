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
#include "implementation/id_type.h"
#include "jni_bind.h"

using ::jni::Array;
using ::jni::Class;
using ::jni::Extends;
using ::jni::Field;
using ::jni::Id;
using ::jni::IdType;
using ::jni::JniT;
using ::jni::kNoIdx;
using ::jni::Rank;
using ::jni::Self;
using ::jni::Signature_v;

// clang-format off
static constexpr Class kParent{
  "kParent",

   // Rank 0.
   Field{"f0", jboolean{}},
   Field{"f1", jbyte{}},
   Field{"f2", jchar{}},
   Field{"f3", jshort{}},
   Field{"f4", jint{}},
   Field{"f5", jfloat{}},
   Field{"f6", jdouble{}},
   Field{"f7", jlong{}},
   Field{"f8", Class{"kGrandchild"}},

   // Rank 1.
   Field{"f9", Array<jboolean>{}},
   Field{"f10", Array<jbyte>{}},
   Field{"f11", Array<jchar>{}},
   Field{"f12", Array<jshort>{}},
   Field{"f13", Array<jint>{}},
   Field{"f14", Array<jfloat>{}},
   Field{"f15", Array<jdouble>{}},
   Field{"f16", Array<jlong>{}},
   Field{"f17", Array{Class{"kGrandchild"}}},

   // Rank 2.
   Field{"f18", Array{jboolean{}, Rank<2>{}}},
   Field{"f19", Array{jbyte{}, Rank<2>{}}},
   Field{"f20", Array{jchar{}, Rank<2>{}}},
   Field{"f21", Array{jshort{}, Rank<2>{}}},
   Field{"f22", Array{jint{}, Rank<2>{}}},
   Field{"f23", Array{jfloat{}, Rank<2>{}}},
   Field{"f24", Array{jdouble{}, Rank<2>{}}},
   Field{"f25", Array{jlong{}, Rank<2>{}}},
   Field{"f26", Array{Class{"kGrandchild"}, Rank<2>{}}},

  // Shadowed Type.
  Field{"f27", jboolean{}},

  // Shadowed Self Type.
  Field{"f28", Self{}}
};

static constexpr Class kChild{
  "kChild",
  Extends{kParent},
  Field{"f27", jint{}},
  Field{"f28", Self{}}
};

static constexpr Class kGrandchild{
  "kGrandchild",
  Extends{kChild},
  Field{"f27", Class{"SomeClazz"}},
  Field{"f28", Self{}}
};
// clang-format on

using JTChild = JniT<jobject, kChild>;
using JTGrandChild = JniT<jobject, kGrandchild>;

////////////////////////////////////////////////////////////////////////////////
// Direct ancestor: Rank 0.
////////////////////////////////////////////////////////////////////////////////
static_assert(std::string_view{"Z"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 0, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"B"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 1, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"C"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 2, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"S"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 3, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"I"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 4, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"F"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 5, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"D"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 6, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"J"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 7, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"LkGrandchild;"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 8, kNoIdx, kNoIdx, 1>>);

////////////////////////////////////////////////////////////////////////////////
// Direct ancestor: Rank 1.
////////////////////////////////////////////////////////////////////////////////
static_assert(std::string_view{"[Z"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 9, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[B"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 10, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[C"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 11, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[S"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 12, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[I"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 13, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[F"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 14, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[D"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 15, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[J"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 16, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[LkGrandchild;"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 17, kNoIdx, kNoIdx, 1>>);

////////////////////////////////////////////////////////////////////////////////
// Direct ancestor: Rank 2.
////////////////////////////////////////////////////////////////////////////////
static_assert(std::string_view{"[[Z"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 18, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[[B"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 19, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[[C"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 20, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[[S"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 21, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[[I"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 22, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[[F"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 23, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[[D"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 24, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[[J"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 25, kNoIdx, kNoIdx, 1>>);
static_assert(std::string_view{"[[LkGrandchild;"} ==
              Signature_v<Id<JTChild, IdType::FIELD, 26, kNoIdx, kNoIdx, 1>>);

//******************************************************************************
// Second Ancestor (child of child of parent).
//******************************************************************************

////////////////////////////////////////////////////////////////////////////////
// Rank 0.
////////////////////////////////////////////////////////////////////////////////
static_assert(
    std::string_view{"Z"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 0, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"B"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 1, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"C"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 2, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"S"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 3, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"I"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 4, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"F"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 5, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"D"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 6, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"J"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 7, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"LkGrandchild;"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 8, kNoIdx, kNoIdx, 2>>);

////////////////////////////////////////////////////////////////////////////////
// Rank 1.
////////////////////////////////////////////////////////////////////////////////
static_assert(
    std::string_view{"[Z"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 9, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[B"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 10, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[C"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 11, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[S"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 12, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[I"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 13, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[F"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 14, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[D"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 15, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[J"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 16, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[LkGrandchild;"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 17, kNoIdx, kNoIdx, 2>>);

////////////////////////////////////////////////////////////////////////////////
// Rank 2.
////////////////////////////////////////////////////////////////////////////////
static_assert(
    std::string_view{"[[Z"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 18, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[[B"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 19, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[[C"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 20, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[[S"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 21, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[[I"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 22, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[[F"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 23, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[[D"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 24, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[[J"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 25, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"[[LkGrandchild;"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 26, kNoIdx, kNoIdx, 2>>);

//******************************************************************************
// Shadowed Values (all levels of ancestry).
//
// Note: Just because the field is shadowing doesn't mean its primary index
// will be consistent across different locii of inheritance.
//******************************************************************************
static_assert(
    std::string_view{"Z"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 27, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"I"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 0, kNoIdx, kNoIdx, 1>>);
static_assert(
    std::string_view{"LSomeClazz;"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 0, kNoIdx, kNoIdx, 0>>);

//******************************************************************************
// Shadowed Values of `Self`.
//******************************************************************************
static_assert(
    std::string_view{"LkParent;"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 28, kNoIdx, kNoIdx, 2>>);
static_assert(
    std::string_view{"LkChild;"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 1, kNoIdx, kNoIdx, 1>>);
static_assert(
    std::string_view{"LkGrandchild;"} ==
    Signature_v<Id<JTGrandChild, IdType::FIELD, 1, kNoIdx, kNoIdx, 0>>);
