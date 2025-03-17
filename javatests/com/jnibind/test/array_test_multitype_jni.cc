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
#include <memory>
#include <utility>

#include "jni_bind.h"

using ::jni::ArrayView;
using ::jni::LocalArray;
using ::jni::ArrayAccessMode;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL 
Java_com_jnibind_test_ArrayTestMultitype_jniTearDown(JNIEnv* env, jclass) {
  jvm = nullptr;
}

// Byte array processing with regular API
JNIEXPORT jbyteArray JNICALL
Java_com_jnibind_test_ArrayTestMultitype_nativeProcessByteArrayRegular(
    JNIEnv* env, jobject, jbyteArray input_array) {
  
  // Create a local wrapper for the input array
  LocalArray<jbyte> input{input_array};
  std::size_t length = input.Length();
  
  // Create a new array for the output
  LocalArray<jbyte> output{length};
  
  // Process the array using regular access mode
  {
    auto input_view = input.Pin(true, ArrayAccessMode::REGULAR);
    auto output_view = output.Pin(true, ArrayAccessMode::REGULAR);
    
    // Double each value
    for (std::size_t i = 0; i < length; i++) {
      output_view.ptr()[i] = input_view.ptr()[i] * 2;
    }
  }
  
  // Convert to a Java array reference and ensure it's retained
  jbyteArray result = static_cast<jbyteArray>(output);
  // Create a local reference to prevent early GC
  jbyteArray local_ref = static_cast<jbyteArray>(env->NewLocalRef(result));
  return local_ref;
}

// Byte array processing with critical API
JNIEXPORT jbyteArray JNICALL
Java_com_jnibind_test_ArrayTestMultitype_nativeProcessByteArrayCritical(
    JNIEnv* env, jobject, jbyteArray input_array) {
  
  // Create a local wrapper for the input array
  LocalArray<jbyte> input{input_array};
  std::size_t length = input.Length();
  
  // Create a new array for the output
  LocalArray<jbyte> output{length};
  
  // Process the array using critical access mode
  {
    auto input_view = input.PinCritical();
    auto output_view = output.PinCritical(true);
    
    // Triple each value
    for (std::size_t i = 0; i < length; i++) {
      output_view.ptr()[i] = input_view.ptr()[i] * 3;
    }
  }
  
  // Convert to a Java array reference and ensure it's retained
  jbyteArray result = static_cast<jbyteArray>(output);
  // Create a local reference to prevent early GC
  jbyteArray local_ref = static_cast<jbyteArray>(env->NewLocalRef(result));
  return local_ref;
}

// Float array processing with regular API
JNIEXPORT jfloatArray JNICALL
Java_com_jnibind_test_ArrayTestMultitype_nativeProcessFloatArrayRegular(
    JNIEnv* env, jobject, jfloatArray input_array) {
  
  // Create a local wrapper for the input array
  LocalArray<jfloat> input{input_array};
  std::size_t length = input.Length();
  
  // Create a new array for the output
  LocalArray<jfloat> output{length};
  
  // Process the array using regular access mode
  {
    auto input_view = input.Pin(true, ArrayAccessMode::REGULAR);
    auto output_view = output.Pin(true, ArrayAccessMode::REGULAR);
    
    // Double each value
    for (std::size_t i = 0; i < length; i++) {
      output_view.ptr()[i] = input_view.ptr()[i] * 2.0f;
    }
  }
  
  // Convert to a Java array reference and ensure it's retained
  jfloatArray result = static_cast<jfloatArray>(output);
  // Create a local reference to prevent early GC
  jfloatArray local_ref = static_cast<jfloatArray>(env->NewLocalRef(result));
  return local_ref;
}

// Float array processing with critical API
JNIEXPORT jfloatArray JNICALL
Java_com_jnibind_test_ArrayTestMultitype_nativeProcessFloatArrayCritical(
    JNIEnv* env, jobject, jfloatArray input_array) {
  
  // Create a local wrapper for the input array
  LocalArray<jfloat> input{input_array};
  std::size_t length = input.Length();
  
  // Create a new array for the output
  LocalArray<jfloat> output{length};
  
  // Process the array using critical access mode
  {
    auto input_view = input.PinCritical();
    auto output_view = output.PinCritical(true);
    
    // Triple each value
    for (std::size_t i = 0; i < length; i++) {
      output_view.ptr()[i] = input_view.ptr()[i] * 3.0f;
    }
  }
  
  // Convert to a Java array reference and ensure it's retained
  jfloatArray result = static_cast<jfloatArray>(output);
  // Create a local reference to prevent early GC
  jfloatArray local_ref = static_cast<jfloatArray>(env->NewLocalRef(result));
  return local_ref;
}

// Test copy flag behavior - this should copy back changes
JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMultitype_nativeModifyArrayWithCopyFlagTrue(
    JNIEnv* env, jobject, jintArray array) {
  
  LocalArray<jint> arr{array};
  
  // Use copy_on_completion = true
  auto view = arr.Pin(true, ArrayAccessMode::REGULAR);
  
  // Modify the array
  for (std::size_t i = 0; i < arr.Length(); i++) {
    view.ptr()[i] = i * 10;
  }
  
  // View will be released with copy_on_completion = true
}

// Test copy flag behavior - this should NOT copy back changes
JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMultitype_nativeModifyArrayWithCopyFlagFalse(
    JNIEnv* env, jobject, jintArray array) {
  
  LocalArray<jint> arr{array};
  
  // Use copy_on_completion = false
  auto view = arr.Pin(false, ArrayAccessMode::REGULAR);
  
  // Modify the array
  for (std::size_t i = 0; i < arr.Length(); i++) {
    view.ptr()[i] = i * 10;
  }
  
  // View will be released with copy_on_completion = false
}

// Process matrix with regular array access
JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMultitype_nativeProcessMatrixRegular(
    JNIEnv* env, jobject, jobjectArray matrix) {
  
  // Create a local wrapper for the matrix
  LocalArray<jfloat, 2> matrix_arr{matrix};
  
  // Process each row of the matrix
  for (std::size_t i = 0; i < matrix_arr.Length(); i++) {
    // Get the row
    auto row = matrix_arr.Get(i);
    
    // Process the row with regular access
    auto row_view = row.Pin(true, ArrayAccessMode::REGULAR);
    
    // Square each element
    for (std::size_t j = 0; j < row.Length(); j++) {
      row_view.ptr()[j] = row_view.ptr()[j] * row_view.ptr()[j];
    }
  }
}

// Process matrix with critical array access where possible
JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMultitype_nativeProcessMatrixWithCritical(
    JNIEnv* env, jobject, jobjectArray matrix) {
  
  // Create a local wrapper for the matrix
  LocalArray<jfloat, 2> matrix_arr{matrix};
  
  // Process each row of the matrix
  for (std::size_t i = 0; i < matrix_arr.Length(); i++) {
    // Get the row
    auto row = matrix_arr.Get(i);
    
    // Process the row with critical access
    auto row_view = row.PinCritical(true);
    
    // Square each element
    for (std::size_t j = 0; j < row.Length(); j++) {
      row_view.ptr()[j] = row_view.ptr()[j] * row_view.ptr()[j];
    }
  }
}

} // extern "C"