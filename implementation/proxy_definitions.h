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

// IWYU pragma: private, include "../jni_bind.h"

#include <string_view>
#include <tuple>
#include <type_traits>

#include "implementation/forward_declarations.h"
#include "implementation/id_type.h"
#include "implementation/jni_helper/lifecycle.h"
#include "implementation/jni_type.h"
#include "implementation/jvm.h"
#include "implementation/loaded_by.h"
#include "implementation/object.h"
#include "implementation/proxy.h"
#include "implementation/proxy_convenience_aliases.h"
#include "implementation/ref_base.h"
#include "implementation/self.h"
#include "jni_dep.h"

namespace jni {

template <typename TUndecayed>
struct ProxyHelper;

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

  template <typename OverloadSelection, typename T>
  static constexpr bool kViable = IsConvertibleKey<T>::template value<char> ||
                                  IsConvertibleKey<T>::template value<jchar>;
};

template <typename BooleanType>
struct Proxy<BooleanType,
             typename std::enable_if_t<std::is_same_v<BooleanType, jboolean>>>
    : public ProxyBase<jboolean> {
  using AsArg = std::tuple<jboolean, bool>;
  using AsDecl = std::tuple<jboolean, bool>;

  template <typename OverloadSelection, typename T>
  static constexpr bool kViable =
      IsConvertibleKey<T>::template value<jboolean> ||
      IsConvertibleKey<T>::template value<bool>;
};

template <typename LongType>
struct Proxy<LongType,
             typename std::enable_if_t<std::is_same_v<LongType, jlong>>>
    : public ProxyBase<jlong> {
  using AsArg = std::tuple<long, jlong>;
  using AsDecl = std::tuple<long, jlong>;

  template <typename OverloadSelection, typename T>
  static constexpr bool kViable = IsConvertibleKey<T>::template value<long> ||
                                  IsConvertibleKey<T>::template value<jlong>;

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

////////////////////////////////////////////////////////////////////////////////
// Object Proxy Definitions.
////////////////////////////////////////////////////////////////////////////////
template <typename JObject>
struct Proxy<JObject,
             typename std::enable_if_t<std::is_same_v<JObject, jobject>>>
    : public ProxyBase<jobject> {
  using AsDecl = std::tuple<Object>;
  using AsArg = std::tuple<jobject, RefBaseTag<jobject>, LoaderTag>;

  template <typename InputParamSelectionT, typename T>
  struct ContextualViabilityHelper {
    // TODO(b/143908983): This is overly permissive, see method_selection_test.
    static constexpr bool kViable = std::is_same_v<T, jobject>;
  };

  // Old "LocalObject" form.
  template <typename IdT,
            template <const auto&, const auto&, const auto&> class Container,
            const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  struct ContextualViabilityHelper<IdT,
                                   Container<class_v, class_loader_v, jvm_v>> {
    static constexpr bool kViable =
        std::string_view{class_v.name_} == std::string_view{IdT::Val().name_};
  };

  // New "LocalObject" form.
  template <typename IdT, LifecycleType lifecycleType, const auto& class_v,
            const auto& class_loader_v, const auto& jvm_v>
  struct ContextualViabilityHelper<
      IdT, Scoped<lifecycleType, void,
                  JniT<jobject, class_v, class_loader_v, jvm_v>, jobject>> {
    static constexpr bool kViable =
        std::string_view{class_v.name_} == std::string_view{IdT::Val().name_};
  };

  template <typename IdT, typename T>
  static constexpr bool kViable = ContextualViabilityHelper<IdT, T>::kViable;

  template <typename Id>
  struct Helper {
    static constexpr auto kClass{Id::Val()};
    static constexpr auto kClassLoader{Id::JniT::GetClassLoader()};

    // TODO(b/174272629): Class loaders should also be enforced.
    using type = LocalObject<kClass, kClassLoader, kDefaultJvm>;
  };

  template <typename Id>
  using AsReturn = typename Helper<Id>::type;

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

////////////////////////////////////////////////////////////////////////////////
// Self Proxy Definitions.
////////////////////////////////////////////////////////////////////////////////
template <typename SelfType>
struct Proxy<SelfType,
             typename std::enable_if_t<std::is_same_v<SelfType, Self>>>
    : public ProxyBase<jobject> {
  using AsDecl = std::tuple<Self>;
  using AsArg = std::tuple<Self>;

  template <typename IdT>
  using SelfIdT_t = typename IdT::template ChangeIdType<IdType::CLASS>;

  template <typename Id>
  struct Helper {
    static constexpr auto kClass{Id::Val()};
    static constexpr auto kClassLoader{Id::JniT::GetClassLoader()};

    // TODO(b/174272629): Class loaders should also be enforced.
    using type = LocalObject<kClass, kClassLoader, kDefaultJvm>;
  };

  template <typename Id>
  using AsReturn = typename Helper<Id>::type;

  template <typename IdT, typename T>
  static constexpr bool kViable =
      Proxy<jobject>::template kViable<SelfIdT_t<IdT>, T>;

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

}  // namespace jni

#endif  // JNI_BIND_TYPE_PROXY_DEFINITIONS_H_
