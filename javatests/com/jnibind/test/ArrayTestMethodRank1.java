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

package com.jnibind.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import org.junit.AfterClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Exercises array functionality arcross rJNI. */
@RunWith(JUnit4.class)
public class ArrayTestMethodRank1 {
  static {
    System.loadLibrary("array_test_method_rank_1_jni");
  }

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  static native void jniTearDown();

  native void nativeBooleanTests(boolean[] intArray);

  native void nativeByteTests(byte[] byteArray);

  native void nativeCharTests(char[] charArray);

  native void nativeShortTests(short[] shortArray);

  native void nativeIntTests(int[] intArray);

  native void nativeLongTests(long[] longArray);

  native void nativeFloatTests(float[] floatArray);

  native void nativeDoubleTests(double[] doubleArray);

  native void nativeStringTests(String[] stringArray);

  native void nativeObjectTests(ObjectTestHelper[] objectArray);

  void booleanArray(boolean testForTrue, boolean[] arr) {
    for (int i = 0; i < arr.length; i++) {
      if (testForTrue) {
        assertTrue(arr[i]);
      } else {
        assertFalse(arr[i]);
      }
    }
  }

  void byteArray(byte baseOffset, byte[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  void charArray(char baseOffset, char[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  void shortArray(short baseOffset, short[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  void intArray(int baseOffset, int[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  void longArray(long baseOffset, long[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  void floatArray(float baseOffset, float[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i], 1.f);
    }
  }

  void doubleArray(double baseOffset, double[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals((double) i + baseOffset, arr[i], 0);
    }
  }

  void stringArray(String[] arr) {
    assertEquals(3, arr.length);
    assertEquals("Foo", arr[0]);
    assertEquals("Baz", arr[1]);
    assertEquals("Bar", arr[2]);
  }

  void objectArray(int objectMemberOffset, ObjectTestHelper[] arr) {
    for (int i = 0; i < arr.length; i++) {
      int idxPlusOffset = i + objectMemberOffset;
      ObjectTestHelper dummyThatMatchesIdx =
          new ObjectTestHelper(idxPlusOffset, idxPlusOffset, idxPlusOffset);
      assertTrue(dummyThatMatchesIdx.isEqualTo(arr[i]));
    }
  }

  void objectArrayArrayOfNulls(ObjectTestHelper[] arr) {
    for (ObjectTestHelper element : arr) {
      assertNull(element);
    }
  }

  @Test
  public void booleanTests() {
    boolean[] arr = {true, true, true};
    nativeBooleanTests(arr);
  }

  @Test
  public void byteTests() {
    byte[] arr = {0, 1, 2, 3, 4, 5};
    nativeByteTests(arr);
  }

  @Test
  public void charTests() {
    char[] arr = {0, 1, 2, 3, 4, 5};
    nativeCharTests(arr);
  }

  @Test
  public void shortTests() {
    short[] arr = {0, 1, 2, 3, 4, 5};
    nativeShortTests(arr);
  }

  @Test
  public void intTests() {
    int[] arr = {0, 1, 2, 3, 4, 5};
    nativeIntTests(arr);
  }

  @Test
  public void longTests() {
    long[] arr = {0, 1, 2, 3, 4, 5};
    nativeLongTests(arr);
  }

  @Test
  public void floatTests() {
    float[] arr = {0, 1, 2, 3, 4, 5};
    nativeFloatTests(arr);
  }

  @Test
  public void doubleTests() {
    double[] arr = {0, 1, 2, 3, 4, 5};
    nativeDoubleTests(arr);
  }

  @Test
  public void stringTests() {
    String[] arr = {"Foo", "Baz", "Bar"};
    nativeStringTests(arr);
  }

  @Test
  public void objectTests() {
    ObjectTestHelper[] objectTestHelper = {
      new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(1, 1, 1), new ObjectTestHelper(2, 2, 2)
    };

    nativeObjectTests(objectTestHelper);
  }
}
