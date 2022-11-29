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

#include "double_locked_value.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <thread>

namespace {

using jni::metaprogramming::DoubleLockedValue;

TEST(DoubleLockedValue, ConstructsTrivialValue) {
  auto a = []() {return 1;};
  DoubleLockedValue<int> double_locked_value{};
  EXPECT_EQ(1, double_locked_value.LoadAndMaybeInit(a));
}

TEST(DoubleLockedValue, HasNoCrossTalkAcrossInstances) {
  auto a = []() {return 1;};
  DoubleLockedValue<int> double_locked_value_1;
  DoubleLockedValue<int> double_locked_value_2;
  DoubleLockedValue<int> double_locked_value_3;

  EXPECT_EQ(1, double_locked_value_1.LoadAndMaybeInit(a));
  EXPECT_EQ(1, double_locked_value_2.LoadAndMaybeInit(a));
  EXPECT_EQ(1, double_locked_value_3.LoadAndMaybeInit(a));
}

TEST(DoubleLockedValue, AllowsNonTrivialInitLambdas) {
  int a = 123;
  DoubleLockedValue<int> double_locked_value;
  EXPECT_EQ(123, double_locked_value.LoadAndMaybeInit([a]() {return a;}));
}

TEST(DoubleLockedValue, DoesntDoExpensiveInitialisationTwice){
  int a = 1;
  auto lambda {[&]() {return a++;}};
  DoubleLockedValue<int> double_locked_value;
  EXPECT_EQ(1, double_locked_value.LoadAndMaybeInit(lambda));
  EXPECT_EQ(1, double_locked_value.LoadAndMaybeInit(lambda));
  EXPECT_EQ(1, double_locked_value.LoadAndMaybeInit(lambda));
}

TEST(DoubleLockedValue, PassesTheCorrectValueOnTeardown){
  DoubleLockedValue<int> double_locked_value;
  EXPECT_EQ(123, double_locked_value.LoadAndMaybeInit([](){return 123;}));
  double_locked_value.Reset([](int teardown_val) {
    EXPECT_EQ(teardown_val, 123);
  });
}

TEST(DoubleLockedValue, DoesExpensiveInitialisationOnlyTwice){
  int a = 1;
  auto lambda {[&]() {return a++;}};
  DoubleLockedValue<int> double_locked_value;
  EXPECT_EQ(1, double_locked_value.LoadAndMaybeInit(lambda));
  EXPECT_EQ(1, double_locked_value.LoadAndMaybeInit(lambda));
  EXPECT_EQ(1, double_locked_value.LoadAndMaybeInit(lambda));
double_locked_value.Reset();
  EXPECT_EQ(2, double_locked_value.LoadAndMaybeInit(lambda));
  EXPECT_EQ(2, double_locked_value.LoadAndMaybeInit(lambda));
  EXPECT_EQ(2, double_locked_value.LoadAndMaybeInit(lambda));
}

TEST(DoubleLockedValue, MixesResetWithAndWithoutLambda){
  DoubleLockedValue<int> double_locked_value;

  // First time the value is loaded.
  EXPECT_EQ(123, double_locked_value.LoadAndMaybeInit([](){return 123;}));
  // Second time, the value is already stored.
  EXPECT_EQ(123, double_locked_value.LoadAndMaybeInit([](){return -1;}));
  // This value is used for the teardown lambda.
  double_locked_value.Reset([](int teardown_val) {
    EXPECT_EQ(teardown_val, 123);
  });
  // The teardown lambda remains uncalled because the value is already reset.
  double_locked_value.Reset([](int teardown_val) {
    FAIL();
  });
  // The value is updated after a reset.
  EXPECT_EQ(456, double_locked_value.LoadAndMaybeInit([](){return 456;}));
  // Using a reset without a lambda silently sets to 0.
  double_locked_value.Reset();
  // The value is 0 and so the teardown lambda is not invoked.
  double_locked_value.Reset([](int teardown_val) {
    FAIL();
  });
}

}  // namespace
