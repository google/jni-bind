// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"

namespace {

using jni::Array;
using jni::Class;
using jni::Constructor;
using jni::GlobalObject;
using jni::kDefaultClassLoader;
using jni::LocalObject;
using jni::Method;
using jni::MethodSelection_t;
using jni::Overload;
using jni::Params;
using jni::Return;

static constexpr Class kClass1{"class1", Method{"Foo", Return{}, Params{}}};
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
constexpr Class c1{"c1",
                   Method{"0", Return<void>{}, Params{}},
                   Method{"1", Return<void>{}, Params<jboolean>{}},
                   Method{"2", Return<void>{}, Params<jchar>{}},
                   Method{"3", Return<void>{}, Params<jshort>{}},
                   Method{"4", Return<void>{}, Params<jint>{}},
                   Method{"5", Return<void>{}, Params<jfloat>{}},
                   Method{"6", Return<void>{}, Params<jdouble>{}},
                   Method{"7", Return<void>{}, Params{jstring{}}},
                   Method{"8", Return<void>{}, Params{kClass1}},
                   Method{"9", Return<void>{}, Params{Array{int{}}}},
                   Method{"10", Return<void>{}, Params{Array{Array{int{}}}}},
                   Method{"11", Return<void>{}, Params{bool{}, Array{int{}}}}};

template <const auto& class_v, bool is_constructor, size_t method_idx>
using Sel_t =
    MethodSelection_t<kDefaultClassLoader, class_v, is_constructor, method_idx>;

static_assert(Sel_t<c1, false, 0>::ArgSetViable<>());

static_assert(!Sel_t<c1, false, 0>::ArgSetViable<int>());
static_assert(!Sel_t<c1, false, 0>::ArgSetViable<int, float>());
static_assert(!Sel_t<c1, false, 0>::ArgSetViable<LocalObj1>());

static_assert(Sel_t<c1, false, 1>::ArgSetViable<jboolean>());
static_assert(Sel_t<c1, false, 1>::ArgSetViable<bool>());

static_assert(Sel_t<c1, false, 2>::ArgSetViable<jchar>());
static_assert(Sel_t<c1, false, 2>::ArgSetViable<char>());

static_assert(Sel_t<c1, false, 3>::ArgSetViable<jshort>());
static_assert(Sel_t<c1, false, 3>::ArgSetViable<short>());

static_assert(Sel_t<c1, false, 4>::ArgSetViable<int>());
static_assert(!Sel_t<c1, false, 4>::ArgSetViable<float>());
static_assert(!Sel_t<c1, false, 4>::ArgSetViable<double>());
static_assert(!Sel_t<c1, false, 4>::ArgSetViable<LocalObj1>());

static_assert(Sel_t<c1, false, 5>::ArgSetViable<float>());
static_assert(!Sel_t<c1, false, 5>::ArgSetViable<int>());
static_assert(!Sel_t<c1, false, 5>::ArgSetViable<double>());
static_assert(!Sel_t<c1, false, 5>::ArgSetViable<LocalObj1>());
static_assert(!Sel_t<c1, false, 5>::ArgSetViable<float, float>());

static_assert(Sel_t<c1, false, 6>::ArgSetViable<jdouble>());
static_assert(!Sel_t<c1, false, 6>::ArgSetViable<int>());
static_assert(!Sel_t<c1, false, 6>::ArgSetViable<float>());
static_assert(!Sel_t<c1, false, 6>::ArgSetViable<LocalObj1>());
static_assert(!Sel_t<c1, false, 6>::ArgSetViable<float, float>());

static_assert(Sel_t<c1, false, 7>::ArgSetViable<std::string>());
static_assert(Sel_t<c1, false, 7>::ArgSetViable<jstring>());
static_assert(Sel_t<c1, false, 7>::ArgSetViable<const char*>());
static_assert(Sel_t<c1, false, 7>::ArgSetViable<char[10]>());
static_assert(Sel_t<c1, false, 7>::ArgSetViable<std::string_view>());
static_assert(Sel_t<c1, false, 7>::ArgSetViable<const char (&)[18]>());

static_assert(Sel_t<c1, false, 8>::ArgSetViable<jobject>());
static_assert(Sel_t<c1, false, 8>::ArgSetViable<LocalObj1>());
static_assert(Sel_t<c1, false, 8>::ArgSetViable<LocalObj1NoDefinition>());
static_assert(!Sel_t<c1, false, 8>::ArgSetViable<LocalObj2>());
static_assert(Sel_t<c1, false, 8>::ArgSetViable<LocalObj1>());
static_assert(Sel_t<c1, false, 8>::ArgSetViable<GlobalObj1>());
static_assert(!Sel_t<c1, false, 8>::ArgSetViable<int>());
static_assert(!Sel_t<c1, false, 8>::ArgSetViable<double>());
static_assert(!Sel_t<c1, false, 8>::ArgSetViable<LocalObj2>());
static_assert(!Sel_t<c1, false, 8>::ArgSetViable<GlobalObj2>());
static_assert(!Sel_t<c1, false, 8>::ArgSetViable<LocalObj1, LocalObj1>());

static_assert(Sel_t<c1, false, 9>::ArgSetViable<jintArray>());
static_assert(!Sel_t<c1, false, 9>::ArgSetViable<jfloatArray>());

////////////////////////////////////////////////////////////////////////////////
// Multiple arguments per method.
////////////////////////////////////////////////////////////////////////////////
constexpr Class c2{
    "c2", Method{"0", Return<void>{}, Params<jint, jint>{}},
    Method{"1", Return<void>{}, Params<jfloat, jfloat, jfloat>{}},
    Method{"2", Return<void>{}, Params{jint{}, jfloat{}, kClass1}},
    Method{"3", Return<void>{}, Params{jboolean{}, Array{int{}}}}};

static_assert(Sel_t<c2, false, 0>::ArgSetViable<int, int>());
static_assert(!Sel_t<c2, false, 0>::ArgSetViable<int>());
static_assert(!Sel_t<c2, false, 0>::ArgSetViable<int, int, int>());
static_assert(!Sel_t<c2, false, 0>::ArgSetViable<double, double>());
static_assert(!Sel_t<c2, false, 0>::ArgSetViable<LocalObj1, LocalObj1>());

static_assert(Sel_t<c2, false, 1>::ArgSetViable<float, float, float>());
static_assert(!Sel_t<c2, false, 1>::ArgSetViable<float>());
static_assert(!Sel_t<c2, false, 1>::ArgSetViable<float, float>());

static_assert(Sel_t<c2, false, 2>::ArgSetViable<int, float, LocalObj1>());

static_assert(Sel_t<c2, false, 3>::ArgSetViable<jboolean, jintArray>());

////////////////////////////////////////////////////////////////////////////////
// Multi param methods, single overload method.
////////////////////////////////////////////////////////////////////////////////
constexpr Class c3{
    "c3", Method{
              "0",
              Overload{Return<void>{}, Params{}},
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

static_assert(Sel_t<c3, false, 0>::ArgSetViable<int, int>());
static_assert(Sel_t<c3, false, 0>::ArgSetViable<int>());
static_assert(Sel_t<c3, false, 0>::ArgSetViable<int, int, float, int>());
static_assert(Sel_t<c3, false, 0>::ArgSetViable<char*>());
static_assert(Sel_t<c3, false, 0>::ArgSetViable<std::string>());
static_assert(Sel_t<c3, false, 0>::ArgSetViable<std::string_view>());

// TODO(b/143908983): The following is ambiguous because it could potentially
// refer to multiple signatures. For now, it's viable, but it shouldn't be.
static_assert(Sel_t<c3, false, 0>::ArgSetViable<jobject>());

static_assert(!Sel_t<c3, false, 0>::ArgSetViable<int, int, int>());
static_assert(!Sel_t<c3, false, 0>::ArgSetViable<double, double>());
static_assert(!Sel_t<c3, false, 0>::ArgSetViable<LocalObj1, LocalObj1>());
static_assert(!Sel_t<c3, false, 0>::ArgSetViable<float>());
static_assert(!Sel_t<c3, false, 0>::ArgSetViable<float, float>());
static_assert(!Sel_t<c3, false, 0>::ArgSetViable<std::string*>());

static_assert(Sel_t<c3, false, 0>::ArgSetViable<jboolean, jintArray>());

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

static_assert(Sel_t<c5, true, 0>::ArgSetViable<>());

static_assert(Sel_t<c5, true, 0>::ArgSetViable<int>());
static_assert(Sel_t<c5, true, 0>::ArgSetViable<float>());
static_assert(Sel_t<c5, true, 0>::ArgSetViable<LocalObj1>());

static_assert(Sel_t<c5, true, 0>::ArgSetViable<int, int>());
static_assert(Sel_t<c5, true, 0>::ArgSetViable<float, float, float>());
static_assert(Sel_t<c5, true, 0>::ArgSetViable<int, float, LocalObj1>());

// Impermissible arg sets (none of the constructors match.)
static_assert(!Sel_t<c5, true, 0>::ArgSetViable<int, float>());
static_assert(!Sel_t<c5, true, 0>::ArgSetViable<int, float, float>());
static_assert(!Sel_t<c5, true, 0>::ArgSetViable<int, float, LocalObj2>());
static_assert(!Sel_t<c5, true, 0>::ArgSetViable<LocalInvalidObj>());
static_assert(!Sel_t<c5, true, 0>::ArgSetViable<GlobalInvalidObj>());

}  // namespace
