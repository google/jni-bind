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

#ifndef JNI_BIND_JNI_TYPE_PROXY_H_
#define JNI_BIND_JNI_TYPE_PROXY_H_

#include <string_view>
#include <type_traits>

#include "object.h"
#include "ref_base.h"
#include "jni_dep.h"
#include "jni_helper/jni_typename_to_string.h"

namespace jni {

// JniTypeProxy always emits a type equivalent to what the type would be as a
// param.  For simple types these will be equivalent (e.g. int is still int when
// used within a JNI call), but RefBase<ObjectName> maps to jobject.
template <typename T, typename Enable = void>
struct JniTypeProxy {
  static_assert(
      !std::is_void_v<Enable>,
      "You appear to be specifying an unimplemented type.  See JniTypeProxy.");
};

template <typename PrimitiveType>
struct JniTypeProxy<PrimitiveType,
                    typename std::enable_if_t<std::is_void_v<PrimitiveType>>> {
  static constexpr std::string_view ToString() {
    return JavaTypeToString<PrimitiveType>();
  }
};

template <typename PrimitiveType>
struct JniTypeProxy<
    PrimitiveType,
    typename std::enable_if_t<std::is_arithmetic_v<PrimitiveType> ||
                              std::is_same_v<std::byte, PrimitiveType>>> {
  using asInputParam = PrimitiveType;

  // This is intentionally a no-op.  Methods that are called with all primitive
  // types will effectively pass this to JNI directly.
  static PrimitiveType&& Proxy(PrimitiveType&& t) {
    return std::forward<PrimitiveType>(t);
  }

  static constexpr std::string_view ToString() {
    return JavaTypeToString<PrimitiveType>();
  }
};

// Object is the compile time representation of a class definition by
// name only.  RefBase is the equivalent runtime representation.
//
// When objects are used in their static class definition, as a return value or
// parameter in a method for example, they exist in their compile time form
// Object<Name>.
//
// When callers want to use these at runtime, they must be proxied from their
// compile time form to their run time form.
//
// TODO(b/174272629): This no longer prevents invalid classloader selections.
template <typename ObjectT>
struct JniTypeProxy<
    ObjectT,
    std::enable_if_t<std::is_base_of_v<Object, ObjectT> ||
                     std::is_base_of_v<RefBaseTag<jobject>, ObjectT>>> {
  // When objects are passed as function parameters they are captured by
  // reference and not by rvalue.  This prevents unintentional copying, moving,
  // etc. of objects which can trigger lifecycle (e.g. DeleteGlobalRef).
  //
  // It also allows objects which may have only had a forward declaration but no
  // definition to be interchanged with their full type with class definition.
  // using asInputParam = RefBase<jobject>&;
  using asInputParam = RefBaseTag<jobject>&;

  // Methods with rich object types must wrap this in a format the JNI can
  // understand.  This proxies from the decorated object type to a type JNI
  // can actually use.
  template <typename ObjectRefT>
  static jobject Proxy(ObjectRefT&& t) {
    return jobject{t};
  }
};

//==============================================================================
template <typename T>
using JniTypeProxyAsInputParamT_t = typename JniTypeProxy<T>::asInputParam;

//==============================================================================
template <typename Tup>
struct JniTypeProxyAsInputParamTup {};

template <typename... Ts>
struct JniTypeProxyAsInputParamTup<std::tuple<Ts...>> {
  using type = typename std::tuple<JniTypeProxyAsInputParamT_t<Ts>...>;
};

// Proxies an individual method's worth (i.e. std::tuple<params...>) of params.
template <typename Tup>
using JniTypeProxyAsInputParamTup_tup =
    typename JniTypeProxyAsInputParamTup<Tup>::type;

//==============================================================================
template <typename ParamSet>
struct JniTypeProxyMethodSetHelper {};

template <typename... ParamSet>
struct JniTypeProxyMethodSetHelper<std::tuple<ParamSet...>> {
  using type = std::tuple<JniTypeProxyAsInputParamTup_tup<ParamSet>...>;
};

// Proxies a set of methods (i.e. std::tuple<std::tuple<params...>>).
template <typename TupleOfParamTuples>
using JniTypeProxyMethodSetTup_Tup =
    typename JniTypeProxyMethodSetHelper<TupleOfParamTuples>::type;

}  // namespace jni

#endif  // JNI_BIND_JNI_TYPE_PROXY_H_
