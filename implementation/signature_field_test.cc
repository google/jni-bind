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
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"

namespace {

using ::jni::Array;
using ::jni::Class;
using ::jni::Field;
using ::jni::Id;
using ::jni::IdType;
using ::jni::JniT;
using ::jni::JniTSelector;
using ::jni::kNoIdx;
using ::jni::Rank;
using ::jni::SelectorStaticInfo;
using ::jni::Self;
using ::jni::Signature_v;

// clang-format off
static constexpr Class kClass1{
  "kClass1",

   // Rank 0.
   Field{"f0", jboolean{}},
   Field{"f1", jbyte{}},
   Field{"f2", jchar{}},
   Field{"f3", jshort{}},
   Field{"f4", jint{}},
   Field{"f5", jfloat{}},
   Field{"f6", jdouble{}},
   Field{"f7", jlong{}},
   Field{"f8", Class{"kClass2"}},

   // Rank 1.
   Field{"f9", Array<jboolean>{}},
   Field{"f10", Array<jbyte>{}},
   Field{"f11", Array<jchar>{}},
   Field{"f12", Array<jshort>{}},
   Field{"f13", Array<jint>{}},
   Field{"f14", Array<jfloat>{}},
   Field{"f15", Array<jdouble>{}},
   Field{"f16", Array<jlong>{}},
   Field{"f17", Array{Class{"kClass2"}}},

   // Rank 2.
   Field{"f18", Array{jboolean{}, Rank<2>{}}},
   Field{"f19", Array{jbyte{}, Rank<2>{}}},
   Field{"f20", Array{jchar{}, Rank<2>{}}},
   Field{"f21", Array{jshort{}, Rank<2>{}}},
   Field{"f22", Array{jint{}, Rank<2>{}}},
   Field{"f23", Array{jfloat{}, Rank<2>{}}},
   Field{"f24", Array{jdouble{}, Rank<2>{}}},
   Field{"f25", Array{jlong{}, Rank<2>{}}},
   Field{"f26", Array{Class{"kClass2"}, Rank<2>{}}},

   // Self.
  Field{"f27", Self{}}
};
// clang-format on

using JT = JniT<jobject, kClass1>;

////////////////////////////////////////////////////////////////////////////////
// Rank 0.
////////////////////////////////////////////////////////////////////////////////
static_assert(std::string_view{"Z"} == Signature_v<Id<JT, IdType::FIELD, 0>>);
static_assert(std::string_view{"B"} == Signature_v<Id<JT, IdType::FIELD, 1>>);
static_assert(std::string_view{"C"} == Signature_v<Id<JT, IdType::FIELD, 2>>);
static_assert(std::string_view{"S"} == Signature_v<Id<JT, IdType::FIELD, 3>>);
static_assert(std::string_view{"I"} == Signature_v<Id<JT, IdType::FIELD, 4>>);
static_assert(std::string_view{"F"} == Signature_v<Id<JT, IdType::FIELD, 5>>);
static_assert(std::string_view{"D"} == Signature_v<Id<JT, IdType::FIELD, 6>>);
static_assert(std::string_view{"J"} == Signature_v<Id<JT, IdType::FIELD, 7>>);
static_assert(std::string_view{"LkClass2;"} ==
              Signature_v<Id<JT, IdType::FIELD, 8>>);

////////////////////////////////////////////////////////////////////////////////
// Rank 1.
////////////////////////////////////////////////////////////////////////////////
static_assert(std::string_view{"[Z"} == Signature_v<Id<JT, IdType::FIELD, 9>>);
static_assert(std::string_view{"[B"} == Signature_v<Id<JT, IdType::FIELD, 10>>);
static_assert(std::string_view{"[C"} == Signature_v<Id<JT, IdType::FIELD, 11>>);
static_assert(std::string_view{"[S"} == Signature_v<Id<JT, IdType::FIELD, 12>>);
static_assert(std::string_view{"[I"} == Signature_v<Id<JT, IdType::FIELD, 13>>);
static_assert(std::string_view{"[F"} == Signature_v<Id<JT, IdType::FIELD, 14>>);
static_assert(std::string_view{"[D"} == Signature_v<Id<JT, IdType::FIELD, 15>>);
static_assert(std::string_view{"[J"} == Signature_v<Id<JT, IdType::FIELD, 16>>);
static_assert(std::string_view{"[LkClass2;"} ==
              Signature_v<Id<JT, IdType::FIELD, 17>>);

////////////////////////////////////////////////////////////////////////////////
// Rank 2.
////////////////////////////////////////////////////////////////////////////////
static_assert(std::string_view{"[[Z"} ==
              Signature_v<Id<JT, IdType::FIELD, 18>>);
static_assert(std::string_view{"[[B"} ==
              Signature_v<Id<JT, IdType::FIELD, 19>>);
static_assert(std::string_view{"[[C"} ==
              Signature_v<Id<JT, IdType::FIELD, 20>>);
static_assert(std::string_view{"[[S"} ==
              Signature_v<Id<JT, IdType::FIELD, 21>>);
static_assert(std::string_view{"[[I"} ==
              Signature_v<Id<JT, IdType::FIELD, 22>>);
static_assert(std::string_view{"[[F"} ==
              Signature_v<Id<JT, IdType::FIELD, 23>>);
static_assert(std::string_view{"[[D"} ==
              Signature_v<Id<JT, IdType::FIELD, 24>>);
static_assert(std::string_view{"[[J"} ==
              Signature_v<Id<JT, IdType::FIELD, 25>>);
static_assert(std::string_view{"[[LkClass2;"} ==
              Signature_v<Id<JT, IdType::FIELD, 26>>);

////////////////////////////////////////////////////////////////////////////////
// Self.
////////////////////////////////////////////////////////////////////////////////
static_assert(std::string_view{"LkClass1;"} ==
              Signature_v<Id<JT, IdType::FIELD, 27>>);

}  // namespace
