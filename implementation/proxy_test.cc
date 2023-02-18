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

#include <tuple>
#include <type_traits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"
#include "metaprogramming/concatenate.h"

using jni::AsDecl_t;
using jni::Class;
using jni::ClassLoader;
using jni::GlobalObject;
using jni::LocalObject;
using jni::Proxy_t;
using jni::Return_t;
using jni::metaprogramming::TypeToTypeMapQuery_t;
using jni::test::JniTest;

namespace jni {

static constexpr Class kClass{
    "kClass", jni::Method{"Bar", jni::Return<void>{}, jni::Params{}}};
static constexpr Class kClass2{
    "kClass2",
    jni::Method{"ReturnsObj", jni::Return{kClass}, jni::Params{}},
};

static constexpr ClassLoader kClassLoader{kDefaultClassLoader,
                                          SupportedClassSet{kClass}};

////////////////////////////////////////////////////////////////////////////////
// Primitive Tests.
////////////////////////////////////////////////////////////////////////////////
static_assert(std::is_same_v<Proxy_t<void>::CDecl, void>);
static_assert(std::is_same_v<Proxy_t<jint>::CDecl, jint>);
static_assert(std::is_same_v<Proxy_t<jfloat>::CDecl, jfloat>);
static_assert(std::is_same_v<Proxy_t<jbyte>::CDecl, jbyte>);
static_assert(std::is_same_v<Proxy_t<jchar>::CDecl, jchar>);
static_assert(std::is_same_v<Proxy_t<jshort>::CDecl, jshort>);
static_assert(std::is_same_v<Proxy_t<jlong>::CDecl, jlong>);
static_assert(std::is_same_v<Proxy_t<jdouble>::CDecl, jdouble>);
static_assert(std::is_same_v<Proxy_t<const char*>::CDecl, jstring>);
static_assert(std::is_same_v<Proxy_t<std::string_view>::CDecl, jstring>);
static_assert(std::is_same_v<Proxy_t<std::string>::CDecl, jstring>);

////////////////////////////////////////////////////////////////////////////////
// Object Tests.
////////////////////////////////////////////////////////////////////////////////
void ReturnsObj() {
  LocalObject<kClass> obj;

  static_assert(std::is_same_v<Proxy_t<decltype(kClass)>::CDecl, jobject>);
  static_assert(std::is_same_v<Proxy_t<decltype(obj)>::CDecl, jobject>);
  static_assert(
      std::is_same_v<Proxy_t<LocalObject<kClass, kClassLoader>>::CDecl,
                     jobject>);
}

////////////////////////////////////////////////////////////////////////////////
// Array Index Lookup Tests.
// For compile time argument viability see |method_selection_test.cc|.
// For runtime testing see |array_test.cc| and |local_array_test.cc|.
////////////////////////////////////////////////////////////////////////////////

// Rank 1 undecorated types.
static_assert(std::is_same_v<Index_t<jbyteArray>, jbyteArray>);
static_assert(std::is_same_v<Index_t<jcharArray>, jcharArray>);
static_assert(std::is_same_v<Index_t<jshortArray>, jshortArray>);
static_assert(std::is_same_v<Index_t<jintArray>, jintArray>);
static_assert(std::is_same_v<Index_t<jfloatArray>, jfloatArray>);
static_assert(std::is_same_v<Index_t<jdoubleArray>, jdoubleArray>);
static_assert(std::is_same_v<Index_t<jlongArray>, jlongArray>);
static_assert(std::is_same_v<Index_t<jbooleanArray>, jbooleanArray>);
static_assert(std::is_same_v<Index_t<jobjectArray>, jobjectArray>);
static_assert(std::is_same_v<Index_t<jarray>, jarray>);

// Rank 1 decorated declarations.
static_assert(std::is_same_v<Index_t<Array<jbyte>>, jbyteArray>);
static_assert(std::is_same_v<Index_t<Array<jchar>>, jcharArray>);
static_assert(std::is_same_v<Index_t<Array<jshort>>, jshortArray>);
static_assert(std::is_same_v<Index_t<Array<jint>>, jintArray>);
static_assert(std::is_same_v<Index_t<Array<jfloat>>, jfloatArray>);
static_assert(std::is_same_v<Index_t<Array<jdouble>>, jdoubleArray>);
static_assert(std::is_same_v<Index_t<Array<jlong>>, jlongArray>);
static_assert(std::is_same_v<Index_t<Array<jboolean>>, jbooleanArray>);
static_assert(std::is_same_v<Index_t<Array<jarray>>, jarray>);
static_assert(std::is_same_v<Index_t<Array<std::decay_t<decltype(kClass)>>>,
                             jobjectArray>);

// Rank 1 decorated arguments.
static_assert(
    std::is_same_v<Index_t<RefBaseTag<jbooleanArray>>, jbooleanArray>);
static_assert(std::is_same_v<Index_t<RefBaseTag<jbyteArray>>, jbyteArray>);
static_assert(std::is_same_v<Index_t<RefBaseTag<jcharArray>>, jcharArray>);
static_assert(std::is_same_v<Index_t<RefBaseTag<jshortArray>>, jshortArray>);
static_assert(std::is_same_v<Index_t<RefBaseTag<jintArray>>, jintArray>);
static_assert(std::is_same_v<Index_t<RefBaseTag<jfloatArray>>, jfloatArray>);
static_assert(std::is_same_v<Index_t<RefBaseTag<jdoubleArray>>, jdoubleArray>);
static_assert(std::is_same_v<Index_t<RefBaseTag<jlongArray>>, jlongArray>);
static_assert(std::is_same_v<Index_t<RefBaseTag<jobjectArray>>, jobjectArray>);
static_assert(std::is_same_v<Index_t<RefBaseTag<jarray>>, jarray>);

// Rank 2 decorated declarations.
static_assert(std::is_same_v<Index_t<Array<jbyte, 2>>, jbyteArray>);
static_assert(std::is_same_v<Index_t<Array<jchar, 2>>, jcharArray>);
static_assert(std::is_same_v<Index_t<Array<jshort, 2>>, jshortArray>);
static_assert(std::is_same_v<Index_t<Array<jint, 2>>, jintArray>);
static_assert(std::is_same_v<Index_t<Array<jfloat, 2>>, jfloatArray>);
static_assert(std::is_same_v<Index_t<Array<jdouble, 2>>, jdoubleArray>);
static_assert(std::is_same_v<Index_t<Array<jlong, 2>>, jlongArray>);
static_assert(std::is_same_v<Index_t<Array<jboolean, 2>>, jbooleanArray>);
static_assert(std::is_same_v<Index_t<Array<jarray, 2>>, jarray>);
static_assert(std::is_same_v<Index_t<Array<jobject, 2>>, jobjectArray>);

// Rank 3 decorated declarations.
static_assert(std::is_same_v<Index_t<Array<jbyte, 3>>, jbyteArray>);
static_assert(std::is_same_v<Index_t<Array<jchar, 3>>, jcharArray>);
static_assert(std::is_same_v<Index_t<Array<jshort, 3>>, jshortArray>);
static_assert(std::is_same_v<Index_t<Array<jint, 3>>, jintArray>);
static_assert(std::is_same_v<Index_t<Array<jfloat, 3>>, jfloatArray>);
static_assert(std::is_same_v<Index_t<Array<jdouble, 3>>, jdoubleArray>);
static_assert(std::is_same_v<Index_t<Array<jlong, 3>>, jlongArray>);
static_assert(std::is_same_v<Index_t<Array<jboolean, 3>>, jbooleanArray>);
static_assert(std::is_same_v<Index_t<Array<jarray, 3>>, jarray>);
static_assert(std::is_same_v<Index_t<Array<jobject, 3>>, jobjectArray>);

////////////////////////////////////////////////////////////////////////////////
// Types as declarations.
////////////////////////////////////////////////////////////////////////////////
static_assert(std::is_same_v<AsDecl_t<void>, std::tuple<void>>);
static_assert(std::is_same_v<AsDecl_t<jint>, std::tuple<int>>);
static_assert(std::is_same_v<AsDecl_t<jfloat>, std::tuple<float>>);
static_assert(std::is_same_v<AsDecl_t<jbyte>, std::tuple<jbyte>>);
static_assert(std::is_same_v<AsDecl_t<jchar>, std::tuple<char, jchar>>);
static_assert(std::is_same_v<AsDecl_t<jshort>, std::tuple<jshort>>);
static_assert(std::is_same_v<AsDecl_t<jlong>, std::tuple<long, jlong>>);
static_assert(std::is_same_v<AsDecl_t<jdouble>, std::tuple<double>>);

// Note, sizeof these types are _not_ equal on x86.
static_assert(!std::is_same_v<AsDecl_t<jboolean>, std::tuple<bool>>);
static_assert(std::is_same_v<AsDecl_t<jboolean>, std::tuple<jboolean, bool>>);

// AsDecl of non-trivial types with multiple keys.
static_assert(std::is_same_v<AsDecl_t<const char*>, std::tuple<jstring>>);
static_assert(std::is_same_v<AsDecl_t<std::string_view>, std::tuple<jstring>>);
static_assert(std::is_same_v<AsDecl_t<std::string>, std::tuple<jstring>>);

////////////////////////////////////////////////////////////////////////////////
// Types as return values.
////////////////////////////////////////////////////////////////////////////////

struct DummyOverload {};

static_assert(std::is_same_v<Return_t<void, DummyOverload>, void>);
static_assert(std::is_same_v<Return_t<jint, DummyOverload>, jint>);
static_assert(std::is_same_v<Return_t<jfloat, DummyOverload>, jfloat>);
static_assert(std::is_same_v<Return_t<jbyte, DummyOverload>, jbyte>);
static_assert(std::is_same_v<Return_t<jchar, DummyOverload>, jchar>);
static_assert(std::is_same_v<Return_t<jshort, DummyOverload>, jshort>);
static_assert(std::is_same_v<Return_t<jlong, DummyOverload>, jlong>);
static_assert(std::is_same_v<Return_t<jdouble, DummyOverload>, jdouble>);

// Objects are better tested through the convertability of their output.
TEST_F(JniTest, MaterializationTests) {
  // Objects passed in can be from plain old jobject.
  LocalObject<kClass2> obj0{jobject{nullptr}};

  // Objects passed in can be lvalues.
  LocalObject<kClass2> obj1{LocalObject<kClass2>{}};

  // Objects can be rvalues materialized in place.
  LocalObject<kClass2> local_0{LocalObject<kClass2>{}};

  // Objects can be rvalues (here an x-value).
  LocalObject<kClass2> generator_obj{};
  LocalObject<kClass> local_1{generator_obj("ReturnsObj")};

  // Objects can also be described with no actual class definition.
  // These objects won't be usable, but they are "name-safe".
  LocalObject local_2{generator_obj("ReturnsObj")};

  // doesn't compile because of invalid class (good).
  // LocalObject<kClass2> local_3 { generator_obj("ReturnsObj") }; }

  // Globals can be built from locals.
  GlobalObject<kClass> global_1{generator_obj("ReturnsObj")};
  global_1("Bar");

  // Globals can be built from expiring locals without a full type.
  GlobalObject global_2{generator_obj("ReturnsObj")};

  // But they lack sufficient definition to have any invocation!
  // global_2("Bar");

  // They can be promoted however, and thus restore their type info.
  GlobalObject<kClass> global_3{std::move(global_2)};
  global_3("Bar");
}

}  // namespace jni
