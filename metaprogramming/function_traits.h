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

#ifndef JNI_BIND_METAPROGRAMMING_FunctionTraits_H_
#define JNI_BIND_METAPROGRAMMING_FunctionTraits_H_

#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// Metafunction for querying traits return and argument types of a function.
template <typename>
struct FunctionTraits;

// Partial specialisation for functions.
template <typename Return_, typename... Args>
struct FunctionTraits<Return_(Args...)> {
  using Return = Return_;
  using ArgsTup = std::tuple<Args...>;

  static constexpr std::size_t arity = sizeof...(Args);

  template <std::size_t N>
  struct Argument {
    static_assert(N < arity, "Parameter index exceeds argument count.");
    using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
  };
};

// Partial specialisation for function pointer.
template <typename R, typename... Args>
struct FunctionTraits<R (*)(Args...)> : public FunctionTraits<R(Args...)> {};

// Member function pointer
template <class C, class R, class... Args>
struct FunctionTraits<R (C::*)(Args...)>
    : public FunctionTraits<R(C&, Args...)> {};

// const member function pointer
template <class C, class R, class... Args>
struct FunctionTraits<R (C::*)(Args...) const>
    : public FunctionTraits<R(C&, Args...)> {};

// Member object pointer
template <class C, class R>
struct FunctionTraits<R(C::*)> : public FunctionTraits<R(C&)> {};

template <typename T>
using FunctionTraitsReturn_t = typename FunctionTraits<T>::Return;

template <typename T, std::size_t argument_idx>
using FunctionTraitsArg_t =
    typename FunctionTraits<T>::template Argument<argument_idx>::type;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_FunctionTraits_H_
