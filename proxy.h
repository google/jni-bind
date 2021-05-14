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

#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "object.h"
#include "proxy_definitions.h"
#include "ref_base.h"
#include "class.h"
#include "class_loader.h"
#include "jni_dep.h"
#include "metaprogramming/cartesian_product.h"
#include "metaprogramming/combine.h"
#include "metaprogramming/concatenate.h"
#include "metaprogramming/flatten.h"
#include "metaprogramming/invoke.h"
#include "metaprogramming/per_element.h"
#include "metaprogramming/reduce.h"
#include "metaprogramming/tuple_manipulation.h"
#include "metaprogramming/type_of_nth_element.h"
#include "metaprogramming/type_to_type_map.h"
#include "metaprogramming/unique_set.h"
#include "metaprogramming/unwrap.h"

namespace jni {

// Instead of directly searching for the type, convertible types are sought.
// E.g. A string like "Foo" the type will be const char[4] not const char*.
template <typename Query>
struct IsConvertibleKey {
  template <typename T>
  static constexpr bool value =
      std::is_same_v<Query, std::decay_t<T>> ||
      std::is_base_of_v<std::decay_t<T>, std::decay_t<Query>> ||
      std::is_base_of_v<std::decay_t<Query>, std::decay_t<T>>;
};

template <typename TUndecayed, const auto& class_v, const auto& class_loader_v>
struct ProxyHelper {
  using T = std::decay_t<TUndecayed>;

  // Metafunction that builds a list of a passable type to all it's possible
  // passable types, which may not be the same.  E.g. jint => jint, but
  // jstring => jstring, const char*, std::string_view, std::string, etc.
  struct CDeclToKey {
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
  using CDeclToKeyAsTuples = metaprogramming::Reduce_t<
      metaprogramming::Combine,
      metaprogramming::InvokePerTupArg_t<CDeclToKey, AllCDecls>>;

  // Collapse this list into a set of keys and values consumable by
  // TypeToTypeMap.
  using CDeclToKeyMap = metaprogramming::TypeToTypeMapFromKeyValuesTup_t<
      metaprogramming::Flatten_t<CDeclToKeyAsTuples>>;

  // When flipped, a type passed can be reverse indexed to select the same
  // Proxy partial specialisation.
  using KeyToCDecl = metaprogramming::TypeToTypeMap_Invert<CDeclToKeyMap>;

  using CDecl =
      metaprogramming::TypeToTypeMapQueryWithComparator_t<KeyToCDecl,
                                                          IsConvertibleKey<T>>;

  using Proxy_t = Proxy<CDecl>;
  using AsReturn_t = typename Proxy_t::AsReturn;
  using AsArg_t = typename Proxy_t::AsArg;
  using AsDecl_t = typename Proxy_t::AsDecl;
};

// See jni::Proxy.
template <typename T, const auto& class_v = kNoClassSpecified,
          const auto& class_loader_v = kDefaultClassLoader>
using Proxy_t = typename ProxyHelper<T, class_v, class_loader_v>::Proxy_t;

template <typename T, const auto& class_v = kNoClassSpecified,
          const auto& class_loader_v = kDefaultClassLoader>
using CDecl_t = typename ProxyHelper<T, class_v, class_loader_v>::CDecl;

template <typename T, const auto& class_v = kNoClassSpecified,
          const auto& class_loader_v = kDefaultClassLoader>
using Return_t = typename ProxyHelper<T, class_v, class_loader_v>::AsReturn_t;

template <typename T, const auto& class_v = kNoClassSpecified,
          const auto& class_loader_v = kDefaultClassLoader>
using Arg_t = typename ProxyHelper<T, class_v, class_loader_v>::AsArg_t;

template <typename T, const auto& class_v = kNoClassSpecified,
          const auto& class_loader_v = kDefaultClassLoader>
using AsDecl_t = typename ProxyHelper<T, class_v, class_loader_v>::AsDecl_t;

////////////////////////////////////////////////////////////////////////////////
// MetaFunction Helpers.
////////////////////////////////////////////////////////////////////////////////
struct ProxyAsArgMetaFunc {
  template <typename T>
  using type = Arg_t<T>;
};

template <const auto& loader, const auto& class_v>
struct ProxyAsDeclMetaFunc {
  template <typename T>
  using type = AsDecl_t<T>;
};

}  // namespace jni

#endif  // JNI_BIND_TYPE_PROXY_H_
