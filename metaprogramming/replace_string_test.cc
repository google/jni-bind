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
#include "metaprogramming/replace_string.h"

#include <string_view>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::jni::metaprogramming::Replace_v;

namespace {

static constexpr std::string_view kStr{"abcdefg"};

static_assert(Replace_v<kStr, 'a', 'X'> == std::string_view{"Xbcdefg"});
static_assert(Replace_v<kStr, 'b', 'X'> == std::string_view{"aXcdefg"});
static_assert(Replace_v<kStr, 'c', 'X'> == std::string_view{"abXdefg"});
static_assert(Replace_v<kStr, 'd', 'X'> == std::string_view{"abcXefg"});
static_assert(Replace_v<kStr, 'e', 'X'> == std::string_view{"abcdXfg"});
static_assert(Replace_v<kStr, 'f', 'X'> == std::string_view{"abcdeXg"});
static_assert(Replace_v<kStr, 'g', 'X'> == std::string_view{"abcdefX"});

}  // namespace
