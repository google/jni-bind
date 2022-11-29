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

#include "cartesian_product.h"

#include <type_traits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "invoke.h"
#include "n_bit.h"
#include "n_bit_sequence.h"

namespace {

using jni::metaprogramming::CartesianProduct;
using jni::metaprogramming::CartesianProduct_t;
using jni::metaprogramming::GenerateBitSequenceFromTupSets_t;
using jni::metaprogramming::InvokeAsPack_t;
using jni::metaprogramming::NBit;
using jni::metaprogramming::NBitSequence;
using jni::metaprogramming::TupleUnroller_t;

template <typename... Ts>
using Tup = std::tuple<Ts...>;

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};
struct F {};

////////////////////////////////////////////////////////////////////////////////
// NBit Generation (used to build Cartesian products).
////////////////////////////////////////////////////////////////////////////////

// No sequence requires no nbits to represent.
static_assert(
    std::is_same_v<GenerateBitSequenceFromTupSets_t<>, NBitSequence<>>);

// A single nBit with a single value (which is alwyas 0) can represent "A".
static_assert(std::is_same_v<GenerateBitSequenceFromTupSets_t<std::tuple<A>>,
                             NBitSequence<NBit<0, 0>>>);
// A single nBit with a maximum value of 1 can represent "A,B".
static_assert(std::is_same_v<GenerateBitSequenceFromTupSets_t<std::tuple<A, B>>,
                             NBitSequence<NBit<0, 1>>>);
// Two nBits with a maximum value of 0 and 1 can represent "{A, B} and {A,C}".
static_assert(std::is_same_v<
              GenerateBitSequenceFromTupSets_t<std::tuple<A>, std::tuple<B, C>>,
              NBitSequence<NBit<0, 0>, NBit<0, 1>>>);
// Three nbits etc...
static_assert(
    std::is_same_v<GenerateBitSequenceFromTupSets_t<
                       std::tuple<A, B, C>, std::tuple<D, E>, std::tuple<F>>,
                   NBitSequence<NBit<0, 2>, NBit<0, 1>, NBit<0, 0>>>);

////////////////////////////////////////////////////////////////////////////////
// Cartesian Product
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same_v<std::tuple<>, CartesianProduct_t<>>);

static_assert(std::is_same_v<std::tuple<>,
                             InvokeAsPack_t<CartesianProduct, std::tuple<>>>);

static_assert(
    std::is_same_v<std::tuple<>,
                   InvokeAsPack_t<CartesianProduct, std::tuple<std::tuple<>>>>);
static_assert(
    std::is_same_v<Tup<Tup<A>>, InvokeAsPack_t<CartesianProduct, Tup<Tup<A>>>>);
static_assert(
    std::is_same_v<Tup<Tup<A, B>>,
                   InvokeAsPack_t<CartesianProduct, Tup<Tup<A>, Tup<B>>>>);
static_assert(
    std::is_same_v<Tup<Tup<A, B>, Tup<A, C>>,
                   InvokeAsPack_t<CartesianProduct, Tup<Tup<A>, Tup<B, C>>>>);

static_assert(std::is_same_v<CartesianProduct_t<Tup<>>, Tup<>>);
static_assert(std::is_same_v<CartesianProduct_t<Tup<A>>, std::tuple<Tup<A>>>);

static_assert(
    std::is_same_v<CartesianProduct_t<Tup<A>, Tup<B>>, Tup<Tup<A, B>>>);
static_assert(std::is_same_v<CartesianProduct_t<Tup<A, B>, Tup<C>>,
                             Tup<Tup<A, C>, Tup<B, C>>>);

static_assert(std::is_same_v<CartesianProduct_t<Tup<A>, Tup<B>, Tup<C>>,
                             Tup<Tup<A, B, C>>>);
static_assert(std::is_same_v<CartesianProduct_t<Tup<A, B>, Tup<C>, Tup<D>>,
                             Tup<Tup<A, C, D>, Tup<B, C, D>>>);
static_assert(std::is_same_v<CartesianProduct_t<Tup<A, B, C>, Tup<D>, Tup<E>>,
                             Tup<Tup<A, D, E>, Tup<B, D, E>, Tup<C, D, E>>>);
static_assert(
    std::is_same_v<CartesianProduct_t<Tup<A, B, C>, Tup<D, E>, Tup<F>>,
                   Tup<Tup<A, D, F>, Tup<A, E, F>, Tup<B, D, F>, Tup<B, E, F>,
                       Tup<C, D, F>, Tup<C, E, F>>>);

static_assert(std::is_same_v<CartesianProduct_t<Tup<>, Tup<A>>, Tup<>>);
static_assert(std::is_same_v<CartesianProduct_t<Tup<A>, Tup<>>, Tup<>>);
static_assert(std::is_same_v<CartesianProduct_t<Tup<>, Tup<>, Tup<>>, Tup<>>);
static_assert(std::is_same_v<CartesianProduct_t<Tup<A>, Tup<>, Tup<>>, Tup<>>);
static_assert(std::is_same_v<CartesianProduct_t<Tup<>, Tup<>, Tup<C>>, Tup<>>);

}  // namespace
