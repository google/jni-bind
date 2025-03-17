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

package com.jnibind.test;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.Arrays;
import org.junit.AfterClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Tests for direct/indirect array access with multiple array types. */
@RunWith(JUnit4.class)
public class ArrayTestMultitype {
  static {
    System.load(
        System.getenv("JAVA_RUNFILES")
            + "/_main/javatests/com/jnibind/test/libarray_test_multitype_jni.so");
  }

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  static native void jniTearDown();

  // Byte array tests
  native byte[] nativeProcessByteArrayRegular(byte[] array);
  native byte[] nativeProcessByteArrayCritical(byte[] array);
  
  // Float array tests
  native float[] nativeProcessFloatArrayRegular(float[] array);
  native float[] nativeProcessFloatArrayCritical(float[] array);
  
  // The copy flag tests to check if changes are copied back
  native void nativeModifyArrayWithCopyFlagTrue(int[] array);
  native void nativeModifyArrayWithCopyFlagFalse(int[] array);
  
  // Matrix processing to test multi-dimensional arrays
  native void nativeProcessMatrixRegular(float[][] matrix);
  native void nativeProcessMatrixWithCritical(float[][] matrix);

  @Test
  public void testByteArrayProcessing() {
    byte[] inputArray = new byte[100];
    Arrays.fill(inputArray, (byte)5);
    
    // Test regular API
    byte[] resultRegular = nativeProcessByteArrayRegular(inputArray);
    for (int i = 0; i < resultRegular.length; i++) {
      assertEquals((byte)(5 * 2), resultRegular[i]);
    }
    
    // Test critical API
    byte[] resultCritical = nativeProcessByteArrayCritical(inputArray);
    for (int i = 0; i < resultCritical.length; i++) {
      assertEquals((byte)(5 * 3), resultCritical[i]);
    }
  }

  @Test
  public void testFloatArrayProcessing() {
    float[] inputArray = new float[100];
    Arrays.fill(inputArray, 3.14f);
    
    // Test regular API
    float[] resultRegular = nativeProcessFloatArrayRegular(inputArray);
    for (int i = 0; i < resultRegular.length; i++) {
      assertEquals(3.14f * 2.0f, resultRegular[i], 0.001f);
    }
    
    // Test critical API
    float[] resultCritical = nativeProcessFloatArrayCritical(inputArray);
    for (int i = 0; i < resultCritical.length; i++) {
      assertEquals(3.14f * 3.0f, resultCritical[i], 0.001f);
    }
  }

  @Test
  public void testCopyFlagBehavior() {
    int[] array1 = new int[10];
    int[] array2 = new int[10];
    
    // This should modify the array and copy changes back
    nativeModifyArrayWithCopyFlagTrue(array1);
    
    // This should modify the array but not copy changes back
    nativeModifyArrayWithCopyFlagFalse(array2);
    
    // Check results
    for (int i = 0; i < array1.length; i++) {
      assertEquals(i * 10, array1[i]);
    }
    
    // array2 should still have all zeros
    for (int i = 0; i < array2.length; i++) {
      assertEquals(0, array2[i]);
    }
  }

  @Test
  public void testMatrixProcessing() {
    float[][] matrix = new float[5][5];
    
    // Initialize matrix
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        matrix[i][j] = i + j;
      }
    }
    
    // Make a copy for the second test
    float[][] matrix2 = new float[5][5];
    for (int i = 0; i < 5; i++) {
      System.arraycopy(matrix[i], 0, matrix2[i], 0, 5);
    }
    
    // Process using regular access
    nativeProcessMatrixRegular(matrix);
    
    // Process using critical access where possible
    nativeProcessMatrixWithCritical(matrix2);
    
    // Both matrices should be processed the same way
    for (int i = 0; i < 5; i++) {
      assertArrayEquals(matrix[i], matrix2[i], 0.001f);
    }
  }
}