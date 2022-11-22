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

#include "type_of_nth_element.h"

#include <tuple>
#include <type_traits>

using jni::metaprogramming::TypeOfNthElement_t;
using jni::metaprogramming::TypeOfNthTupleElement_t;

namespace {

static_assert(std::is_same_v<TypeOfNthElement_t<0, int>, int>);
static_assert(std::is_same_v<TypeOfNthElement_t<0, int&>, int&>);
static_assert(std::is_same_v<TypeOfNthElement_t<0, int&&>, int&&>);
static_assert(std::is_same_v<TypeOfNthElement_t<0, const int&>, const int&>);
static_assert(std::is_same_v<TypeOfNthElement_t<0, const int&&>, const int&&>);
static_assert(std::is_same_v<TypeOfNthElement_t<0, int, float, double>, int>);
static_assert(std::is_same_v<TypeOfNthElement_t<1, int, float, double>, float>);
static_assert(
    std::is_same_v<TypeOfNthElement_t<2, int, float, double>, double>);

static_assert(std::is_same_v<TypeOfNthTupleElement_t<0, std::tuple<int>>, int>);
static_assert(std::is_same_v<
              TypeOfNthTupleElement_t<0, std::tuple<int, float, double>>, int>);
static_assert(
    std::is_same_v<TypeOfNthTupleElement_t<1, std::tuple<int, float, double>>,
                   float>);
static_assert(
    std::is_same_v<TypeOfNthTupleElement_t<2, std::tuple<int, float, double>>,
                   double>);

static_assert(
    std::is_same_v<TypeOfNthTupleElement_t<
                       0, std::tuple<std::tuple<int>, std::tuple<float>>>,
                   std::tuple<int>>);

static_assert(
    std::is_same_v<TypeOfNthTupleElement_t<
                       1, std::tuple<std::tuple<int>, std::tuple<float>>>,
                   std::tuple<float>>);

}  // namespace
