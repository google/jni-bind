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

#include "apply.h"

#include <tuple>
#include <type_traits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "concatenate.h"
#include "invoke.h"
#include "same.h"
#include "tuple_manipulation.h"
#include "type_of_nth_element.h"

using ::jni::metaprogramming::Apply;
using ::jni::metaprogramming::Apply_t;
using ::jni::metaprogramming::Invoke_t;
using ::jni::metaprogramming::InvokePerTupArg_t;
using ::jni::metaprogramming::Same;

namespace {

struct A {};
struct B {};
struct C {};

// Ops can be deferred.
using DeferredOp = Apply<Same>;
static_assert(std::is_same_v<typename DeferredOp::type<A>, Same<A>>);
static_assert(std::is_same_v<Apply_t<Same, A>, Same<A>>);

// And then later invoked.
static_assert(std::is_same_v<Invoke_t<Apply_t<Same, A>, A>, std::true_type>);
static_assert(std::is_same_v<Invoke_t<Apply_t<Same, A>, B>, std::false_type>);

// Arguments to metafunctions can also be deferred.
// This reads as:  "Is A the same as A then B, then C".
using SampleTypes = std::tuple<A, B, C>;
using Operator = Apply<Same>;
static_assert(std::is_same_v<
              InvokePerTupArg_t<Invoke_t<Operator, A>, SampleTypes>,
              std::tuple<std::true_type, std::false_type, std::false_type>>);

}  // namespace
