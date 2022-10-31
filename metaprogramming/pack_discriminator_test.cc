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
#include "metaprogramming/pack_discriminator.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using ::jni::metaprogramming::PackDiscriminator_e;
using ::jni::metaprogramming::PackType;

int a = 1;
int b = 1;

static constexpr bool kVal1 = false;
static constexpr bool kVal2 = false;

template <typename... Ts>
struct A {};

template <auto... Vs>
struct B {};

template <auto&... Vs>
struct C {};

template <const auto&... Vs>
struct D {};

// See class comment.
// static_assert(PackDiscriminator_e<B<>> == PackType::AUTO);
// static_assert(PackDiscriminator_e<C<>> == PackType::AUTO_REF);
// static_assert(PackDiscriminator_e<D<>> == PackType::CONST_AUTO_REF);

static_assert(PackDiscriminator_e<A<>> == PackType::TYPES);
static_assert(PackDiscriminator_e<A<float>> == PackType::TYPES);
static_assert(PackDiscriminator_e<A<int, float>> == PackType::TYPES);

static_assert(PackDiscriminator_e<B<0>> == PackType::AUTO);
static_assert(PackDiscriminator_e<B<0, 1>> == PackType::AUTO);

static_assert(PackDiscriminator_e<C<a>> == PackType::AUTO_REF);
static_assert(PackDiscriminator_e<C<a, b>> == PackType::AUTO_REF);

static_assert(PackDiscriminator_e<D<kVal1>> == PackType::CONST_AUTO_REF);
static_assert(PackDiscriminator_e<D<kVal1, kVal2>> == PackType::CONST_AUTO_REF);

}  // namespace
