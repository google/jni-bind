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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.AfterClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Tests for direct vs. indirect array access. */
@RunWith(JUnit4.class)
public class ArrayTestCritical {
  static {
    System.load(
        System.getenv("JAVA_RUNFILES")
            + "/_main/javatests/com/jnibind/test/libarray_test_critical_jni.so");
  }

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  static native void jniTearDown();

  // Regular array API tests
  native boolean nativeRegularArrayTest(int[] intArray);
  native boolean nativeRegularArraySetTest(int[] intArray);
  
  // Critical array API tests
  native boolean nativeCriticalArrayTest(int[] intArray);
  native boolean nativeCriticalArraySetTest(int[] intArray);
  
  // Performance comparison tests
  native long nativeRegularArrayPerformanceTest(int[] intArray, int iterations);
  native long nativeCriticalArrayPerformanceTest(int[] intArray, int iterations);
  
  // Direct access verification (is the array direct or pinned)
  native boolean nativeIsArrayDirectTest(int[] intArray);

  @Test
  public void testRegularArrayAccess() {
    int[] arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    assertTrue(nativeRegularArrayTest(arr));
  }

  @Test
  public void testRegularArraySet() {
    int[] arr = new int[10];
    boolean success = nativeRegularArraySetTest(arr);
    assertTrue(success);
    
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * 2, arr[i]);
    }
  }

  @Test
  public void testCriticalArrayAccess() {
    int[] arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    assertTrue(nativeCriticalArrayTest(arr));
  }

  @Test
  public void testCriticalArraySet() {
    int[] arr = new int[10];
    boolean success = nativeCriticalArraySetTest(arr);
    assertTrue(success);
    
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * 3, arr[i]);
    }
  }

  @Test
  public void testArrayPerformance() {
    // Create a larger array for performance testing
    int[] arr = new int[10000];
    for (int i = 0; i < arr.length; i++) {
      arr[i] = i;
    }
    
    int iterations = 100;
    
    // Run both tests
    long regularTime = nativeRegularArrayPerformanceTest(arr, iterations);
    long criticalTime = nativeCriticalArrayPerformanceTest(arr, iterations);
    
    // Print the results to the test log
    System.out.println("Regular array access time: " + regularTime + "ns");
    System.out.println("Critical array access time: " + criticalTime + "ns");
    
    // We're not making assertions about which is faster, as it depends
    // on JVM implementation and hardware
  }
  
  @Test
  public void testIsArrayDirect() {
    int[] arr = new int[10];
    
    // We can't make assertions about whether the array is direct or not,
    // as it's JVM implementation dependent, but we can run the test to see
    // what the current JVM returns
    boolean isDirect = nativeIsArrayDirectTest(arr);
    System.out.println("Array is direct: " + isDirect);
  }
}