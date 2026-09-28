/*
 * Copyright 2026 Google LLC
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

#ifndef JNI_BIND_BIND_LANG_PROXY_GENERIC_PROXY_HELPER_H_
#define JNI_BIND_BIND_LANG_PROXY_GENERIC_PROXY_HELPER_H_

#include <tuple>
#include <type_traits>

#include "bind_lang/metaprogramming/cartesian_product.h"
#include "bind_lang/metaprogramming/combine.h"
#include "bind_lang/metaprogramming/concatenate.h"
#include "bind_lang/metaprogramming/flatten.h"
#include "bind_lang/metaprogramming/invoke.h"
#include "bind_lang/metaprogramming/reduce.h"
#include "bind_lang/metaprogramming/type_to_type_map.h"
#include "bind_lang/metaprogramming/unique_set.h"
#include "bind_lang/proxy/is_convertible_key.h"

namespace bind_lang {

// Selects the `ProxyTemplate` partial specialisation for an arbitrary C++ type
// `TUndecayed` by searching every canonical declaration in `AllKeysT`.
//
// Each binding language supplies its own `ProxyTemplate` (e.g. `jni::Proxy`)
// and key corpus (e.g. `jni::AllKeys`), and typically derives a
// language-specific `ProxyHelper` from this.
template <template <typename...> class ProxyTemplate, typename AllKeysT,
          typename TUndecayed>
struct GenericProxyHelper {
  using T = std::decay_t<TUndecayed>;

  // Metafunction that builds a list of a canonical declaration type to all its
  // possible passable types, which may not be the same.  E.g. jint => jint,
  // but jstring => jstring, const char*, std::string_view, std::string, etc.
  struct IndexToKey {
    // Proxies can be indexed by their |AsArg|s or their |AsDecl|.
    template <typename CDecl>
    using type = metaprogramming::CartesianProduct_t<
        std::tuple<CDecl>,
        metaprogramming::UniqueSet_Tup<metaprogramming::ConcatenateTup_t<
            typename ProxyTemplate<CDecl>::AsArg,
            typename ProxyTemplate<CDecl>::AsDecl>>>;
  };

  // Build a list of two element tuples (in preparation to build a map).  e.g.
  // { {jint, int}, {jstring, const char*}, {jstring, std::string}, etc. }.
  // Note that types may map to 1 or more types, such as jstring above.
  using IndexToKeyAsTuples = metaprogramming::Reduce_t<
      metaprogramming::Combine,
      metaprogramming::InvokePerTupArg_t<IndexToKey, AllKeysT>>;

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

  using Proxy_t = ProxyTemplate<Index>;

  using CDecl = typename Proxy_t::CDecl;

  template <typename Id>
  using AsReturn_t = typename Proxy_t::template AsReturn<Id>;

  template <typename ParamSelection>
  using AsArg_t = typename Proxy_t::AsArg;

  using AsDecl_t = typename Proxy_t::AsDecl;
};

}  // namespace bind_lang

#endif  // JNI_BIND_BIND_LANG_PROXY_GENERIC_PROXY_HELPER_H_
