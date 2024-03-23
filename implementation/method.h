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
#ifndef JNI_BIND_METHOD_H_
#define JNI_BIND_METHOD_H_

#include <tuple>
#include <type_traits>

#include "implementation/params.h"
#include "implementation/return.h"

namespace jni {

struct OverloadBase {};
struct MethodBase {};

// Represents a single overload of a given method.
template <typename ReturnT_, typename Params_>
struct Overload : OverloadBase {
  const ReturnT_ return_;
  const Params_ params_;

  // `Return`, no `Params`.
  constexpr Overload(ReturnT_ return_type)
      : return_(return_type), params_(Params{}) {}

  // `Return` and `Params`.
  constexpr Overload(ReturnT_ return_type, Params_ params)
      : return_(return_type), params_(params) {}
};

template <typename ReturnT_>
Overload(ReturnT_) -> Overload<ReturnT_, Params<>>;

template <typename ReturnT_, typename Params_>
Overload(ReturnT_, Params_) -> Overload<ReturnT_, Params_>;

template <typename Returns_, typename Params_>
struct Method {};

template <typename... Returns, typename... Params_>
struct Method<std::tuple<Returns...>, std::tuple<Params_...>>
    : public MethodBase {
 public:
  const char* name_;
  const std::tuple<Overload<Returns, Params_>...> invocations_;

  // `Return`, no `Params`.
  template <typename ReturnT_,
            std::enable_if_t<std::is_base_of_v<ReturnBase, ReturnT_>, int> = 0>
  constexpr Method(const char* name, ReturnT_ return_type)
      : name_(name), invocations_(Overload{return_type}) {}

  // `Return` and `Params`.
  template <typename ReturnT_, typename ParamsT_,
            std::enable_if_t<std::is_base_of_v<ReturnBase, ReturnT_>, int> = 0>
  constexpr Method(const char* name, ReturnT_ return_type, ParamsT_ params)
      : name_(name), invocations_(Overload{return_type, params}) {}

  // `Overload` Set.
  constexpr Method(const char* name, Overload<Returns, Params_>... invocations)
      : name_(name), invocations_(invocations...) {}
};

// CTAD for Non-overloaded form, no Params.
template <typename ReturnT, typename = std::enable_if_t<
                                !std::is_base_of_v<OverloadBase, ReturnT>>>
Method(const char*,
       ReturnT) -> Method<std::tuple<ReturnT>, std::tuple<Params<>>>;

// CTAD for Non-overloaded form.
template <
    typename ReturnT, typename ParamsT,
    typename = std::enable_if_t<!std::is_base_of_v<OverloadBase, ReturnT> &&
                                !std::is_base_of_v<OverloadBase, ParamsT>>>
Method(const char*, ReturnT, ParamsT)
    -> Method<std::tuple<ReturnT>, std::tuple<ParamsT>>;

// CTAD for Overloaded form.
template <typename... Returns, typename... Params>
Method(const char*, Overload<Returns, Params>...)
    -> Method<std::tuple<Returns...>, std::tuple<Params...>>;

template <typename ReturnT1, typename ParamsT1, typename ReturnT2,
          typename ParamsT2>
constexpr bool operator==(const Method<ReturnT1, ParamsT1>& lhs,
                          const Method<ReturnT2, ParamsT2>& rhs) {
  return std::string_view(lhs.name_) == std::string_view(rhs.name_);
}

}  // namespace jni

#endif  // JNI_BIND_METHOD_H_
