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

#ifndef JNI_BIND_METAPROGRAMMING_N_BIT_H_
#define JNI_BIND_METAPROGRAMMING_N_BIT_H_

#include <algorithm>
#include <cstddef>

namespace jni::metaprogramming {

// Represents a value that can be [0, max], and an overflow bit.
// When incremented, the type will rollover, and set its overflow bit.
template <size_t cur_value, size_t max = 1, bool overflow_bit = false>
struct NBit {
  static constexpr size_t value_ = cur_value;
  static constexpr size_t overflow_bit_ = overflow_bit;

  // Only unary values will ever need the clamp.
  using Increment =
      NBit<(value_ == max ? 0 : std::clamp(value_ + 1, size_t{0}, max)), max,
           value_ == max>;

  using ResetOverflow = NBit<cur_value, max, false>;
};

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_N_BIT_H_
