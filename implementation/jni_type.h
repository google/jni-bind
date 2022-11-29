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

#ifndef JNI_BIND_IMPLEMENTATION_JNI_TYPE_H_
#define JNI_BIND_IMPLEMENTATION_JNI_TYPE_H_

#include <type_traits>
#include <variant>

#include "implementation/array_type_conversion.h"
#include "implementation/class.h"
#include "implementation/class_loader.h"
#include "implementation/default_class_loader.h"
#include "implementation/jvm.h"
#include "metaprogramming/vals_equal.h"

namespace jni {

template <typename SpanType_, const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm, std::size_t kRank = 0,
          std::size_t class_idx_ = kNoIdx,
          std::size_t class_loader_idx_ = kNoIdx>
struct JniType {
  static constexpr const auto& GetClassLoader() {
    if constexpr (class_loader_idx_ != kNoIdx) {
      return std::get<class_loader_idx_>(jvm_v_.class_loaders_);
    } else {
      return class_loader_v_;
    }
  }

  static constexpr const auto& GetClass() {
    if constexpr (class_idx_ != kNoIdx) {
      return std::get<class_idx_>(GetClassLoader().supported_classes_);
    } else {
      return class_v_;
    }
  }

  static constexpr auto GetStatic() { return GetClass().static_; }

  static constexpr decltype(GetClass()) class_v = GetClass();
  static constexpr decltype(GetClassLoader()) class_loader_v = GetClassLoader();
  static constexpr decltype(jvm_v_) jvm_v = jvm_v_;
  static constexpr decltype(GetStatic()) static_v = GetStatic();

  using SpanType = SpanType_;
  using StorageType = typename StorageHelper<SpanType_, kRank>::type;

  using ClassT = std::decay_t<decltype(GetClass())>;
  using ClassLoaderT = std::decay_t<decltype(GetClassLoader())>;
  using JvmT = std::decay_t<decltype(jvm_v)>;
  using StaticT = std::decay_t<decltype(GetStatic())>;
};

template <typename T1, typename T2>
struct JniTypeEqual {
  static constexpr bool val = false;
};

template <typename SpanType1, const auto& class_v_1,
          const auto& class_loader_v_1, const auto& jvm_v_1, typename SpanType2,
          const auto& class_v_2, const auto& class_loader_v_2,
          const auto& jvm_v_2>
struct JniTypeEqual<JniType<SpanType1, class_v_1, class_loader_v_1, jvm_v_1>,
                    JniType<SpanType2, class_v_2, class_loader_v_2, jvm_v_2> > {
  static constexpr bool val =
      std::is_same_v<SpanType1, SpanType2> &&
      metaprogramming::ValsEqual_cr_v<class_v_1, class_v_2> &&
      metaprogramming::ValsEqual_cr_v<class_loader_v_1, class_loader_v_2> &&
      metaprogramming::ValsEqual_cr_v<jvm_v_1, jvm_v_2>;
};

template <typename T1, typename T2>
constexpr bool JniTypeEqual_v = JniTypeEqual<T1, T2>::val;

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_TYPE_H_
