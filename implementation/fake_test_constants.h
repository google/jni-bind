/*
 * Copyright 2023 Google LLC
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

#ifndef JNI_BIND_FAKE_TEST_CONSTANTS_H_
#define JNI_BIND_FAKE_TEST_CONSTANTS_H_

#include <cstdint>
#include <type_traits>

#include "jni_dep.h"

namespace jni::test {

template <typename T>
struct FakeImpl;

// Produces a fake value of type T with non-trivial default value.
// If modifier is not NONE, will add a modifier.
// Offset can be used to nudge off base value so fake values don't match.
template <typename T,
          typename OffsetT = int>
auto Fake(OffsetT offset = 0) {
  return reinterpret_cast<T>(FakeImpl<std::decay_t<T>>::Val(offset));
}

template <typename T>
struct FakeImpl<T*> {
  static T* Val(int offset) {
    if constexpr (std::is_pointer_v<T>) {
      return reinterpret_cast<T*>(FakeImpl<std::decay_t<T>>::Val(offset));
    } else {
      return reinterpret_cast<T*>(static_cast<intptr_t>(FakeImpl<std::decay_t<T>>::Val(offset)));
    }
  }
};

////////////////////////////////////////////////////////////////////////////////
// JNI C API constants.
////////////////////////////////////////////////////////////////////////////////
template <>
struct FakeImpl<jclass> {
  static jclass Val(int offset) {
    return reinterpret_cast<jclass>(0xABCDEF12345 + offset);
  }
};

template <>
struct FakeImpl<jmethodID> {
  static jmethodID Val(int offset) {
    return reinterpret_cast<jmethodID>(0xBEBEBEBEBEBEBE + offset);
  }
};

template <>
struct FakeImpl<jfieldID> {
  static jfieldID Val(int offset) {
    return reinterpret_cast<jfieldID>(0xDEDEDEDEDEDEDE + offset);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Primitive constants.
////////////////////////////////////////////////////////////////////////////////
template <>
struct FakeImpl<jboolean> {
  static jboolean Val(int) { return false; }
};

template <>
struct FakeImpl<jbyte> {
  static jbyte Val(jbyte offset) { return jbyte{111} + offset; }
};

template <>
struct FakeImpl<jchar> {
  static jchar Val(jchar offset) { return jchar{222} + offset; }
};

template <>
struct FakeImpl<jshort> {
  static jshort Val(jshort offset) { return jshort{333} + offset; }
};

template <>
struct FakeImpl<jint> {
  static jint Val(jint offset) { return jint{444444444} + offset; }
};

template <>
struct FakeImpl<jlong> {
  static jlong Val(jlong offset) { return jlong{0x123456789ABCDE} + offset; }
};

template <>
struct FakeImpl<jfloat> {
  static jfloat Val(jfloat offset) { return jfloat{66666666.f} + offset; }
};

template <>
struct FakeImpl<jdouble> {
  static jdouble Val(jdouble offset) { return jdouble{77777777} + offset; }
};

template <>
struct FakeImpl<jobject> {
  static jobject Val(int offset) {
    return reinterpret_cast<jobject>(0xAAAAABBBBBB + offset);
  }
};

template <>
struct FakeImpl<jstring> {
  static jstring Val(int offset) {
    return reinterpret_cast<jstring>(0xEEEEFFFFF + offset);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Array constants.
////////////////////////////////////////////////////////////////////////////////
template <>
struct FakeImpl<jarray> {
  static jarray Val(int offset) {
    return reinterpret_cast<jarray>(0xCCCCCDDDDDD + offset);
  }
};

template <>
struct FakeImpl<jbooleanArray> {
  static jbooleanArray Val(int offset) {
    return reinterpret_cast<jbooleanArray>(0xEEEEEFFFFFFF + offset);
  }
};

template <>
struct FakeImpl<jbyteArray> {
  static jbyteArray Val(int offset) {
    return reinterpret_cast<jbyteArray>(0xEEEEEFFFFFFF + offset);
  }
};

template <>
struct FakeImpl<jcharArray> {
  static jcharArray Val(int offset) {
    return reinterpret_cast<jcharArray>(0xEFEFEF0A0A0A + offset);
  }
};

template <>
struct FakeImpl<jshortArray> {
  static jshortArray Val(int offset) {
    return reinterpret_cast<jshortArray>(0xEEEEEFFFFFFF + offset);
  }
};

template <>
struct FakeImpl<jintArray> {
  static jintArray Val(int offset) {
    return reinterpret_cast<jintArray>(0xEEEEEFFFFFFF + offset);
  }
};

template <>
struct FakeImpl<jfloatArray> {
  static jfloatArray Val(int offset) {
    return reinterpret_cast<jfloatArray>(0xEEEEEFFFFFFF + offset);
  }
};

template <>
struct FakeImpl<jlongArray> {
  static jlongArray Val(int offset) {
    return reinterpret_cast<jlongArray>(0xEEEEEFFFFFFF + offset);
  }
};

template <>
struct FakeImpl<jdoubleArray> {
  static jdoubleArray Val(int offset) {
    return reinterpret_cast<jdoubleArray>(0xEEEEEFFFFFFF + offset);
  }
};

template <>
struct FakeImpl<jobjectArray> {
  static jobjectArray Val(int offset) {
    return reinterpret_cast<jobjectArray>(0xEEEEEFFFFFFF + offset);
  }
};

}  // namespace jni::test

#endif  // JNI_BIND_FAKE_TEST_CONSTANTS_H_
