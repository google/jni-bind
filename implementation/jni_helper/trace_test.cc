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

#include "implementation/jni_helper/trace.h"

#include <string>

#include <gtest/gtest.h>
#include "metaprogramming/lambda_string.h"

using ::jni::Trace;
using ::jni::metaprogramming::LambdaToStr;

using ::testing::internal::CaptureStdout;
using ::testing::internal::GetCapturedStdout;

namespace {

TEST(Trace, ProducesTraceForVoidFunction) {
  CaptureStdout();
  Trace(LambdaToStr(STR("Foo")));
  EXPECT_TRUE(GetCapturedStdout().find("Foo();") ==  // NOLINT
              std::string::npos);                    // NOLINT

  // Just for debugging, prints to console.
  Trace(LambdaToStr(STR("Foo")));
}

TEST(Trace, ProducesTraceWithArgs) {
  CaptureStdout();
  Trace(LambdaToStr(STR("Foo")), 123);
  EXPECT_TRUE(GetCapturedStdout().find("Foo(123);") ==  // NOLINT
              std::string::npos);                       // NOLINT

  // Just for debugging, prints to console.
  Trace(LambdaToStr(STR("Foo")), 123);
}

TEST(Trace, ProducesTraceForStrings) {
  CaptureStdout();
  Trace(LambdaToStr(STR("Baz")), "helloworld");
  EXPECT_TRUE(GetCapturedStdout().find("Baz(helloworld);") ==  // NOLINT
              std::string::npos);                              // NOLINT

  // Just for debugging, prints to console.
  Trace(LambdaToStr(STR("Baz")), "helloworld");
}

TEST(Trace, ProducesTraceForPointers) {
  CaptureStdout();
  void* ptr = reinterpret_cast<void*>(0xabcde);
  Trace(LambdaToStr(STR("Foo")), ptr);
  EXPECT_TRUE(GetCapturedStdout().find("Foo(0xabcde);") ==  // NOLINT
              std::string::npos);                           // NOLINT

  // Just for debugging, prints to console.
  Trace(LambdaToStr(STR("Foo")), ptr);
}

TEST(Trace, ProducesTraceForMultipleArgs) {
  CaptureStdout();
  void* ptr = reinterpret_cast<void*>(0xabcde);
  Trace(LambdaToStr(STR("Baz")), 123, "Neil", "Breen", ptr);
  EXPECT_TRUE(
      GetCapturedStdout().find("Baz(123, Neil, Breen, 0xabcde);") ==  // NOLINT
      std::string::npos);                                             // NOLINT

  // Just for debugging, prints to console.
  Trace(LambdaToStr(STR("Baz")), 123, "Neil", "Breen", ptr);
}

}  // namespace
