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

#ifndef JNI_BIND_IMPLEMENTATION_JNI_HELPER_ARG_STRING_H_
#define JNI_BIND_IMPLEMENTATION_JNI_HELPER_ARG_STRING_H_

#include <sstream>
#include <string>

namespace jni {

// Converts a single argument into a std::string (useful for debugging).
template <typename T>
struct ArgStringify {
  static std::string Str(const T& val) {
    std::stringstream ss;
    ss << val;

    return ss.str();
  }
};

template <typename T>
struct ArgStringify<T*> {
  static std::string Str(const T* val) {
    if (val == nullptr) {
      return "nullptr";
    }

    std::stringstream ss;
    ss << val;

    return ss.str();
  }
};

// Helper function to select correct ArgString partial specialization.
template <typename T>
std::string ArgString(const T& val) {
  return ArgStringify<T>::Str(val);
}

////////////////////////////////////////////////////////////////////////////////
// Void.
////////////////////////////////////////////////////////////////////////////////
template <>
struct ArgStringify<void> {
  static std::string Str() { return ""; }
};

inline std::string ArgString() { return ArgStringify<void>::Str(); }

////////////////////////////////////////////////////////////////////////////////
// Bool.
////////////////////////////////////////////////////////////////////////////////
template <>
struct ArgStringify<bool> {
  static std::string Str(const bool val) { return val ? "TRUE" : "FALSE"; }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_HELPER_ARG_STRING_H_
