/*
 * Copyright 2024 Google LLC
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

#include "color.h"

#include <cstdio>
#include <string>

#include <gtest/gtest.h>

using ::jni::metaprogramming::Color;
using ::jni::metaprogramming::Colorize;

namespace {

TEST(ColorTest, HasBlue) {
  std::string output = Colorize(Color::kBlue, "Blue words").data();

  printf("%s\n", output.c_str());

  EXPECT_TRUE(Colorize(Color::kBlue, "Blue words").find(Color::kBlue) !=
              std::string::npos);
}

TEST(ColorTest, HasGreenAndDoesntBleed) {
  std::string output = Colorize(Color::kRed, "Red words").data();
  printf("%s, no color.\n", output.c_str());

  EXPECT_TRUE(output.find(Color::kRed) != std::string::npos);
  EXPECT_TRUE(output.find(Color::kNone) != std::string::npos);
}

}  // namespace
