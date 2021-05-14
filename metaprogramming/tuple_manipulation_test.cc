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

#include "tuple_manipulation.h"

#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

using jni::metaprogramming::ExtractTupleFromType_t;
using jni::metaprogramming::TupleToType_t;

namespace {

// TupleToType takes a tuple and injects it into another type.
struct ComplexType {
  explicit ComplexType(ComplexType&&) = delete;
  ComplexType(const ComplexType&) = delete;
};

static_assert(std::is_same_v<TupleToType_t<std::tuple<int, float>, std::pair>,
                             std::pair<int, float>>);
static_assert(std::is_same_v<TupleToType_t<std::tuple<float, int>, std::pair>,
                             std::pair<float, int>>);
static_assert(
    std::is_same_v<
        TupleToType_t<std::tuple<std::string, float, ComplexType>, std::tuple>,
        std::tuple<std::string, float, ComplexType>>);

// ExtractTupleFromType takes a tuple and injects it into another type.
template <typename... Ts>
struct TypeWithVariadicPack {};

static_assert(std::is_same_v<ExtractTupleFromType_t<std::pair<int, float>>,
                             std::tuple<int, float>>);
static_assert(
    std::is_same_v<ExtractTupleFromType_t<
                       TypeWithVariadicPack<std::string, int, float, int>>,
                   std::tuple<std::string, int, float, int>>);

}  // namespace
