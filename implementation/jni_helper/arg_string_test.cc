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

#include "implementation/jni_helper/arg_string.h"

#include <limits>

#include <gtest/gtest.h>
#include "jni_dep.h"

using ::jni::ArgString;

namespace {

////////////////////////////////////////////////////////////////////////////////
// Void.
////////////////////////////////////////////////////////////////////////////////
TEST(ArgString, Void) { EXPECT_EQ(ArgString(), ""); }

////////////////////////////////////////////////////////////////////////////////
// Primitive Types.
////////////////////////////////////////////////////////////////////////////////
TEST(ArgString, Bool) {
  EXPECT_EQ(ArgString(true), "TRUE");
  EXPECT_EQ(ArgString(false), "FALSE");
}

TEST(ArgString, Char) {
  EXPECT_EQ(ArgString('x'), "x");
  EXPECT_EQ(ArgString('y'), "y");
  EXPECT_EQ(ArgString('z'), "z");
}

TEST(ArgString, Short) {
  EXPECT_EQ(ArgString(jshort{32767}), "32767");
  EXPECT_EQ(ArgString(jshort{-32768}), "-32768");
  EXPECT_EQ(ArgString(static_cast<jshort>(65535)), "-1");  // no unsigned
}

TEST(ArgString, Int) { EXPECT_EQ(ArgString(jint{-2147483648}), "-2147483648"); }

TEST(ArgString, Long) {
  EXPECT_EQ(ArgString(jlong{9223372036854775807}), "9223372036854775807");
}

TEST(ArgString, Float) {
  EXPECT_EQ(ArgString(jfloat{std::numeric_limits<float>::max()}),
            "3.40282e+38");
}

TEST(ArgString, Double) {
  EXPECT_EQ(ArgString(jdouble{std::numeric_limits<double>::max()}),
            "1.79769e+308");
}

////////////////////////////////////////////////////////////////////////////////
// Pointers.
////////////////////////////////////////////////////////////////////////////////
TEST(ArgString, HandlesNullPointers) {
  EXPECT_EQ(ArgString(reinterpret_cast<int*>(0)), "nullptr");
}

TEST(ArgString, Pointers) {
  EXPECT_EQ(ArgString(reinterpret_cast<float*>(0XABCDEF)), "0xabcdef");
}

////////////////////////////////////////////////////////////////////////////////
// Char* (Strings).
////////////////////////////////////////////////////////////////////////////////
TEST(ArgString, NullForEmptyChar) {
  EXPECT_EQ(ArgString(static_cast<char*>(nullptr)), "nullptr");
}

TEST(ArgString, ExplicitLengthArray) {
  EXPECT_EQ(ArgString("fooBaz"),  // Type is char[7].
            "fooBaz");
}

}  // namespace
