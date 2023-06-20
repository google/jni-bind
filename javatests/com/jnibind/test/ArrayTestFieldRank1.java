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
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;

import org.junit.AfterClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Exercises array functionality arcross rJNI. */
@RunWith(JUnit4.class)
public class ArrayTestFieldRank1 {
  static {
    System.loadLibrary("array_test_field_rank_1_jni");
  }

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  static native void jniTearDown();

  public boolean[] booleanArrayField = {false, false, false};
  byte[] byteArrayField = {0, 0, 0, 0, 0, 0};
  char[] charArrayField = {0, 0, 0, 0, 0, 0};
  short[] shortArrayField = {0, 0, 0, 0, 0, 0};
  int[] intArrayField = {0, 0, 0, 0, 0, 0};
  long[] longArrayField = {0, 0, 0, 0, 0, 0};
  float[] floatArrayField = {0, 0, 0, 0, 0, 0};
  double[] doubleArrayField = {0, 0, 0, 0, 0, 0};

  String[] stringArrayField = {"BAD", "BAD", "BAD"};
  ObjectTestHelper[] objectArrayField = {
    new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(1, 1, 1)
  };

  native void nativeBooleanTests(ArrayTestFieldRank1 arrayTest, boolean[] intArray);

  native void nativeByteTests(ArrayTestFieldRank1 arrayTest, byte[] byteArray);

  native void nativeCharTests(ArrayTestFieldRank1 arrayTest, char[] charArray);

  native void nativeShortTests(ArrayTestFieldRank1 arrayTest, short[] shortArray);

  native void nativeIntTests(ArrayTestFieldRank1 arrayTest, int[] intArray);

  native void nativeLongTests(ArrayTestFieldRank1 arrayTest, long[] longArray);

  native void nativeFloatTests(ArrayTestFieldRank1 arrayTest, float[] floatArray);

  native void nativeDoubleTests(ArrayTestFieldRank1 arrayTest, double[] doubleArray);

  native void nativeStringTests(ArrayTestFieldRank1 arrayTest, String[] stringArray);

  native void nativeObjectTests(ArrayTestFieldRank1 arrayTest, ObjectTestHelper[] objectArray);

  void booleanArray(boolean baseOffset, boolean stride, boolean[] arr) {
    // Booleans stride up until true (i.e. once) and then red line.
    // This isn't intuitive, but allows for generalised testing.
    for (int i = 0; i < arr.length; i++) {
      if (baseOffset || (i > 0 && stride)) {
        assertTrue(arr[i]);
      } else {
        assertFalse(arr[i]);
      }
    }
  }

  void byteArray(byte baseOffset, byte stride, byte[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * stride + baseOffset, arr[i]);
    }
  }

  void charArray(char baseOffset, char stride, char[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * stride + baseOffset, arr[i]);
    }
  }

  void shortArray(short baseOffset, short stride, short[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * stride + baseOffset, arr[i]);
    }
  }

  void intArray(int baseOffset, int stride, int[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * stride + baseOffset, arr[i]);
    }
  }

  void longArray(long baseOffset, long stride, long[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * stride + baseOffset, arr[i]);
    }
  }

  void floatArray(float baseOffset, float stride, float[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * stride + baseOffset, arr[i], 1.f);
    }
  }

  void doubleArray(double baseOffset, double stride, double[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals((double) i * stride + baseOffset, arr[i], 0);
    }
  }

  void stringArray(String[] arr, boolean isFooBazBar) {
    if (isFooBazBar) {
      assertEquals(3, arr.length);
      assertEquals("Foo", arr[0]);
      assertEquals("Baz", arr[1]);
      assertEquals("Bar", arr[2]);
    } else {
      assertNotEquals("Foo", arr[0]);
      assertNotEquals("Baz", arr[1]);
      assertNotEquals("Bar", arr[2]);
    }
  }

  void stringEqual(boolean expectEqual, String lhs, String rhs) {
    if (expectEqual) {
      assertEquals(lhs, rhs);
    } else {
      assertNotEquals(lhs, rhs);
    }
  }

  void objectArray(int objectMemberOffset, ObjectTestHelper[] arr) {
    for (int i = 0; i < arr.length; i++) {
      int idxPlusOffset = i + objectMemberOffset;
      ObjectTestHelper dummyThatMatchesIdx =
          new ObjectTestHelper(idxPlusOffset, idxPlusOffset, idxPlusOffset);
      assertTrue(dummyThatMatchesIdx.isEqualTo(arr[i]));
    }
  }

  @Test
  public void booleanTests() {
    boolean[] arr = {false, false, false};
    nativeBooleanTests(this, arr);
  }

  @Test
  public void byteTests() {
    byte[] arr = {0, 1, 2, 3, 4, 5};
    nativeByteTests(this, arr);
  }

  @Test
  public void charTests() {
    char[] arr = {0, 1, 2, 3, 4, 5};
    nativeCharTests(this, arr);
  }

  @Test
  public void shortTests() {
    short[] arr = {0, 1, 2, 3, 4, 5};
    nativeShortTests(this, arr);
  }

  @Test
  public void intTests() {
    int[] arr = {0, 1, 2, 3, 4, 5};
    nativeIntTests(this, arr);
  }

  @Test
  public void longTests() {
    long[] arr = {0, 1, 2, 3, 4, 5};
    nativeLongTests(this, arr);
  }

  @Test
  public void floatTests() {
    float[] arr = {0, 1, 2, 3, 4, 5};
    nativeFloatTests(this, arr);
  }

  @Test
  public void doubleTests() {
    double[] arr = {0, 1, 2, 3, 4, 5};
    nativeDoubleTests(this, arr);
  }

  @Test
  public void stringTests() {
    String[] arr = {"Foo", "Baz", "Bar"};
    nativeStringTests(this, arr);
  }

  @Test
  public void objectTests() {
    ObjectTestHelper[] objectTestHelper = {
      new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(1, 1, 1), new ObjectTestHelper(2, 2, 2)
    };

    nativeObjectTests(this, objectTestHelper);
  }
}
