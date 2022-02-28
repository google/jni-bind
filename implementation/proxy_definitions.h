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
#include <string_view>
#include <tuple>
#include <type_traits>

#include "array.h"
#include "class.h"
#include "class_loader.h"
#include "jvm.h"
#include "local_array.h"
#include "local_string.h"
#include "object.h"
#include "ref_base.h"
#include "implementation/class_loader.h"
#include "implementation/default_class_loader.h"
#include "jni_dep.h"
#include "metaprogramming/contains.h"

namespace jni {

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class LocalObject;

template <typename SpanType, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
class LocalArray;

template <typename Overload>
struct ArrayHelper;

template <typename TUndecayed>
struct ProxyHelper;

// Default Proxy, all types and values are pure passthrough.
template <typename Key_>
struct ProxyBase {
  using Key = Key_;

  using CDecl = Key_;

  template <typename>
  using AsReturn = Key_;

  using AsArg = std::tuple<Key_>;
  using AsDecl = std::tuple<Key_>;

  template <typename T>
  static auto ProxyAsArg(T&& t) {
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
//  |Index|: A uniquely identifying Key for proxy lookup.  This is usually the
//    CDecl (e.g. jint => jint), but rich types may differ (Object =>jobject).
//  |CDecl|: This is both the unique ID for a given proxy, as well as the
//    distinct type (of which there is only one) that is usable when invoking a
//    JNI call through the C API (e.g. |jint|, |jobject|).
//  |AsArg|: All valid passable types.
//  |AsDecl|: The type to be used in a function declaration, either as
//   return or as a declared argument. If is templated by |class_v| and
//   |class_loader_v| which can allow for additional decoration.
template <typename CDecl, typename Enable>
struct Proxy : public ProxyBase<CDecl> {};

static_assert(std::is_same_v<jboolean, unsigned char>);
static_assert(std::is_same_v<jint, int>);
static_assert(std::is_same_v<jfloat, float>);
// static_assert(std::is_same_v<jlong, long>);  // not true on all platforms.
static_assert(std::is_same_v<jdouble, double>);

template <typename VoidType>
struct Proxy<VoidType,
             typename std::enable_if_t<std::is_same_v<VoidType, void>>>
    : public ProxyBase<void> {};

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

  template <typename Return>
  using AsReturn = LocalString;

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

  template <typename OverloadT>
  struct Helper {
    // It's illegal to initialise this type with a sub-object of another,
    // however, we can construct types with enough validation to guarantee
    // correctness.
    static constexpr Class kClass{OverloadT::GetReturn().return_raw_.name_};

    // TODO(b/174272629): Class loaders should also be enforced.
    using type = LocalObject<kClass, kDefaultClassLoader, kDefaultJvm>;
  };

  template <typename Overload>
  using AsReturn = typename Helper<Overload>::type;

  static jobject ProxyAsArg(jobject obj) { return obj; };

  // Applies for both local and global.
  template <typename T>
  static jobject ProxyAsArg(T& t) {
    return jobject{t};
  };

  // Applies for both local and global.
  template <typename T>
  static jobject ProxyAsArg(T&& t) {
    return t.Release();
  };
};

template <typename JArrayType>
struct Proxy<JArrayType,
             typename std::enable_if_t<std::is_same_v<JArrayType, jarray>>>
    : public ProxyBase<JArrayType> {
  using AsDecl = std::tuple<ArrayTag>;
  // TODO(b/143908983): Constrain input types to the correct array type.  This
  // is overly permissive and will allow incorrect types to be passed.
  using AsArg =
      std::tuple<jarray, jbooleanArray, jbyteArray, jcharArray, jfloatArray,
                 jintArray, jlongArray, ArrayTag, RefBaseTag<jarray>>;

  using CDecl = jobject;

  template <typename Overload>
  using AsReturn = typename ArrayHelper<Overload>::AsReturn;

  static jarray ProxyAsArg(jarray arr) { return arr; };

  template <typename T>
  static jarray ProxyAsArg(T& t) {
    return jarray{t};
  };

  template <typename T, typename = std::enable_if_t<
                            std::is_base_of_v<RefBaseTag<jarray>, T>>>
  static jarray ProxyAsArg(T&& t) {
    return t.Release();
  };
};

template <typename JObjectArrayType>
struct Proxy<
    JObjectArrayType,
    typename std::enable_if_t<std::is_same_v<JObjectArrayType, jobjectArray>>>
    : public ProxyBase<JObjectArrayType> {
  using AsDecl = std::tuple<ObjectArrayTag>;
  using AsArg =
      std::tuple<jobjectArray, ObjectArrayTag, RefBaseTag<jobjectArray>>;
  using CDecl = jobjectArray;

  template <typename Overload>
  using AsReturn = typename ArrayHelper<Overload>::AsReturn;

  static jobjectArray ProxyAsArg(jobjectArray arr) { return arr; };

  template <typename T>
  static jobjectArray ProxyAsArg(T& t) {
    return jobjectArray{t};
  };

  template <typename T, typename = std::enable_if_t<
                            std::is_base_of_v<RefBaseTag<jobjectArray>, T>>>
  static jobjectArray ProxyAsArg(T&& t) {
    return t.Release();
  };
};

// This must be defined outside of Proxy so implicit definition doesn't occur.
template <typename Overload>
struct ArrayHelper {
  static constexpr Array kArray{Overload::GetReturn().return_raw_};
  using FullRawReturnT = std::decay_t<decltype(kArray.raw_type_)>;

  using AsReturn = decltype(LocalArrayBuildFromArray<kArray>());
};

}  // namespace jni

#endif  // JNI_BIND_TYPE_PROXY_DEFINITIONS_H_
