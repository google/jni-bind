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

#ifndef LOCAL_GOOGLE_HOME_JWHPRYOR_JNI_BIND_GERRIT_JNI_BIND_METAPROGRAMMING_SIZE_T_MAX_H_
#define LOCAL_GOOGLE_HOME_JWHPRYOR_JNI_BIND_GERRIT_JNI_BIND_METAPROGRAMMING_SIZE_T_MAX_H_

#include <cstddef>
#include <initializer_list>
#include <limits>

namespace jni::metaprogramming {

constexpr std::size_t kNegativeOne = std::numeric_limits<std::size_t>::max();

// This function returns a std::size_t with a symbolic -1 that will not be
// included. This can be useful for building masks of indexes.
//
// When passed an empty set, this function returns kNegativeOne.
// When passed n elements, this function returns the maximum value, or
//   kNegativeOne if all elements are kNegativeOne.
constexpr std::size_t ModifiedMax(
    std::initializer_list<std::size_t> initializer_lister) {
  if (initializer_lister.size() == 0) {
    return kNegativeOne;
  }

  // If all values -1, return -1.
  bool non_trivial_value_found = false;
  for (const std::size_t val : initializer_lister) {
    non_trivial_value_found |= (val != kNegativeOne);
  }
  if (!non_trivial_value_found) {
    return kNegativeOne;
  }

  std::size_t max = 0;
  for (const std::size_t val : initializer_lister) {
    if (max < val && val != kNegativeOne) {
      max = val;
    }
  }

  return max;
}

}  // namespace jni::metaprogramming

#endif  // LOCAL_GOOGLE_HOME_JWHPRYOR_JNI_BIND_GERRIT_JNI_BIND_METAPROGRAMMING_SIZE_T_MAX_H_
