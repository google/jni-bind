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

#include "proxy.h"

#include <tuple>
#include <type_traits>

#include "class.h"
#include "class_loader.h"
#include "local_object.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "class.h"
#include "class_loader.h"
#include "jni_dep.h"
#include "metaprogramming/concatenate.h"
#include "supported_class_set.h"

using jni::AsDecl_t;
using jni::Class;
using jni::ClassLoader;
using jni::LocalObject;
using jni::Proxy_t;
using jni::metaprogramming::TypeToTypeMapQuery_t;

namespace jni {

static constexpr Class kClass{"kClass"};
static constexpr Class kClass2{"kClass2"};
static constexpr ClassLoader kClassLoader{kDefaultClassLoader,
                                          SupportedClassSet{kClass}};

// CDecls of primitive types.
static_assert(std::is_same_v<Proxy_t<void>::CDecl, void>);
static_assert(std::is_same_v<Proxy_t<jint>::CDecl, jint>);
static_assert(std::is_same_v<Proxy_t<jfloat>::CDecl, jfloat>);
static_assert(std::is_same_v<Proxy_t<jbyte>::CDecl, jbyte>);
static_assert(std::is_same_v<Proxy_t<jchar>::CDecl, jchar>);
static_assert(std::is_same_v<Proxy_t<jshort>::CDecl, jshort>);
static_assert(std::is_same_v<Proxy_t<jlong>::CDecl, jlong>);
static_assert(std::is_same_v<Proxy_t<jdouble>::CDecl, jdouble>);

// CDecls of non-primitive types.
// Multiple types map to single index.
static_assert(std::is_same_v<Proxy_t<const char*>::CDecl, jstring>);
static_assert(std::is_same_v<decltype("Foo"), char const (&)[4]>);
static_assert(std::is_same_v<Proxy_t<const char (&)[4]>::CDecl, jstring>);

static_assert(std::is_same_v<Proxy_t<const char*>::CDecl, jstring>);
static_assert(std::is_same_v<Proxy_t<std::string_view>::CDecl, jstring>);
static_assert(std::is_same_v<Proxy_t<std::string>::CDecl, jstring>);

void Foo() {
  LocalObject<kClass> obj;
  LocalObject<kClass, kClassLoader> class_loaded_obj;

  static_assert(std::is_same_v<Proxy_t<decltype(kClass)>::CDecl, jobject>);
  static_assert(std::is_same_v<Proxy_t<decltype(obj)>::CDecl, jobject>);
  static_assert(
      std::is_same_v<Proxy_t<decltype(class_loaded_obj)>::CDecl, jobject>);
}

// Types as declarations.
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

}  // namespace jni
