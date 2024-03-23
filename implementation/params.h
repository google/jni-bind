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

#ifndef JNI_BIND_PARAMS_H_
#define JNI_BIND_PARAMS_H_

#include <string>
#include <string_view>
#include <tuple>
#include <utility>

namespace jni {

struct ParamsBase {};

// Represents a sequence of parameters for a method in a compile time
// definition.
//
// These may be in their native type (e.g. int, float, etc) or they may have
// some minimal type wrapping such as Object<Name> (see JniTProxy).
template <typename... ValuesRaw_>
struct Params : ParamsBase {
 public:
  using ParamsRawTup = std::tuple<ValuesRaw_...>;

  const std::tuple<ValuesRaw_...> values_ = {};

  // Zero args constructor.
  template <std::size_t N = sizeof...(ValuesRaw_),
            typename std::enable_if<(N == 0), int>::type = 0>
  constexpr explicit Params() {}

  // CTAD constructor, fully defined args (e.g. Params{class_value}).
  template <
      std::size_t N = sizeof...(ValuesRaw_), typename... Args,
      typename std::enable_if<(N > 0 && sizeof...(Args) == N), int>::type = 0>
  constexpr explicit Params(Args... args) : values_(args...) {}

  // Constructor for explicit templatizing, no args (e.g. Params<jint>{}).
  template <
      std::size_t N = sizeof...(ValuesRaw_), typename... Args,
      typename std::enable_if<(N > 0 && sizeof...(Args) == 0), int>::type = 0>
  constexpr explicit Params() {}
};

template <typename... Ts>
Params(Ts...) -> Params<Ts...>;

template <typename... LhsValuesRaw, typename... RhsValuesRaw>
constexpr bool operator==(const Params<LhsValuesRaw...>& lhs,
                          const Params<RhsValuesRaw...>& rhs) {
  return lhs.values_ == rhs.values_;
}

//==============================================================================

template <typename T>
using ParamsRawTup_t = typename T::ParamsRawTup;

}  // namespace jni

#endif  // JNI_BIND_PARAMS_H_
