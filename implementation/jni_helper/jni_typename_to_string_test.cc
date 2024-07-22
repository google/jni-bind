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

#include "jni_typename_to_string.h"

#include <string_view>  // NOLINT

#include <gtest/gtest.h>
#include "jni_dep.h"

namespace {

using ::jni::JavaTypeToString;

TEST(TypeToString, CompilesBasicTypeTests) {
  using namespace std::literals;
  static_assert(JavaTypeToString<void>() == "V"sv);
  static_assert(JavaTypeToString<jboolean>() == "Z"sv);
  static_assert(JavaTypeToString<jchar>() == "C"sv);
  static_assert(JavaTypeToString<jbyte>() == "B"sv);
  static_assert(JavaTypeToString<jshort>() == "S"sv);
  static_assert(JavaTypeToString<jint>() == "I"sv);
  static_assert(JavaTypeToString<jlong>() == "J"sv);
  static_assert(JavaTypeToString<jfloat>() == "F"sv);
  static_assert(JavaTypeToString<jdouble>() == "D"sv);
  static_assert(JavaTypeToString<jstring>() == "Ljava/lang/String;"sv);
}

}  // namespace
