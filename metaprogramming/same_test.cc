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

#include "same.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using ::jni::metaprogramming::Same;

static_assert(std::is_same_v<Same<int>::type<int>, std::true_type>);
static_assert(std::is_same_v<Same<float>::type<float>, std::true_type>);
static_assert(std::is_same_v<Same<float>::type<int>, std::false_type>);

static_assert(std::is_same_v<Same<int, int>::type<int, int>, std::true_type>);
static_assert(
    std::is_same_v<Same<float, float>::type<float, float>, std::true_type>);
static_assert(
    std::is_same_v<Same<int, float>::type<int, float>, std::true_type>);
static_assert(
    std::is_same_v<Same<float, float>::type<int, float>, std::false_type>);

}  // namespace
