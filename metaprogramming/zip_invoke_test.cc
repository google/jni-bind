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

#include "zip_invoke.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "all.h"
#include "base.h"
#include "increment.h"
#include "per_element.h"
#include "same.h"
#include "unwrap.h"

namespace {

using ::jni::metaprogramming::All_v;
using ::jni::metaprogramming::Same;
using ::jni::metaprogramming::ZipInvoke_t;

struct A {};
struct B : A {};
struct C {};

static_assert(std::is_same_v<ZipInvoke_t<Same, std::tuple<>, std::tuple<>>,
                             std::tuple<>>);

static_assert(std::is_same_v<ZipInvoke_t<Same, std::tuple<A>, std::tuple<A>>,
                             std::tuple<std::true_type>>);

static_assert(
    std::is_same_v<ZipInvoke_t<Same, std::tuple<A, B>, std::tuple<A, B>>,
                   std::tuple<std::true_type, std::true_type>>);
static_assert(
    std::is_same_v<ZipInvoke_t<Same, std::tuple<A, B, C>, std::tuple<A, B, C>>,
                   std::tuple<std::true_type, std::true_type, std::true_type>>);
static_assert(std::is_same_v<
              ZipInvoke_t<Same, std::tuple<A, A, A>, std::tuple<A, B, C>>,
              std::tuple<std::true_type, std::false_type, std::false_type>>);

}  // namespace
