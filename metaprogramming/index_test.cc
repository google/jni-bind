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

#include "metaprogramming/index.h"

#include <tuple>

namespace {

using ::jni::metaprogramming::Index_t;
using ::jni::metaprogramming::Index_Tup;
using ::jni::metaprogramming::IndexEntry;

struct A {};
struct B {};
struct C {};

static_assert(std::is_same_v<Index_t<A>, std::tuple<IndexEntry<0, A>>>);

static_assert(std::is_same_v<Index_t<A, B>,
                             std::tuple<IndexEntry<0, A>, IndexEntry<1, B>>>);

static_assert(std::is_same_v<Index_t<A, B, C>,
                             std::tuple<IndexEntry<0, A>, IndexEntry<1, B>,
                                        IndexEntry<2, C>>>);

// Tuple Unroller
static_assert(std::is_same_v<Index_Tup<std::tuple<A, B, C>>,
                             std::tuple<IndexEntry<0, A>, IndexEntry<1, B>,
                                        IndexEntry<2, C>>>);

}  // namespace
