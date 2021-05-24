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

#include "params.h"

#include "class.h"
#include "local_object.h"
#include "local_string.h"
#include "mock_jni_env.h"
#include "object_ref.h"
#include "string.h"
#include <gtest/gtest.h>

namespace {

using jni::Params;

TEST(Params, ReturnsCorrectSignatureFromPrimitiveParams) {
  constexpr Params void_params{};
  constexpr Params<jint> int_params{};
  constexpr Params double_params{jdouble{}};

  EXPECT_EQ(void_params.GetSignature(), "()");
  EXPECT_EQ(int_params.GetSignature(), "(I)");
  EXPECT_EQ(double_params.GetSignature(), "(D)");
}

TEST(Params, ReturnsCorrectSignatureFromObjectParams) {
  static constexpr jni::Class c1{"Class1"};
  constexpr Params object_params{c1};

  EXPECT_EQ(object_params.GetSignature(), "(LClass1;)");
}

TEST(Params, SupportStrings) {
  constexpr Params<jstring> string_params;

  EXPECT_EQ(string_params.GetSignature(), "(Ljava/lang/String;)");
}

}  // namespace
