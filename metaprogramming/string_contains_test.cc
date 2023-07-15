/*
 * Copyright 2023 Google LLC
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

#include "metaprogramming/string_contains.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::jni::metaprogramming::StringContains_v;

namespace {

static constexpr std::string_view kStr{"abcdefg"};

static_assert(StringContains_v<kStr, 'a'>);
static_assert(StringContains_v<kStr, 'b'>);
static_assert(StringContains_v<kStr, 'c'>);
static_assert(StringContains_v<kStr, 'd'>);
static_assert(StringContains_v<kStr, 'e'>);
static_assert(StringContains_v<kStr, 'f'>);
static_assert(StringContains_v<kStr, 'g'>);
static_assert(!StringContains_v<kStr, 'h'>);
static_assert(!StringContains_v<kStr, 'i'>);

}  // namespace
