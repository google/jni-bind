/*
 * Copyright 2024 Google LLC
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
#ifndef JNI_BIND_IMPLEMENTATION_JNI_HELPER_TRACE_H_
#define JNI_BIND_IMPLEMENTATION_JNI_HELPER_TRACE_H_

#include <cstddef>
#include <utility>

#include "arg_string.h"
#include "metaprogramming/color.h"
#include "metaprogramming/lambda_string.h"

namespace jni {

// Called once, before any arguments are printed.
template <char... chars>
inline void PreTrace(metaprogramming::StringAsType<chars...> tag) {
  printf(
      "%s%s",
      metaprogramming::Colorize(metaprogramming::Color::kBlueBold,
                                tag.static_chars)
          .data(),
      metaprogramming::Colorize(metaprogramming::Color::kPurple, "(").data());
}

template <typename T1, typename T2>
struct ArgTrace {};

// Called per argument.
template <std::size_t... Is, char... chars>
struct ArgTrace<std::index_sequence<Is...>,
                metaprogramming::StringAsType<chars...> > {
  template <std::size_t I>
  struct Helper {
    static constexpr bool IsLastArgument() { return I == sizeof...(Is) - 1; }

    template <typename Arg>
    static inline void Do(const Arg& arg) {
      printf("%s", metaprogramming::Colorize(metaprogramming::Color::kCyan,
                                             ArgString(arg))
                       .c_str());
      if constexpr (!IsLastArgument()) {
        printf("%s",
               metaprogramming::Colorize(metaprogramming::Color::kYellow, ",")
                   .data());
      }
    }
  };

  template <typename... Args>
  static inline void Do(const Args&... args) {
    (Helper<Is>::Do(args), ...);
  }
};

// Called once, after all arguments.
template <char... chars>
inline void PostTrace(metaprogramming::StringAsType<chars...> tag) {
  printf(
      "%s\n",
      metaprogramming::Colorize(metaprogramming::Color::kPurple, ");").data());
}

template <char... chars, typename... Args>
inline void Trace(metaprogramming::StringAsType<chars...> tag,
                  const Return& ret, const Args&... args) {
// WARNING: This define is temporary and will be replaced. This unblocks
// 1.0 release but will eventually be configurable at compile time.
#ifdef ENABLE_DEBUG_OUTPUT
  PreTrace(tag);
  ArgTrace<std::make_index_sequence<sizeof...(Args)>, decltype(tag)>::Do(
      args...);
  PostTrace(tag);
#endif
}

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_HELPER_TRACE_H_
