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

namespace {

using ::jni::metaprogramming::PackDiscriminator_e;
using ::jni::metaprogramming::PackType;

static constexpr bool kVal1 = false;
static constexpr bool kVal2 = false;

struct NonContainer {};

template <typename... Ts>
struct A {};

template <const auto&... Vs>
struct D {};

static_assert(PackDiscriminator_e<int> == PackType::NOT_CONTAINER);
static_assert(PackDiscriminator_e<NonContainer> == PackType::NOT_CONTAINER);

static_assert(PackDiscriminator_e<A<>> == PackType::TYPES);
static_assert(PackDiscriminator_e<A<float>> == PackType::TYPES);
static_assert(PackDiscriminator_e<A<int, float>> == PackType::TYPES);

static_assert(PackDiscriminator_e<D<kVal1>> == PackType::CONST_AUTO_REF);
static_assert(PackDiscriminator_e<D<kVal1, kVal2>> == PackType::CONST_AUTO_REF);

// For |PackDiscriminatedForward| tests, see |Next|.

}  // namespace
