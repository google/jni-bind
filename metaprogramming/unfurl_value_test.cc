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

#include "unfurl_value.h"

namespace {

using ::jni::metaprogramming::kUnfurlValue;

// Sums all of the first I vals.
static constexpr auto kSomeFunc = [](std::size_t I, auto... vals) {
  return I % 2 == 0 && std::array{vals...} == std::array{2,4,6};
};

static constexpr auto a = kUnfurlValue<kSomeFunc, 2, 4, 6>;
static_assert(a == std::array{true, false, true});

}  // namespace
