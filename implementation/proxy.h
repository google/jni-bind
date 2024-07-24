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

#ifndef JNI_BIND_TYPE_PROXY_H_
#define JNI_BIND_TYPE_PROXY_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <tuple>
#include <type_traits>
#include <utility>

#include "implementation/forward_declarations.h"
#include "implementation/proxy_convenience_aliases.h"
#include "implementation/self.h"
#include "jni_dep.h"
#include "metaprogramming/cartesian_product.h"
#include "metaprogramming/combine.h"
#include "metaprogramming/concatenate.h"
#include "metaprogramming/corpus.h"
#include "metaprogramming/flatten.h"
#include "metaprogramming/invoke.h"
#include "metaprogramming/reduce.h"
#include "metaprogramming/type_to_type_map.h"
#include "metaprogramming/unique_set.h"

namespace jni {

template <typename t1, typename t2 = void>
struct Proxy;

// CDecls for all declarable types (these index into proxy definitions).
using AllKeys = metaprogramming::Corpus_t<
    JniUserDefinedCorpusTag, void, jboolean, jbyte, jshort, jint, jfloat, jlong,
    jchar, jdouble, jstring, jobject, Self, jarray, jobjectArray, jintArray,
    jbooleanArray, jbyteArray, jcharArray, jshortArray, jdoubleArray,
    jfloatArray, jlongArray>;

template <typename TUndecayed>
struct ProxyHelper {
  using T = std::decay_t<TUndecayed>;

  // Metafunction that builds a list of a passable type to all it's possible
  // passable types, which may not be the same.  E.g. jint => jint, but
  // jstring => jstring, const char*, std::string_view, std::string, etc.
  struct IndexToKey {
    // Proxies can be indexed by their |AsArg|s or their |AsDecl|.
    template <typename CDecl>
    using type = metaprogramming::CartesianProduct_t<
        std::tuple<CDecl>,
        metaprogramming::UniqueSet_Tup<metaprogramming::ConcatenateTup_t<
            typename Proxy<CDecl>::AsArg, typename Proxy<CDecl>::AsDecl>>>;
  };

  // Build a list of two element tuples (in preparation to build a map).  e.g.
  // { {jint, int}, {jstring, const char*}, {jstring, std::string}, etc. }.
  // Note that types may map to 1 or more types, such as jstring above.
  using IndexToKeyAsTuples = metaprogramming::Reduce_t<
      metaprogramming::Combine,
      metaprogramming::InvokePerTupArg_t<IndexToKey, AllKeys>>;

  // Collapse this list into a set of keys and values consumable by
  // TypeToTypeMap.
  using IndexToKeyMap = metaprogramming::TypeToTypeMapFromKeyValuesTup_t<
      metaprogramming::Flatten_t<IndexToKeyAsTuples>>;

  // When flipped, a type passed can be reverse indexed to select the same
  // Proxy partial specialisation.
  using KeyToIndex = metaprogramming::TypeToTypeMap_Invert<IndexToKeyMap>;

  using Index =
      metaprogramming::TypeToTypeMapQueryWithComparator_t<KeyToIndex,
                                                          IsConvertibleKey<T>>;

  using Proxy_t = Proxy<Index>;

  using CDecl = typename Proxy_t::CDecl;

  template <typename Id>
  using AsReturn_t = typename Proxy_t::template AsReturn<Id>;

  template <typename ParamSelection>
  using AsArg_t = typename Proxy_t::AsArg;

  using AsDecl_t = typename Proxy_t::AsDecl;
};

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

  template <typename InputParamSelectionT, typename T>
  static constexpr bool kViable = IsConvertibleKey_v<Key_, T>;
};

}  // namespace jni

#endif  // JNI_BIND_TYPE_PROXY_H_
