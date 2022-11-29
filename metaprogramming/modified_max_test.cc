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

#include "modified_max.h"

namespace {

using jni::metaprogramming::kNegativeOne;
using jni::metaprogramming::ModifiedMax;

// Null sets work.
static_assert(ModifiedMax({}) == kNegativeOne);
static_assert(ModifiedMax({kNegativeOne}) == kNegativeOne);

// Regular integers work.
static_assert(ModifiedMax({std::size_t{1}}) == std::size_t{1});
static_assert(ModifiedMax({std::size_t{1}, std::size_t{2}}) == std::size_t{2});

// -1 is ignored.
static_assert(ModifiedMax({kNegativeOne, std::size_t{2}}) == std::size_t{2});

// Multiple numbers works.
static_assert(ModifiedMax({std::size_t{1}, std::size_t{2}, std::size_t{3}}) ==
              std::size_t{3});
static_assert(ModifiedMax({std::size_t{3}, std::size_t{2}, std::size_t{3}}) ==
              std::size_t{3});

// Long chains, even with -1 work.
static_assert(ModifiedMax({std::size_t{1}, std::size_t{2}, std::size_t{3}}) ==
              std::size_t{3});
static_assert(ModifiedMax({std::size_t{3}, std::size_t{2}, std::size_t{1}}) ==
              std::size_t{3});
static_assert(ModifiedMax({std::size_t{3}, kNegativeOne, std::size_t{2},
                           std::size_t{1}}) == std::size_t{3});

}  // namespace
