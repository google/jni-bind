/*
 * Copyright 2021 Google LLC
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
using ::jni::Constructor;
using ::jni::GlobalObject;
using ::jni::Id;
using ::jni::IdType;
using ::jni::JniT;
using ::jni::kNoIdx;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::MethodSelection;
using ::jni::Overload;
using ::jni::Params;
using ::jni::Rank;
using ::jni::Return;

static constexpr Class kClass1{"class1", Method{"Foo", Return{}}};
static constexpr Class kClass1NoDefinition{"class1"};
static constexpr Class kClass2{"class2"};
static constexpr Class kUnusedClass{"invalidClass"};

using LocalObj1 = LocalObject<kClass1>;
using LocalObj1NoDefinition = LocalObject<kClass1NoDefinition>;
using LocalObj2 = LocalObject<kClass2>;
using LocalInvalidObj = LocalObject<kUnusedClass>;

using GlobalObj1 = GlobalObject<kClass1>;
using GlobalObj2 = GlobalObject<kClass2>;
using GlobalInvalidObj = GlobalObject<kUnusedClass>;

////////////////////////////////////////////////////////////////////////////////
// Single argument to method.
////////////////////////////////////////////////////////////////////////////////
constexpr Class c1{
    "c1",
    Method{"0", Return<void>{}},
    Method{"1", Return<void>{}, Params<jboolean>{}},
    Method{"2", Return<void>{}, Params<jchar>{}},
    Method{"3", Return<void>{}, Params<jshort>{}},
    Method{"4", Return<void>{}, Params<jint>{}},
    Method{"5", Return<void>{}, Params<jfloat>{}},
    Method{"6", Return<void>{}, Params<jdouble>{}},
    Method{"7", Return<void>{}, Params{jstring{}}},
    Method{"8", Return<void>{}, Params{kClass1}},
    Method{"9", Return<void>{}, Params{Array<int>{}}},
    Method{"10", Return<void>{}, Params{Array<int, 2>{}}},
    Method{"11", Return<void>{}, Params{Array<float, 3>{}}},
    Method{"12", Return<void>{}, Params{Array{kClass1}}},
    Method{"13", Return<void>{}, Params{Array{kClass1, Rank<2>{}}}},
    Method{"14", Return<void>{}, Params{Array{kClass1, Rank<3>{}}}}};

template <const auto& class_v, size_t method_idx>
using MthdId_t = MethodSelection<
    Id<JniT<jobject, class_v>, IdType::OVERLOAD_SET, method_idx>,
    IdType::OVERLOAD, IdType::OVERLOAD_PARAM>;

static_assert(MthdId_t<c1, 0>::ArgSetViable<>());
static_assert(!MthdId_t<c1, 0>::ArgSetViable<int>());
static_assert(!MthdId_t<c1, 0>::ArgSetViable<int, float>());
static_assert(!MthdId_t<c1, 0>::ArgSetViable<LocalObj1>());

static_assert(MthdId_t<c1, 1>::ArgSetViable<jboolean>());
static_assert(MthdId_t<c1, 1>::ArgSetViable<bool>());

static_assert(MthdId_t<c1, 2>::ArgSetViable<jchar>());
static_assert(MthdId_t<c1, 2>::ArgSetViable<char>());

static_assert(MthdId_t<c1, 3>::ArgSetViable<jshort>());
static_assert(MthdId_t<c1, 3>::ArgSetViable<short>());

static_assert(MthdId_t<c1, 4>::ArgSetViable<int>());
static_assert(!MthdId_t<c1, 4>::ArgSetViable<float>());
static_assert(!MthdId_t<c1, 4>::ArgSetViable<double>());
static_assert(!MthdId_t<c1, 4>::ArgSetViable<LocalObj1>());

static_assert(MthdId_t<c1, 5>::ArgSetViable<float>());
static_assert(!MthdId_t<c1, 5>::ArgSetViable<int>());
static_assert(!MthdId_t<c1, 5>::ArgSetViable<double>());
static_assert(!MthdId_t<c1, 5>::ArgSetViable<LocalObj1>());
static_assert(!MthdId_t<c1, 5>::ArgSetViable<float, float>());

static_assert(MthdId_t<c1, 6>::ArgSetViable<jdouble>());
static_assert(!MthdId_t<c1, 6>::ArgSetViable<int>());
static_assert(!MthdId_t<c1, 6>::ArgSetViable<float>());
static_assert(!MthdId_t<c1, 6>::ArgSetViable<LocalObj1>());
static_assert(!MthdId_t<c1, 6>::ArgSetViable<float, float>());

static_assert(MthdId_t<c1, 7>::ArgSetViable<std::string>());
static_assert(MthdId_t<c1, 7>::ArgSetViable<jstring>());
static_assert(MthdId_t<c1, 7>::ArgSetViable<const char*>());
static_assert(MthdId_t<c1, 7>::ArgSetViable<char[10]>());
static_assert(MthdId_t<c1, 7>::ArgSetViable<std::string_view>());
static_assert(MthdId_t<c1, 7>::ArgSetViable<const char (&)[18]>());

static_assert(MthdId_t<c1, 8>::ArgSetViable<jobject>());
static_assert(MthdId_t<c1, 8>::ArgSetViable<LocalObj1>());
static_assert(MthdId_t<c1, 8>::ArgSetViable<LocalObj1&>());
static_assert(MthdId_t<c1, 8>::ArgSetViable<LocalObj1&&>());
static_assert(MthdId_t<c1, 8>::ArgSetViable<LocalObj1NoDefinition>());
static_assert(!MthdId_t<c1, 8>::ArgSetViable<LocalObj2>());
static_assert(MthdId_t<c1, 8>::ArgSetViable<LocalObj1>());
static_assert(MthdId_t<c1, 8>::ArgSetViable<GlobalObj1>());
static_assert(!MthdId_t<c1, 8>::ArgSetViable<int>());
static_assert(!MthdId_t<c1, 8>::ArgSetViable<double>());
static_assert(!MthdId_t<c1, 8>::ArgSetViable<LocalObj2>());
static_assert(!MthdId_t<c1, 8>::ArgSetViable<GlobalObj2>());
static_assert(!MthdId_t<c1, 8>::ArgSetViable<LocalObj1, LocalObj1>());

// Rank 1 Arrays.
static_assert(MthdId_t<c1, 9>::ArgSetViable<jintArray>());
static_assert(MthdId_t<c1, 9>::ArgSetViable<LocalArray<jint>>());
static_assert(!MthdId_t<c1, 9>::ArgSetViable<jfloatArray>());

// Rank 2 Arrays.
static_assert(MthdId_t<c1, 10>::ArgSetViable<jobjectArray>());
static_assert(MthdId_t<c1, 10>::ArgSetViable<LocalArray<jint, 2>>());
static_assert(!MthdId_t<c1, 10>::ArgSetViable<jintArray>());
static_assert(!MthdId_t<c1, 10>::ArgSetViable<jfloatArray>());

// Rank 3 Arrays.
static_assert(MthdId_t<c1, 11>::ArgSetViable<jobjectArray>());
static_assert(MthdId_t<c1, 11>::ArgSetViable<LocalArray<jfloat, 3>>());
static_assert(!MthdId_t<c1, 11>::ArgSetViable<jintArray>());
static_assert(!MthdId_t<c1, 11>::ArgSetViable<LocalArray<jfloatArray, 2>>());
static_assert(!MthdId_t<c1, 11>::ArgSetViable<LocalArray<jfloatArray, 4>>());
static_assert(!MthdId_t<c1, 10>::ArgSetViable<jfloatArray>());

// Rank 1 Arrays of Objects.
static_assert(MthdId_t<c1, 12>::ArgSetViable<jobjectArray>());
static_assert(
    MthdId_t<c1, 12>::ArgSetViable<LocalArray<jobject, 1, kClass1>>());
static_assert(
    !MthdId_t<c1, 12>::ArgSetViable<LocalArray<jobject, 2, kClass1>>());
static_assert(!MthdId_t<c1, 12>::ArgSetViable<jint>());
static_assert(!MthdId_t<c1, 12>::ArgSetViable<jintArray>());

// Rank 2 Arrays of Objects.
static_assert(MthdId_t<c1, 13>::ArgSetViable<jobjectArray>());
static_assert(
    MthdId_t<c1, 13>::ArgSetViable<LocalArray<jobject, 2, kClass1>>());
static_assert(
    !MthdId_t<c1, 13>::ArgSetViable<LocalArray<jobject, 1, kClass1>>());
static_assert(
    !MthdId_t<c1, 13>::ArgSetViable<LocalArray<jobject, 3, kClass1>>());
static_assert(!MthdId_t<c1, 13>::ArgSetViable<jint>());
static_assert(!MthdId_t<c1, 13>::ArgSetViable<jintArray>());

// Rank 3 Arrays of Objects.
static_assert(MthdId_t<c1, 14>::ArgSetViable<jobjectArray>());
static_assert(
    MthdId_t<c1, 14>::ArgSetViable<LocalArray<jobject, 3, kClass1>>());
static_assert(
    !MthdId_t<c1, 14>::ArgSetViable<LocalArray<jobject, 1, kClass1>>());
static_assert(
    !MthdId_t<c1, 14>::ArgSetViable<LocalArray<jobject, 2, kClass1>>());
static_assert(
    !MthdId_t<c1, 14>::ArgSetViable<LocalArray<jobject, 4, kClass1>>());
static_assert(!MthdId_t<c1, 14>::ArgSetViable<jint>());
static_assert(!MthdId_t<c1, 14>::ArgSetViable<jintArray>());

////////////////////////////////////////////////////////////////////////////////
// Multiple arguments per method.
////////////////////////////////////////////////////////////////////////////////
constexpr Class c2{
    "c2", Method{"0", Return<void>{}, Params<jint, jint>{}},
    Method{"1", Return<void>{}, Params<jfloat, jfloat, jfloat>{}},
    Method{"2", Return<void>{}, Params{jint{}, jfloat{}, kClass1}},
    Method{"3", Return<void>{}, Params{jboolean{}, Array{int{}}}}};

static_assert(MthdId_t<c2, 0>::ArgSetViable<int, int>());
static_assert(!MthdId_t<c2, 0>::ArgSetViable<int>());
static_assert(!MthdId_t<c2, 0>::ArgSetViable<int, int, int>());
static_assert(!MthdId_t<c2, 0>::ArgSetViable<double, double>());
static_assert(!MthdId_t<c2, 0>::ArgSetViable<LocalObj1, LocalObj1>());

static_assert(MthdId_t<c2, 1>::ArgSetViable<float, float, float>());
static_assert(!MthdId_t<c2, 1>::ArgSetViable<float>());
static_assert(!MthdId_t<c2, 1>::ArgSetViable<float, float>());

static_assert(MthdId_t<c2, 2>::ArgSetViable<int, float, LocalObj1>());

static_assert(MthdId_t<c2, 3>::ArgSetViable<jboolean, jintArray>());

////////////////////////////////////////////////////////////////////////////////
// Multi param methods, single overload method.
////////////////////////////////////////////////////////////////////////////////
constexpr Class c3{
    "c3", Method{
              "0",
              Overload{Return<void>{}},
              Overload{Return<void>{}, Params<int>{}},
              Overload{Return<void>{}, Params<int, int>{}},
              Overload{Return<void>{}, Params<int, int, float, int>{}},
              Overload{Return<void>{}, Params<jstring>{}},
              Overload{Return<void>{}, Params<jstring, jstring>{}},
              Overload{Return<void>{}, Params{kClass1}},
              Overload{
                  Return<void>{},
                  Params{kClass2},
              },
              Overload{Return<void>{}, Params{jboolean{}, Array{int{}}}},
          }};

static_assert(MthdId_t<c3, 0>::ArgSetViable<int, int>());
static_assert(MthdId_t<c3, 0>::ArgSetViable<int>());
static_assert(MthdId_t<c3, 0>::ArgSetViable<int, int, float, int>());
static_assert(MthdId_t<c3, 0>::ArgSetViable<char*>());
static_assert(MthdId_t<c3, 0>::ArgSetViable<std::string>());
static_assert(MthdId_t<c3, 0>::ArgSetViable<std::string_view>());

// TODO(b/143908983): The following is ambiguous because it could potentially
// refer to multiple signatures. For now, it's viable, but it shouldn't be.
static_assert(MthdId_t<c3, 0>::ArgSetViable<jobject>());

static_assert(!MthdId_t<c3, 0>::ArgSetViable<int, int, int>());
static_assert(!MthdId_t<c3, 0>::ArgSetViable<double, double>());
static_assert(!MthdId_t<c3, 0>::ArgSetViable<LocalObj1, LocalObj1>());
static_assert(!MthdId_t<c3, 0>::ArgSetViable<float>());
static_assert(!MthdId_t<c3, 0>::ArgSetViable<float, float>());
static_assert(!MthdId_t<c3, 0>::ArgSetViable<std::string*>());

static_assert(MthdId_t<c3, 0>::ArgSetViable<jboolean, jintArray>());
static_assert(MthdId_t<c3, 0>::ArgSetViable<jboolean, LocalArray<jint>>());

////////////////////////////////////////////////////////////////////////////////
// Constructor Method Tests
// (Roughly same logic, so just smoking the code path ought be sufficient).
////////////////////////////////////////////////////////////////////////////////
constexpr Class c5{"c5",
                   Constructor{},

                   Constructor{int{}},
                   Constructor{float{}},
                   Constructor{kClass1},

                   Constructor{int{}, int{}},
                   Constructor{float{}, float{}, float{}},
                   Constructor{int{}, float{}, kClass1},
                   Constructor{kClass2}};

template <const auto& class_v>
using ConstructorId_t =
    MethodSelection<Id<JniT<jobject, class_v>, IdType::OVERLOAD_SET, kNoIdx>>;

static_assert(ConstructorId_t<c5>::ArgSetViable<>());
static_assert(ConstructorId_t<c5>::ArgSetViable<int>());
static_assert(ConstructorId_t<c5>::ArgSetViable<float>());
static_assert(ConstructorId_t<c5>::ArgSetViable<LocalObj1>());

static_assert(ConstructorId_t<c5>::ArgSetViable<int, int>());
static_assert(ConstructorId_t<c5>::ArgSetViable<float, float, float>());
static_assert(ConstructorId_t<c5>::ArgSetViable<int, float, LocalObj1>());

// Impermissible arg sets (none of the constructors match.)
static_assert(!ConstructorId_t<c5>::ArgSetViable<int, float>());
static_assert(!ConstructorId_t<c5>::ArgSetViable<int, float, float>());
static_assert(!ConstructorId_t<c5>::ArgSetViable<int, float, LocalObj2>());
static_assert(!ConstructorId_t<c5>::ArgSetViable<LocalInvalidObj>());
static_assert(!ConstructorId_t<c5>::ArgSetViable<GlobalInvalidObj>());

}  // namespace
