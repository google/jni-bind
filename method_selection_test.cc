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

#include "method_selection.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "class.h"
#include "class_loader.h"
#include "default_class_loader.h"
#include "global_object.h"
#include "local_object.h"
#include "method.h"
#include "params.h"
#include "ref_base.h"
#include "return.h"
#include "supported_class_set.h"

namespace {

using jni::Class;
using jni::Constructor;
using jni::GlobalObject;
using jni::Invocation;
using jni::kDefaultClassLoader;
using jni::kNoSelection;
using jni::LocalObject;
using jni::Method;
using jni::MethodSelection_t;
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

// Empty method.
constexpr Method m0{"m_0", Return<void>{}, Params{}};

// Single param methods, single overload.
constexpr Method m1{"m_1", Return<void>{}, Params<int>{}};
constexpr Method m2{"m_2", Return<void>{}, Params<float>{}};
constexpr Method m3{"m_3", Return<void>{}, Params{kClass1}};

// Multi param methods, single overload method.
constexpr Method m4{"m_4", Return<void>{}, Params<int, int>{}};
constexpr Method m5{"m_5", Return<void>{}, Params<float, float, float>{}};
constexpr Method m6{"m_6", Return<void>{}, Params{int{}, float{}, kClass1}};
constexpr Method m7{"m_7", Return<void>{}, Params{kClass1}};

constexpr Class c1{"class_1", m0, m1, m2, m3, m4, m5, m6, m7};

template <const auto& class_v, bool is_constructor, size_t method_idx>
using Sel_t =
    MethodSelection_t<kDefaultClassLoader, class_v, is_constructor, method_idx>;

static_assert(Sel_t<c1, false, 0>::ArgSetViable<>());
static_assert(!Sel_t<c1, false, 0>::ArgSetViable<int>());
static_assert(!Sel_t<c1, false, 0>::ArgSetViable<int, float>());
static_assert(!Sel_t<c1, false, 0>::ArgSetViable<LocalObj1>());

static_assert(Sel_t<c1, false, 1>::ArgSetViable<int>());
static_assert(!Sel_t<c1, false, 1>::ArgSetViable<float>());
static_assert(!Sel_t<c1, false, 1>::ArgSetViable<double>());
static_assert(!Sel_t<c1, false, 1>::ArgSetViable<LocalObj1>());

static_assert(Sel_t<c1, false, 2>::ArgSetViable<float>());
static_assert(!Sel_t<c1, false, 2>::ArgSetViable<int>());
static_assert(!Sel_t<c1, false, 2>::ArgSetViable<double>());
static_assert(!Sel_t<c1, false, 2>::ArgSetViable<LocalObj1>());
static_assert(!Sel_t<c1, false, 2>::ArgSetViable<float, float>());

static_assert(Sel_t<c1, false, 3>::ArgSetViable<LocalObj1>());
static_assert(Sel_t<c1, false, 3>::ArgSetViable<GlobalObj1>());
static_assert(!Sel_t<c1, false, 3>::ArgSetViable<int>());
static_assert(!Sel_t<c1, false, 3>::ArgSetViable<double>());
static_assert(!Sel_t<c1, false, 3>::ArgSetViable<LocalObj2>());
static_assert(!Sel_t<c1, false, 3>::ArgSetViable<GlobalObj2>());
static_assert(!Sel_t<c1, false, 3>::ArgSetViable<LocalObj1, LocalObj1>());

static_assert(Sel_t<c1, false, 4>::ArgSetViable<int, int>());
static_assert(!Sel_t<c1, false, 4>::ArgSetViable<int>());
static_assert(!Sel_t<c1, false, 4>::ArgSetViable<int, int, int>());
static_assert(!Sel_t<c1, false, 4>::ArgSetViable<double, double>());
static_assert(!Sel_t<c1, false, 4>::ArgSetViable<LocalObj1, LocalObj1>());

static_assert(Sel_t<c1, false, 5>::ArgSetViable<float, float, float>());
static_assert(!Sel_t<c1, false, 5>::ArgSetViable<float>());
static_assert(!Sel_t<c1, false, 5>::ArgSetViable<float, float>());

static_assert(Sel_t<c1, false, 6>::ArgSetViable<int, float, LocalObj1>());

static_assert(Sel_t<c1, false, 7>::ArgSetViable<LocalObj1>());
static_assert(Sel_t<c1, false, 7>::ArgSetViable<LocalObj1NoDefinition>());
static_assert(!Sel_t<c1, false, 7>::ArgSetViable<LocalObj2>());

////////////////////////////////////////////////////////////////////////////////
// Method with multiple invocations
////////////////////////////////////////////////////////////////////////////////

constexpr Method m_with_overloads{
    Method{"method_with_overload_", Invocation{Return<void>{}, Params{}},
           Invocation{Return<void>{}, Params<int>{}},
           Invocation{Return<void>{}, Params<int, int>{}},
           Invocation{Return<void>{}, Params<int, int, float, int>{}},
           Invocation{Return<void>{}, Params<jstring>{}},
           Invocation{Return<void>{}, Params<jstring, jstring>{}},
           Invocation{Return<void>{}, Params{kClass1}},
           Invocation{
               Return<void>{},
               Params{kClass2},
           }}};

constexpr Class c2{"class_2", m_with_overloads};
using Sel2 = Sel_t<c2, false, 0>;

using Idx = std::pair<std::size_t, std::size_t>;

// These tests use "IdxPair" which reports not only the method idx of the viable
// but also the permutation index as well.
static_assert(Sel2::IdxPair<>() == Idx{0, 0});
static_assert(Sel2::IdxPair<int>() == Idx{1, 0});
static_assert(Sel2::IdxPair<int, int>() == Idx{2, 0});
static_assert(Sel2::IdxPair<int, int, int>() ==
              Idx{kNoSelection, kNoSelection});
static_assert(Sel2::IdxPair<int, int, float>() ==
              Idx{kNoSelection, kNoSelection});
static_assert(Sel2::IdxPair<int, int, float, int>() == Idx{3, 0});

// Note, jstring has multiple valid "permutations".
static_assert(Sel2::IdxPair<std::string>() == Idx{4, 0});
static_assert(Sel2::IdxPair<jstring>() == Idx{4, 1});
static_assert(Sel2::IdxPair<const char*>() == Idx{4, 2});
static_assert(Sel2::IdxPair<std::string_view>() == Idx{4, 3});

// Even when chained together, the permutations correctly index through all
// possibilities, rolling over when all viable types are exhausted.
static_assert(Sel2::IdxPair<std::string, std::string>() == Idx{5, 0});
static_assert(Sel2::IdxPair<std::string, jstring>() == Idx{5, 1});
static_assert(Sel2::IdxPair<std::string, const char*>() == Idx{5, 2});
static_assert(Sel2::IdxPair<std::string, std::string_view>() == Idx{5, 3});
static_assert(Sel2::IdxPair<jstring, std::string>() == Idx{5, 4});

// LocalObjects also have multiple viable types, but only objects of the same
// name are permitted. Storage doesn't matter (e.g. Local or Global is fine).
static_assert(Sel2::IdxPair<jobject>() == Idx{6, 0});
static_assert(Sel2::IdxPair<LocalObj1>() == Idx{6, 1});
static_assert(Sel2::IdxPair<GlobalObj1>() == Idx{6, 1});

static_assert(Sel2::IdxPair<LocalInvalidObj>() ==
              Idx{kNoSelection, kNoSelection});
static_assert(Sel2::IdxPair<GlobalInvalidObj>() ==
              Idx{kNoSelection, kNoSelection});

////////////////////////////////////////////////////////////////////////////////
// Constructor Method Tests
////////////////////////////////////////////////////////////////////////////////

// Empty method.
constexpr Constructor ctor0{};

// Single param methods, single overload.
constexpr Constructor ctor1{int{}};
constexpr Constructor ctor2{float{}};
constexpr Constructor ctor3{kClass1};

// Multi param methodsctor single overload method.
constexpr Constructor ctor4{int{}, int{}};
constexpr Constructor ctor5{float{}, float{}, float{}};
constexpr Constructor ctor6{int{}, float{}, kClass1};
constexpr Constructor ctor7{kClass2};

constexpr Class c_with_overloads{
    "c_with_overloads", ctor0, ctor1, ctor2, ctor3, ctor4, ctor5, ctor6, ctor7};

static_assert(Sel_t<c_with_overloads, true, 0>::ArgSetViable<>());

static_assert(Sel_t<c_with_overloads, true, 0>::ArgSetViable<int>());
static_assert(Sel_t<c_with_overloads, true, 0>::ArgSetViable<float>());
static_assert(Sel_t<c_with_overloads, true, 0>::ArgSetViable<LocalObj1>());

static_assert(Sel_t<c_with_overloads, true, 0>::ArgSetViable<int, int>());
static_assert(
    Sel_t<c_with_overloads, true, 0>::ArgSetViable<float, float, float>());
static_assert(
    Sel_t<c_with_overloads, true, 0>::ArgSetViable<int, float, LocalObj1>());

// Impermissible arg sets (none of the constructors match.)
static_assert(!Sel_t<c_with_overloads, true, 0>::ArgSetViable<int, float>());
static_assert(
    !Sel_t<c_with_overloads, true, 0>::ArgSetViable<int, float, float>());
static_assert(
    !Sel_t<c_with_overloads, true, 0>::ArgSetViable<int, float, LocalObj2>());
static_assert(
    !Sel_t<c_with_overloads, true, 0>::ArgSetViable<LocalInvalidObj>());
static_assert(
    !Sel_t<c_with_overloads, true, 0>::ArgSetViable<GlobalInvalidObj>());

// IdxPairs work also.
using SelCtor = Sel_t<c_with_overloads, true, 0>;
static_assert(SelCtor::IdxPair<>() == Idx{0, 0});
static_assert(SelCtor::IdxPair<int>() == Idx{1, 0});
static_assert(SelCtor::IdxPair<float>() == Idx{2, 0});
static_assert(SelCtor::IdxPair<jobject>() == Idx{3, 0});
static_assert(SelCtor::IdxPair<LocalObj1>() == Idx{3, 1});
static_assert(SelCtor::IdxPair<int, int>() == Idx{4, 0});
static_assert(SelCtor::IdxPair<float, float, float>() == Idx{5, 0});
static_assert(SelCtor::IdxPair<int, float, jobject>() == Idx{6, 0});
static_assert(SelCtor::IdxPair<int, float, LocalObj1>() == Idx{6, 1});
// TODO(b/174272629): Disallow ambiguous invocations (jobject viable for 3 & 7).
static_assert(SelCtor::IdxPair<jobject>() == Idx{3, 0});
static_assert(SelCtor::IdxPair<LocalObj2>() == Idx{7, 1});

}  // namespace
