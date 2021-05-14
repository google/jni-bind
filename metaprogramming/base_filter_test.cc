// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "base_filter.h"

#include <tuple>
#include <type_traits>

using jni::metaprogramming::BaseFilter_t;

namespace {

struct S {};
struct S1 {};
struct S2 {};

struct D : S {};
struct D2 : D {};
struct D3 : D2 {};

static_assert(std::is_base_of_v<S, S>);

static_assert(std::is_same_v<BaseFilter_t<S>, std::tuple<>>);
static_assert(std::is_same_v<BaseFilter_t<S, S>, std::tuple<S>>);
static_assert(std::is_same_v<BaseFilter_t<S, S, S1>, std::tuple<S>>);
static_assert(std::is_same_v<BaseFilter_t<S, S, S1, S2>, std::tuple<S>>);
static_assert(std::is_same_v<BaseFilter_t<S, S, S1, S>, std::tuple<S, S>>);

static_assert(std::is_same_v<BaseFilter_t<S, D>, std::tuple<D>>);
static_assert(std::is_same_v<BaseFilter_t<S, D2>, std::tuple<D2>>);
static_assert(std::is_same_v<BaseFilter_t<S, D3>, std::tuple<D3>>);

static_assert(std::is_same_v<BaseFilter_t<S, S, D, D2, D3, S1, S2>,
                             std::tuple<S, D, D2, D3>>);

}  // namespace
