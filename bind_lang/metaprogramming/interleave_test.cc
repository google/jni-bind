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

#include "interleave.h"

#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

using ::jni::metaprogramming::Interleaved;

namespace {

// https://stackoverflow.com/questions/33988297/deleted-default-constructor-objects-can-still-be-created-sometimes
// Privately inheriting from DummyBase guarantees all types will definitely not
// be aggregates which can have surprising initialisation and cause tests to
// give false positives on tests.
struct DummyBase {};

TEST(TypeInterleave, Simple) {
  std::tuple t1 = {int{}, int{}, int{}};
  std::tuple t2 = {1.f, 2.f, 3.f};

  static_assert(std::is_same_v<Interleaved<decltype(t1), decltype(t2)>::type,
                               std::tuple<int, float, int, float, int, float>>);
}

TEST(TypeInterleave, NoCopyCtor) {
  struct Complex : private DummyBase {
    Complex() = default;
    Complex(const Complex&) = delete;
    Complex(Complex&&) = default;
  };

  std::tuple t1 = {Complex{}, Complex{}, Complex{}};
  std::tuple t2 = {1.f, 2.f, 3.f};

  static_assert(std::is_same_v<
                Interleaved<decltype(t1), decltype(t2)>::type,
                std::tuple<Complex, float, Complex, float, Complex, float>>);
}

TEST(TypeInterleave, NoDefaultCtor) {
  struct Complex : private DummyBase {
    Complex() = delete;
    Complex(int, int) {}
  };

  static_assert(std::is_same_v<
                Interleaved<std::tuple<Complex, Complex, Complex>,
                            std::tuple<float, float, float>>::type,
                std::tuple<Complex, float, Complex, float, Complex, float>>);
}

}  // namespace
