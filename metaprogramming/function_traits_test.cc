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

#include "function_traits.h"

#include <gtest/gtest.h>

namespace {

using jni::metaprogramming::FunctionTraitsArg_t;
using jni::metaprogramming::FunctionTraitsReturn_t;

void VFoo() {}
void VFoo_I(int) {}
void VFoo_II(int, int) {}
void VFoo_III(int, int, int) {}

TEST(FunctionTraits, VoidFunctionWithIntArgs) {
  static_assert(std::is_same_v<FunctionTraitsReturn_t<decltype(VFoo)>, void>);

  static_assert(std::is_same_v<FunctionTraitsArg_t<decltype(VFoo_I), 0>, int>);

  static_assert(std::is_same_v<FunctionTraitsArg_t<decltype(VFoo_II), 0>, int>);
  static_assert(std::is_same_v<FunctionTraitsArg_t<decltype(VFoo_II), 1>, int>);

  static_assert(
      std::is_same_v<FunctionTraitsArg_t<decltype(VFoo_III), 0>, int>);
  static_assert(
      std::is_same_v<FunctionTraitsArg_t<decltype(VFoo_III), 1>, int>);
  static_assert(
      std::is_same_v<FunctionTraitsArg_t<decltype(VFoo_III), 2>, int>);
}

void VFoo_F(float) {}
void VFoo_FF(float, float) {}
void VFoo_FFF(float, float, float) {}

TEST(FunctionTraits, VoidFunctionWithFloatArgs) {
  static_assert(std::is_same_v<FunctionTraitsReturn_t<decltype(VFoo)>, void>);

  static_assert(
      std::is_same_v<FunctionTraitsArg_t<decltype(VFoo_F), 0>, float>);

  static_assert(
      std::is_same_v<FunctionTraitsArg_t<decltype(VFoo_FF), 0>, float>);
  static_assert(
      std::is_same_v<FunctionTraitsArg_t<decltype(VFoo_FF), 1>, float>);

  static_assert(
      std::is_same_v<FunctionTraitsArg_t<decltype(VFoo_FFF), 0>, float>);
  static_assert(
      std::is_same_v<FunctionTraitsArg_t<decltype(VFoo_FFF), 1>, float>);
  static_assert(
      std::is_same_v<FunctionTraitsArg_t<decltype(VFoo_FFF), 2>, float>);
}

struct A {};

std::pair<float, double> Complex(std::string, int, A, double, double) {
  return {};
}

TEST(FunctionTraits, ComplexFunctionTypes) {
  using Complex_t = decltype(Complex);
  static_assert(std::is_same_v<FunctionTraitsReturn_t<Complex_t>,
                               std::pair<float, double>>);
  static_assert(std::is_same_v<FunctionTraitsArg_t<Complex_t, 0>, std::string>);
  static_assert(std::is_same_v<FunctionTraitsArg_t<Complex_t, 1>, int>);
  static_assert(std::is_same_v<FunctionTraitsArg_t<Complex_t, 2>, A>);
  static_assert(std::is_same_v<FunctionTraitsArg_t<Complex_t, 3>, double>);
  static_assert(std::is_same_v<FunctionTraitsArg_t<Complex_t, 4>, double>);
}

TEST(FunctionTraits, ComplexFunctionTypeFunctionPointers) {
  auto* func_ptr = &Complex;
  using Complex_t = decltype(func_ptr);

  static_assert(std::is_same_v<FunctionTraitsReturn_t<Complex_t>,
                               std::pair<float, double>>);
  static_assert(std::is_same_v<FunctionTraitsArg_t<Complex_t, 0>, std::string>);
  static_assert(std::is_same_v<FunctionTraitsArg_t<Complex_t, 1>, int>);
  static_assert(std::is_same_v<FunctionTraitsArg_t<Complex_t, 2>, A>);
  static_assert(std::is_same_v<FunctionTraitsArg_t<Complex_t, 3>, double>);
  static_assert(std::is_same_v<FunctionTraitsArg_t<Complex_t, 4>, double>);
}

class ClassWithFunctions {
 public:
  void VFooI(int) {}
  static void Static_VFooI(int) {}
};

TEST(FunctionTraits, MemberFunctions) {
  // Non-Static member function test.
  static_assert(
      std::is_same_v<
          FunctionTraitsReturn_t<decltype(&ClassWithFunctions::VFooI)>, void>);
  static_assert(std::is_same_v<
                FunctionTraitsArg_t<decltype(&ClassWithFunctions::VFooI), 0>,
                ClassWithFunctions&>);
  static_assert(
      std::is_same_v<
          FunctionTraitsArg_t<decltype(&ClassWithFunctions::VFooI), 1>, int>);

  // Static member function test.
  static_assert(
      std::is_same_v<
          FunctionTraitsReturn_t<decltype(&ClassWithFunctions::Static_VFooI)>,
          void>);
  static_assert(
      std::is_same_v<
          FunctionTraitsArg_t<decltype(&ClassWithFunctions::Static_VFooI), 0>,
          int>);
}

TEST(FunctionTraits,
     UseFunctionsSoCompilerDoesntEmitWarningsAboutDiscardingThem) {
  VFoo();
  VFoo_I(1);
  VFoo_II(1, 2);
  VFoo_III(1, 2, 3);

  VFoo_F(1.f);
  VFoo_FF(1.f, 2.f);
  VFoo_FFF(1.f, 2.f, 3.f);
}

}  // namespace
