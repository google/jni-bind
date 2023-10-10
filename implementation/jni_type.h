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

#include "implementation/array.h"
#include "implementation/array_type_conversion.h"
#include "implementation/class.h"
#include "implementation/class_loader.h"
#include "implementation/default_class_loader.h"
#include "implementation/jvm.h"
#include "implementation/loaded_by.h"
#include "implementation/no_idx.h"
#include "jni_dep.h"
#include "metaprogramming/replace_string.h"
#include "metaprogramming/vals_equal_diminished.h"

namespace jni {

template <typename SpanType_, const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm, std::size_t kRank_ = 0,
          std::size_t class_idx_ = kNoIdx,
          std::size_t class_loader_idx_ = kNoIdx>
struct JniT {
  static constexpr std::size_t kRank = kRank_;
  static_assert(kRank != -1);

  // Same as this type, except uses rank-1.
  using RankLess1 = JniT<SpanType_, class_v_, class_loader_v_, jvm_v_,
                         kRank_ - 1, class_idx_, class_loader_idx_>;

  static constexpr bool MinimallySpecified() {
    return (class_v_ == kNoClassSpecified
                ? true
                : (class_idx_ == kNoIdx ? true : false)) &&
           (class_loader_v_ == kDefaultClassLoader
                ? true
                : (class_loader_idx_ == kNoIdx ? true : false));
  }

  static constexpr auto GetClass() {
    if constexpr (class_idx_ != kNoIdx) {
      return StripClassFromLoadedBy(
          std::get<class_idx_>(GetClassLoader().supported_classes_));
    } else {
      return StripClassFromLoadedBy(class_v_);
    }
  }

  static constexpr auto GetClassLoader() {
    if constexpr (class_loader_idx_ != kNoIdx) {
      return StripClassLoaderFromLoadedBy(
          std::get<class_loader_idx_>(jvm_v_.class_loaders_));
    } else {
      return StripClassLoaderFromLoadedBy(class_loader_v_);
    }
  }

  static constexpr std::string_view kName{GetClass().name_};
  static constexpr std::string_view kNameWithDots{
      metaprogramming::Replace_v<kName, '/', '.'>};

  static constexpr const auto& GetJvm() { return jvm_v_; }

  static constexpr auto GetStatic() {
    return FullArrayStripV(GetClass()).static_;
  }

  static constexpr decltype(GetClass()) class_v = GetClass();
  static constexpr decltype(GetClassLoader()) class_loader_v = GetClassLoader();
  static constexpr decltype(jvm_v_) jvm_v = jvm_v_;
  static constexpr decltype(GetStatic()) static_v = GetStatic();

  using SpanType = SpanType_;
  using StorageType = typename StorageHelper<SpanType_, kRank>::type;

  using ClassT = std::decay_t<ArrayStrip_t<std::decay_t<decltype(GetClass())>>>;
  using ClassLoaderT = std::decay_t<decltype(GetClassLoader())>;
  using JvmT = std::decay_t<decltype(jvm_v)>;
  using StaticT = std::decay_t<decltype(GetStatic())>;

  static constexpr ClassT stripped_class_v{FullArrayStripV(GetClass())};

  ////////////////////////////////////////////////////////////////////////////
  // Minimal Spanning type (allows perfect cacheing for fully specified jvm).
  ////////////////////////////////////////////////////////////////////////////

  // kNoIdx for default loader, then provided idx, then calculated idx.
  static constexpr std::size_t MinimalClassIdx() {
    if constexpr (class_idx_ != kNoIdx) {
      // Fully specified (already).
      return class_idx_;
    } else if constexpr (class_loader_v_ == kDefaultClassLoader) {
      // Partially specified.
      return kNoIdx;
    } else {
      if constexpr (class_loader_v_.template IdxOfClass<class_v>() !=
                    kClassNotInLoaderSetIdx) {
        return class_loader_v_.template IdxOfClass<class_v>();
      } else {
        return kNoIdx;
      }
    }
  }

  // kNoIdx for default jvm, then provided idx, then calculated idx.
  static constexpr std::size_t MinimalClassLoaderIdx() {
    // Fully specified (already).
    if constexpr (class_loader_idx_ != kNoIdx) {
      return class_loader_idx_;
    } else if constexpr (jvm_v_ == kDefaultJvm) {
      // Partially specified.
      return kNoIdx;
    } else {
      // Fully specified (derived).
      return jvm_v_.template IdxOfClassLoader<class_loader_v>();
    }
  }

  static constexpr auto& MinimalSpanningClass() {
    if constexpr (MinimalClassIdx() == kNoIdx) {
      // If idx uncomputable, use provided.
      return class_v_;
    } else {
      return kNoClassSpecified;
    }
  }

  static constexpr auto& MinimalSpanningClassLoader() {
    if constexpr (MinimalClassLoaderIdx() == kNoIdx) {
      // If idx uncomputable, use provided.
      return class_loader_v_;
    } else {
      return kNoClassLoaderSpecified;
    }
  }

  // Some compilers seem to only be OK if this indirection is used.
  static constexpr auto& kMinimalSpanningClass = MinimalSpanningClass();
  static constexpr auto& kMinimalSpanningClassLoader =
      MinimalSpanningClassLoader();

  using MinimallySpanningType =
      JniT<SpanType_, kMinimalSpanningClass, kMinimalSpanningClassLoader,
           jvm_v_, kRank, MinimalClassIdx(), MinimalClassLoaderIdx()>;
};

template <typename T1, typename T2>
struct JniTEqual {
  static constexpr bool val = false;
};

template <
    typename SpanType1, const auto& class_v_1, const auto& class_loader_v_1,
    const auto& jvm_v_1, std::size_t kRank_1, std::size_t class_idx_1,
    std::size_t class_loader_idx_1,

    typename SpanType2, const auto& class_v_2, const auto& class_loader_v_2,
    const auto& jvm_v_2, std::size_t kRank_2, std::size_t class_idx_2,
    std::size_t class_loader_idx_2>
struct JniTEqual<JniT<SpanType1, class_v_1, class_loader_v_1, jvm_v_1, kRank_1,
                      class_idx_1, class_loader_idx_1>,
                 JniT<SpanType2, class_v_2, class_loader_v_2, jvm_v_2, kRank_2,
                      class_idx_2, class_loader_idx_2>> {
  static constexpr bool val =
      std::is_same_v<SpanType1, SpanType2> &&
      metaprogramming::ValsEqualDiminished_cr_v<class_v_1, class_v_2> &&
      metaprogramming::ValsEqualDiminished_cr_v<class_loader_v_1,
                                                class_loader_v_2> &&
      metaprogramming::ValsEqualDiminished_cr_v<jvm_v_1, jvm_v_2> &&
      kRank_1 == kRank_2 && class_idx_1 == class_idx_2 &&
      class_loader_idx_1 == class_loader_idx_2;
};

template <typename T1, typename T2>
constexpr bool JniTEqual_v = JniTEqual<T1, T2>::val;

template <typename JniT_, typename SpanT, int kLessRank>
struct RawProxy {
  using RawValT = SpanT;
  static constexpr std::size_t kRank = JniT_::kRank;
  const RawValT raw_{0};
};

template <typename JniT, int kLessRank>
struct RawProxy<JniT, jobject, kLessRank> {
  using RawValT = decltype(JniT::GetClass());

  const RawValT raw_ = JniT::GetClass();
  const char* name_ = JniT::GetClass().name_;

  static constexpr std::size_t kRank = JniT::kRank + kLessRank;
};

// Helper to generate signatures for objects at rank-1 but span types at rank.
// Used in static selection signature generation (for types like LocalArray).
template <typename JniT, int kLessRank>
struct JniTSelector {
  using RawProxyT = RawProxy<JniT, typename JniT::SpanType, kLessRank>;
  using RawValT = typename RawProxyT::RawValT;

  static constexpr std::size_t kRank = RawProxyT::kRank;

  static constexpr auto Val() { return RawProxyT{}; }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_TYPE_H_
