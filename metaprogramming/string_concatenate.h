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

#ifndef JNI_BIND_METAPROGRAMMING_STRING_CONCATENATE_H_
#define JNI_BIND_METAPROGRAMMING_STRING_CONCATENATE_H_

#include <array>
#include <cstddef>
#include <string_view>

namespace jni::metaprogramming {

struct StringConcatenate {
  template <std::string_view const&... Vs>
  struct Helper {
    static constexpr auto BuildConcatenation() noexcept {
      constexpr std::size_t len = (Vs.size() + ... + 0);
      std::array<char, len + 1> arr{};
      auto append_single_string =
          [i = 0, &arr](auto const& string_to_concatenate) mutable {
            for (auto c : string_to_concatenate) arr[i++] = c;
          };
      (append_single_string(Vs), ...);
      arr[len] = 0;

      // This stifles an incorrect compiler warning on gcc.
      static_assert(!std::is_same_v<void, decltype(append_single_string)>);

      return arr;
    }

    static constexpr auto arr = BuildConcatenation();
    static constexpr std::string_view value{arr.data(), arr.size() - 1};
  };

  template <std::string_view const&... Vs>
  static constexpr std::string_view value = Helper<Vs...>::value;
};

template <std::string_view const&... Vs>
static constexpr auto StringConcatenate_v = StringConcatenate::value<Vs...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_STRING_CONCATENATE_H_
