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

#include "signature.h"

#include <string_view>

#include "class.h"
#include "mock_jni_env.h"
#include <gtest/gtest.h>
#include "params.h"

using namespace std::literals;
using jni::SignatureForParameterTuple;
using jni::SignatureForSingleParameter;

namespace {

TEST(Signature, GivesSaneSingleValues) {
  EXPECT_EQ(SignatureForSingleParameter(jint{}), "I");
  EXPECT_EQ(SignatureForSingleParameter(jdouble{}), "D");
  EXPECT_EQ(SignatureForSingleParameter(jfloat{}), "F");
}

TEST(Signature, GivesSaneObjectValues) {
  jni::test::MockJniEnv env;
  static constexpr jni::Class dummy_class{"com/google/Foo"};

  EXPECT_EQ(SignatureForSingleParameter(dummy_class), "Lcom/google/Foo;");
}

TEST(Signature, GivesSaneCompoundValues) {
  EXPECT_EQ(SignatureForParameterTuple(std::tuple<jint, jdouble, jfloat>{}),
            "IDF");
  EXPECT_EQ(
      SignatureForParameterTuple(std::tuple<jboolean, jbyte, jchar, jshort,
                                            jint, jlong, jfloat, jdouble>{}),
      "ZBCSIJFD");
}

}  // namespace
