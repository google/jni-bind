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

#include "implementation/array_type_conversion.h"
#include "implementation/default_class_loader.h"
#include "implementation/jvm.h"
#include "metaprogramming/vals_equal.h"

namespace jni {

template <typename SpanType_, const auto& class_v_,
          // const auto& class_loader_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm, std::size_t kRank = 0>
struct JniType {
  static constexpr decltype(class_v_) class_v = class_v_;
  static constexpr decltype(class_loader_v_) class_loader_v = class_loader_v_;
  static constexpr decltype(jvm_v_) jvm_v = jvm_v_;

  using SpanType = SpanType_;
  using StorageType = typename StorageHelper<SpanType_, kRank>::type;

  using ClassT = std::decay_t<decltype(class_v)>;
  using ClassLoaderT = std::decay_t<decltype(class_loader_v)>;
  using JvmT = std::decay_t<decltype(jvm_v)>;
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
