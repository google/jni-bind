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

#ifndef JNI_BIND_JNI_HELPER_JNI_TYPENAME_TO_STRING_H_
#define JNI_BIND_JNI_HELPER_JNI_TYPENAME_TO_STRING_H_

#include <string_view>

#include "jni_dep.h"

namespace jni {

// Translates a single JNI term (e.g. jint -> "I") as if it were being used as a
// parameter to a method.
//
// Note, the context a parameter is used on occasion will alter a signature,
// e.g. void in a return is explicit, whereas when used as a parameter, it is
// represented as the omission of any value.
//
// Additionally, Android will obnoxiously fail to compile the standard looking:
//    static constexpr char kStr[] = "SomeString";
//
// Because it is against style to import have a using declaration header wide,
// but these are also template definitions, they must remain in this header, and
// so there are goofy looking "using literal" declarations throughout.
//
// https://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/types.html
//
// TODO:  Rename to JavaPrimitiveTypeToString
template <typename JavaType>
constexpr std::string_view JavaTypeToString();

template <>
constexpr std::string_view JavaTypeToString<void>() {
  // Note:  This only applies when used as a return, not as a parameter.  This
  // could be enforced through type system, but maybe feels excessive to do so.
  // For now, enforcing this is unnecesssary, as this function is only called
  // for each Param, which, in the case of no params, is 0 times.
  using namespace std::literals;
  return "V"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jboolean>() {
  using namespace std::literals;
  return "Z"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jbyte>() {
  using namespace std::literals;
  return "B"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jchar>() {
  using namespace std::literals;
  return "C"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jshort>() {
  using namespace std::literals;
  return "S"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jint>() {
  using namespace std::literals;
  return "I"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jlong>() {
  using namespace std::literals;
  return "J"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jfloat>() {
  using namespace std::literals;
  return "F"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jdouble>() {
  using namespace std::literals;
  return "D"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jstring>() {
  using namespace std::literals;
  return "Ljava/lang/String;"sv;
}

}  // namespace jni

#endif  // JNI_BIND_JNI_HELPER_JNI_TYPENAME_TO_STRING_H_
