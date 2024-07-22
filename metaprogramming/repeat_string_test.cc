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

#include "metaprogramming/repeat_string.h"

#include <string_view>

using ::jni::metaprogramming::RepeatString_v;

namespace {

static constexpr std::string_view kLetterA = "A";

static_assert(RepeatString_v<0, kLetterA> == std::string_view{""});
static_assert(RepeatString_v<1, kLetterA> == std::string_view{"A"});
static_assert(RepeatString_v<2, kLetterA> == std::string_view{"AA"});
static_assert(RepeatString_v<3, kLetterA> == std::string_view{"AAA"});
static_assert(RepeatString_v<80, kLetterA> ==
              std::string_view{"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
                               "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"});

static constexpr std::string_view kFoo = "Foo";

static_assert(RepeatString_v<0, kFoo> == std::string_view{""});
static_assert(RepeatString_v<1, kFoo> == std::string_view{"Foo"});
static_assert(RepeatString_v<2, kFoo> == std::string_view{"FooFoo"});
static_assert(RepeatString_v<3, kFoo> == std::string_view{"FooFooFoo"});
static_assert(
    RepeatString_v<80, kFoo> ==
    std::string_view{
        "FooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooF"
        "ooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFo"
        "oFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFooFoo"
        "FooFooFooFooFooFooFooFooFooFoo"});

}  // namespace
