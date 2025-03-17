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

#include <cstddef>
#include <chrono>
#include <memory>
#include <utility>

#include "jni_bind.h"

using ::jni::ArrayView;
using ::jni::LocalArray;
using ::jni::ArrayAccessMode;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

// Helper to calculate a checksum of all array values
int calculateChecksum(jint* ptr, std::size_t length) {
  int sum = 0;
  for (std::size_t i = 0; i < length; i++) {
    sum += ptr[i];
  }
  return sum;
}

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL 
Java_com_jnibind_test_ArrayTestCritical_jniTearDown(JNIEnv* env, jclass) {
  jvm = nullptr;
}

JNIEXPORT jboolean JNICALL
Java_com_jnibind_test_ArrayTestCritical_nativeRegularArrayTest(
    JNIEnv* env, jobject, jintArray int_array) {
  
  LocalArray<jint> arr{int_array};
  
  // Using regular Pin method (default access mode)
  auto view = arr.Pin(true);
  
  // Verify array access
  bool valid = true;
  for (std::size_t i = 0; i < arr.Length() && valid; ++i) {
    valid = (view.ptr()[i] == static_cast<jint>(i));
  }
  
  return valid;
}

JNIEXPORT jboolean JNICALL
Java_com_jnibind_test_ArrayTestCritical_nativeRegularArraySetTest(
    JNIEnv* env, jobject, jintArray int_array) {
  
  LocalArray<jint> arr{int_array};
  
  // Using regular Pin method
  auto view = arr.Pin(true, ArrayAccessMode::REGULAR);
  
  // Set values in the array
  for (std::size_t i = 0; i < arr.Length(); ++i) {
    view.ptr()[i] = static_cast<jint>(i * 2);
  }
  
  return true;
}

JNIEXPORT jboolean JNICALL
Java_com_jnibind_test_ArrayTestCritical_nativeCriticalArrayTest(
    JNIEnv* env, jobject, jintArray int_array) {
  
  LocalArray<jint> arr{int_array};
  
  // Using critical Pin method
  auto view = arr.PinCritical(true);
  
  // Verify array access
  bool valid = true;
  for (std::size_t i = 0; i < arr.Length() && valid; ++i) {
    valid = (view.ptr()[i] == static_cast<jint>(i));
  }
  
  return valid;
}

JNIEXPORT jboolean JNICALL
Java_com_jnibind_test_ArrayTestCritical_nativeCriticalArraySetTest(
    JNIEnv* env, jobject, jintArray int_array) {
  
  LocalArray<jint> arr{int_array};
  
  // Using critical Pin method
  auto view = arr.Pin(true, ArrayAccessMode::CRITICAL); 
  
  // Set values in the array
  for (std::size_t i = 0; i < arr.Length(); ++i) {
    view.ptr()[i] = static_cast<jint>(i * 3);
  }
  
  return true;
}

JNIEXPORT jlong JNICALL
Java_com_jnibind_test_ArrayTestCritical_nativeRegularArrayPerformanceTest(
    JNIEnv* env, jobject, jintArray int_array, jint iterations) {
  
  LocalArray<jint> arr{int_array};
  
  // Use std::chrono for timing
  auto start = std::chrono::high_resolution_clock::now();
  
  int result = 0;
  for (int iter = 0; iter < iterations; iter++) {
    // Get a view of the array
    auto view = arr.Pin(true, ArrayAccessMode::REGULAR);
    
    // Perform a computation (sum of the array)
    result += calculateChecksum(view.ptr(), arr.Length());
  }
  
  auto end = std::chrono::high_resolution_clock::now();
  
  // Prevent compiler optimization
  if (result == -1) {
    return -1;
  }
  
  // Return the time in nanoseconds
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

JNIEXPORT jlong JNICALL
Java_com_jnibind_test_ArrayTestCritical_nativeCriticalArrayPerformanceTest(
    JNIEnv* env, jobject, jintArray int_array, jint iterations) {
  
  LocalArray<jint> arr{int_array};
  
  // Use std::chrono for timing
  auto start = std::chrono::high_resolution_clock::now();
  
  int result = 0;
  for (int iter = 0; iter < iterations; iter++) {
    // Get a critical view of the array
    auto view = arr.PinCritical(true);
    
    // Perform a computation (sum of the array)
    result += calculateChecksum(view.ptr(), arr.Length());
  }
  
  auto end = std::chrono::high_resolution_clock::now();
  
  // Prevent compiler optimization
  if (result == -1) {
    return -1;
  }
  
  // Return the time in nanoseconds
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

JNIEXPORT jboolean JNICALL
Java_com_jnibind_test_ArrayTestCritical_nativeIsArrayDirectTest(
    JNIEnv* env, jobject, jintArray int_array) {
  
  LocalArray<jint> arr{int_array};
  
  // Check both regular and critical access to see if they yield direct access
  bool isDirect = false;
  
  {
    // Regular access
    auto view = arr.Pin(true, ArrayAccessMode::REGULAR);
    // is_copy will be false for direct access
    isDirect = !view.get_array_elements_result_.is_copy;
  }
  
  if (!isDirect) {
    // Try critical access - which is more likely to yield direct access
    auto view = arr.PinCritical(true);
    isDirect = !view.get_array_elements_result_.is_copy;
  }
  
  return isDirect;
}

} // extern "C"