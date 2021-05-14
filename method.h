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

#include "params.h"

namespace jni {

struct InvocationBase {};

// Represents a single overload of a given method.
template <typename ReturnT_, typename Params_>
struct Invocation : InvocationBase {
  const ReturnT_ return_;
  const Params_ params_;

  constexpr Invocation(ReturnT_ return_type, Params_ params)
      : return_(return_type), params_(params) {}
};

template <typename ReturnT_, typename Params_>
Invocation(ReturnT_, Params_) -> Invocation<ReturnT_, Params_>;

struct MethodBase {};

template <typename Returns_, typename Params_>
struct Method {};

template <typename... Returns, typename... Params_>
struct Method<std::tuple<Returns...>, std::tuple<Params_...>>
    : public MethodBase {
 public:
  const char* name_;
  const std::tuple<Invocation<Returns, Params_>...> invocations_;

  // TODO(b/175083373): Add full support for overload sets.
  constexpr auto& Params() const { return std::get<0>(invocations_).params_; }
  constexpr auto& Return() const { return std::get<0>(invocations_).return_; }

  using ReturnT = decltype(std::get<0>(invocations_).return_);
  using ParamsT = decltype(std::get<0>(invocations_).params_);

  template <typename ReturnT_, typename ParamsT_,
            std::enable_if_t<std::is_base_of_v<ParamsBase, ParamsT_>, int> = 0>
  constexpr Method(const char* name, ReturnT_ return_type, ParamsT_ params)
      : name_(name), invocations_(Invocation{return_type, params}) {}

  constexpr Method(const char* name,
                   Invocation<Returns, Params_>... invocations)
      : name_(name), invocations_(invocations...) {}

  std::string Signature() const {
    std::string return_val = Params().GetSignature() + Return().GetSignature();

    return return_val;
  }
};

// CTAD for Non-overloaded form.
template <
    typename ReturnT, typename ParamsT,
    typename = std::enable_if_t<!std::is_base_of_v<InvocationBase, ReturnT> &&
                                !std::is_base_of_v<InvocationBase, ParamsT>>>
Method(const char*, ReturnT, ParamsT)
    -> Method<std::tuple<ReturnT>, std::tuple<ParamsT>>;

// CTAD for Overloaded form.
template <typename... Returns, typename... Params>
Method(const char*, Invocation<Returns, Params>...)
    -> Method<std::tuple<Returns...>, std::tuple<Params...>>;

template <typename ReturnT1, typename ParamsT1, typename ReturnT2,
          typename ParamsT2>
constexpr bool operator==(const Method<ReturnT1, ParamsT1>& lhs,
                          const Method<ReturnT2, ParamsT2>& rhs) {
  return std::string_view(lhs.name_) == std::string_view(rhs.name_);
}

//==============================================================================
template <typename T>
using ReturnT_t = typename T::ReturnT;

template <typename T>
using ParamsT_t = typename T::ParamsT;

}  // namespace jni

#endif  // JNI_BIND_METHOD_H_
