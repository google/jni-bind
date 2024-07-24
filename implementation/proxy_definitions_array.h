/*
 * Copyright 2022 Google LLC
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

#ifndef JNI_BIND_IMPLEMENTATION_PROXY_DEFINITIONS_ARRAY_H_
#define JNI_BIND_IMPLEMENTATION_PROXY_DEFINITIONS_ARRAY_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <cstddef>
#include <string_view>
#include <tuple>
#include <type_traits>

#include "implementation/array.h"
#include "implementation/array_type_conversion.h"
#include "implementation/default_class_loader.h"
#include "implementation/jvm.h"
#include "implementation/name_constants.h"
#include "implementation/no_class_specified.h"
#include "implementation/proxy.h"
#include "implementation/proxy_convenience_aliases.h"
#include "implementation/ref_base.h"
#include "jni_dep.h"

namespace jni {

template <typename Overload>
struct ArrayHelper;

template <typename SpanType, std::size_t kRank, const auto& class_v_,
          const auto& class_loader_v_, const auto& jvm_v_>
class LocalArray;

template <typename JArrayType>
struct Proxy<JArrayType, typename std::enable_if_t<
                             std::is_convertible_v<JArrayType, jarray>>>
    : public ProxyBase<JArrayType> {
  // Non-array primitive type (e.g. jintArray => jint).
  using CDecl = ArrayToRegularTypeMap_t<JArrayType>;

  // Primitive Array Types (e.g. if JArrayType is jintarray and T is too).
  template <typename ParamSelection, typename T, typename Enable = void>
  struct Helper {
    static constexpr bool val =
        (std::is_same_v<T, JArrayType> && ParamSelection::kRank == 1) ||
        (std::is_same_v<T, jobjectArray> && ParamSelection::kRank >= 2);
  };

  // LocalArray.
  template <typename ParamSelection, typename SpanType, std::size_t kRank,
            const auto& class_v_, const auto& class_loader_v_,
            const auto& jvm_v_>
  struct Helper<ParamSelection, LocalArray<SpanType, kRank, class_v_,
                                           class_loader_v_, jvm_v_>> {
    static constexpr auto param_copy = FullArrayStripV(ParamSelection::Val());

    static constexpr bool val =
        (kRank == ParamSelection::kRank) &&
        (std::is_same_v<SpanType, typename ParamSelection::RawValT> ||
         (std::is_same_v<SpanType, jobjectArray> &&
          ParamSelection::kRank >= 2) ||
         (std::string_view{class_v_.name_} == NameOrNothing_v<param_copy>));
  };

  template <typename ParamSelection, typename T>
  static constexpr bool kViable = Helper<ParamSelection, T>::val;

  using AsDecl = std::tuple<ArrayTag<JArrayType>>;
  using AsArg =
      std::tuple<JArrayType, RefBaseTag<JArrayType>, ArrayTag<JArrayType>>;

  template <typename Id>
  using AsReturn = typename ArrayHelper<Id>::AsReturn;

  static JArrayType ProxyAsArg(JArrayType arr) { return arr; };

  template <typename T>
  static JArrayType ProxyAsArg(const T& t) {
    return JArrayType{t};
  };

  template <typename T, typename = std::enable_if_t<
                            std::is_base_of_v<RefBaseTag<JArrayType>, T>>>
  static JArrayType ProxyAsArg(T&& t) {
    return t.Release();
  };
};

// This must be defined outside of Proxy so implicit definition doesn't occur.
template <typename IdT>
struct ArrayHelper {
  template <const auto& t>
  struct Helper {
    static constexpr auto val = FullArrayStripV(t.raw_);

    using StrippedCDecl = CDecl_t<std::decay_t<decltype(val)>>;
    using ConvertedCDecl = RegularToArrayTypeMap_t<StrippedCDecl>;
  };

  static constexpr auto kVal{IdT::Materialize()};

  static constexpr auto LocalArrayBuildFromArray() {
    using RawT = typename IdT::RawMaterializeT;
    constexpr std::size_t kRank = IdT::kMaterializedRank;

    if constexpr (!std::is_same_v<CDecl_t<RawT>, jobject>) {
      return LocalArray<RawT, kRank, kNoClassSpecified, kDefaultClassLoader,
                        kDefaultJvm>{1};
    } else {
      return LocalArray<jobject, kRank, Helper<kVal>::val, kDefaultClassLoader,
                        kDefaultJvm>{jobjectArray{nullptr}};
    }
  }

  using StrippedCDecl = typename Helper<kVal>::StrippedCDecl;
  using ConvertedCDecl = typename Helper<kVal>::ConvertedCDecl;

  using AsReturn = decltype(LocalArrayBuildFromArray());
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_PROXY_DEFINITIONS_ARRAY_H_
