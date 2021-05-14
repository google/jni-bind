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

#ifndef JNI_BIND_TYPE_PROXY_DEFINITIONS_H_
#define JNI_BIND_TYPE_PROXY_DEFINITIONS_H_

#include <string>
#include <tuple>
#include <type_traits>

#include "class.h"
#include "class_loader.h"
#include "local_string.h"
#include "metaprogramming/contains.h"
#include "object.h"
#include "ref_base.h"
#include "class_loader.h"
#include "jni_dep.h"

namespace jni {

// Everything you are permitted to declare at method prototypes.
// Note, if the size can reasonably differ, the jtype is enforced by virtue of
// being a different type (i.e. you can't accidentally widen).
using AllCDecls = std::tuple<void, jboolean, jbyte, jshort, jint, jfloat, jlong,
                             jchar, jdouble, jstring, jobject>;

// Default Proxy, all types and values are pure passthrough.
template <typename CDecl_>
struct ProxyBase {
  using CDecl = CDecl_;

  using AsReturn = CDecl;
  using AsArg = std::tuple<CDecl_>;
  using AsDecl = std::tuple<CDecl_>;

  template <typename T>
  static auto ProxyAsArg(T&& t) {
    return std::forward<T>(t);
  }

  template <typename T>
  static auto ProxyAsReturn(T&& t) {
    return std::forward<T>(t);
  }
};

// Proxy is a metafunction that gives useful conversions from
// types and forwards to a corresponding type that's viable as input.
//
// Note, given the context, different types present differently.  E.g. a |jint|
// is always a jint, but a |jobject| is declared as a |jni::Class|, passed as a
// |jni::RefBase&| and then converted to a |jobject| to cross the C API.
//
// |Proxy_t| will select the correct proxy for any of the above types.  To be
// specific, |Proxy_t| of any type in |Arg| or |AsDecl| will return
// the parent Proxy.
//
// Each proxy exports aliases for a given |CDecl|.
//  |CDecl|: This is both the unique ID for a given proxy, as well as the
//    distinct type (of which there is only one) that is usable when invoking a
//    JNI call through the C API (e.g. |jint|, |jobject|).
//  |AsArg|: All valid passable types.
//  |AsDecl|: The type to be used in a function declaration, either as
//   return or as a declared argument. If is templated by |class_v| and
//   |class_loader_v| which can allow for additional decoration.
template <typename CDecl, typename Enable = void>
struct Proxy : public ProxyBase<CDecl> {
  static_assert(
      metaprogramming::TupContains_v<CDecl, AllCDecls>,
      "You appear to be specifying an unimplemented type. See jni::Proxy.");
};

static_assert(std::is_same_v<jboolean, unsigned char>);
static_assert(std::is_same_v<jint, int>);
static_assert(std::is_same_v<jfloat, float>);
// static_assert(std::is_same_v<jlong, long>);  // not true on all platforms.
static_assert(std::is_same_v<jdouble, double>);

template <typename VoidType>
struct Proxy<VoidType,
             typename std::enable_if_t<std::is_same_v<VoidType, void>>>
    : public ProxyBase<void> {
  static void ProxyAsReturn() {}
};

template <typename CharType>
struct Proxy<CharType,
             typename std::enable_if_t<std::is_same_v<CharType, jchar>>>
    : public ProxyBase<jchar> {
  using AsArg = std::tuple<char, jchar>;

  using AsDecl = std::tuple<char, jchar>;
};

template <typename BooleanType>
struct Proxy<BooleanType,
             typename std::enable_if_t<std::is_same_v<BooleanType, jboolean>>>
    : public ProxyBase<jboolean> {
  using AsArg = std::tuple<jboolean, bool>;
  using AsDecl = std::tuple<jboolean, bool>;
};

template <typename LongType>
struct Proxy<LongType,
             typename std::enable_if_t<std::is_same_v<LongType, jlong>>>
    : public ProxyBase<jlong> {
  using AsArg = std::tuple<long, jlong>;
  using AsDecl = std::tuple<long, jlong>;

  static jlong ProxyAsArg(jlong val) { return val; }

  // jlong is a smaller type on ARM than x86.
  // When jlong is not equivalent, we upcast to the wider type.
  template <typename T,
            typename = std::enable_if_t<std::is_same_v<T, long> &&
                                        !std::is_same_v<jlong, long>>>
  static jlong ProxyAsArg(T val) {
    return jlong{val};
  }
};

template <typename JString>
struct Proxy<JString,
             typename std::enable_if_t<std::is_same_v<JString, jstring>>>
    : public ProxyBase<JString> {
  using AsArg = std::tuple<std::string, jstring, const char*, std::string_view>;

  template <typename>
  static LocalString ProxyAsReturn(jobject obj) {
    return {reinterpret_cast<jstring>(obj)};
  }

  // These leak local instances of strings.  Usually, RAII mechanisms would
  // correctly release local instances, but here we are stripping that so it can
  // be used in a method.  This could be obviated by wrapping the calling scope
  // in a local stack frame.
  static jstring ProxyAsArg(jstring s) { return s; }
  static jstring ProxyAsArg(const char* s) { return LocalString{s}.Release(); }
  static jstring ProxyAsArg(std::string s) { return LocalString{s}.Release(); }
  static jstring ProxyAsArg(std::string_view s) {
    return LocalString{s}.Release();
  }
};

template <typename JObject>
struct Proxy<JObject,
             typename std::enable_if_t<std::is_same_v<JObject, jobject>>>
    : public ProxyBase<jobject> {
  using AsDecl = std::tuple<Object>;
  using AsArg = std::tuple<jobject, RefBaseTag<jobject>>;
  using AsReturn = std::tuple<jobject, RefBaseTag<jobject>>;

  static jobject ProxyAsArg(RefBase<jobject>& obj) { return jobject{obj}; };
  static jobject ProxyAsArg(jobject obj) { return obj; };

  template <typename T>
  static jobject ProxyAsArg(T&& t) {
    return jobject{t};
  };

  template <typename T>
  static auto ProxyAsReturn(jobject obj) {
    return RefBaseTag<jobject>(obj);
  }
};

}  // namespace jni

#endif  // JNI_BIND_TYPE_PROXY_DEFINITIONS_H_
