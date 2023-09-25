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
package com.jnibind.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.AfterClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Exercises array functionality arcross rJNI. */
@RunWith(JUnit4.class)
public class ArrayTestMethodRank2 {
  static {
    System.loadLibrary("array_test_method_rank_2_jni");
  }

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  static native void jniTearDown();

  /** 2D Tests JNI declarations. * */
  native void nativeBooleanTests2D(ArrayTestMethodRank2 arrayTest, boolean[][] intArray);

  native void nativeByteTests2D(ArrayTestMethodRank2 arrayTest, byte[][] byteArray);

  native void nativeCharTests2D(ArrayTestMethodRank2 arrayTest, char[][] charArray);

  native void nativeShortTests2D(ArrayTestMethodRank2 arrayTest, short[][] shortArray);

  native void nativeIntTests2D(ArrayTestMethodRank2 arrayTest, int[][] intArray);

  native void nativeLongTests2D(ArrayTestMethodRank2 arrayTest, long[][] longArray);

  native void nativeFloatTests2D(ArrayTestMethodRank2 arrayTest, float[][] floatArray);

  native void nativeDoubleTests2D(ArrayTestMethodRank2 arrayTest, double[][] doubleArray);

  native void nativeObjectTests2D(ArrayTestMethodRank2 arrayTest, ObjectTestHelper[][] objectArray);

  /** 2D Tests RJNI declarations. * */
  void boolean2D(boolean testForTrue, boolean[][] arr) {
    // All even are true (unless testForTrue when they're flipped).
    for (int i = 0; i < arr.length; i++) {
      for (int j = 0; j < arr[i].length; j++) {
        boolean hotBit = (i + j) % 2 == 0;
        boolean expectTrue = testForTrue ? hotBit : !hotBit;

        if (expectTrue) {
          assertTrue(arr[i][j]);
        } else {
          assertFalse(arr[i][j]);
        }
      }
    }
  }

  void byte2D(byte baseOffset, byte[][] arr) {
    byte curVal = baseOffset;
    for (byte[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j]);
        curVal++;
      }
    }
  }

  void char2D(char baseOffset, char[][] arr) {
    char curVal = baseOffset;
    for (char[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j]);
        curVal++;
      }
    }
  }

  void short2D(short baseOffset, short[][] arr) {
    short curVal = baseOffset;
    for (short[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j]);
        curVal++;
      }
    }
  }

  void int2D(int baseOffset, int[][] arr) {
    int curVal = baseOffset;
    for (int[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j]);
        curVal++;
      }
    }
  }

  void long2D(long baseOffset, long[][] arr) {
    long curVal = baseOffset;
    for (long[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j]);
        curVal++;
      }
    }
  }

  void float2D(float baseOffset, float[][] arr) {
    float curVal = baseOffset;
    for (float[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j], 0.0f);
        curVal++;
      }
    }
  }

  void double2D(double baseOffset, double[][] arr) {
    double curVal = baseOffset;
    for (double[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j], 0.0);
        curVal++;
      }
    }
  }

  void object1D(ObjectTestHelper[] arr) {
    for (ObjectTestHelper element : arr) {
      System.out.println(element);
    }
  }

  void object2D(int objectMemberOffset, ObjectTestHelper[][] arr) {
    int curVal = objectMemberOffset;
    for (ObjectTestHelper[] objectList : arr) {
      for (ObjectTestHelper element : objectList) {
        assertTrue(element.isEqualTo(new ObjectTestHelper(curVal, curVal, curVal)));
        curVal++;
      }
    }
  }

  /** 2D Tests. * */
  @Test
  public void booleanTests2D() {
    boolean[][] arr = {
      {false, true, false},
      {true, false, true},
      {false, true, false}
    };

    nativeBooleanTests2D(this, arr);
  }

  @Test
  public void byteTests2D() {
    byte[][] arr = {
      {0, 1, 2, 3, 4, 5},
      {6, 7, 8, 9, 10, 11},
      {12, 13, 14, 15, 16, 17}
    };
    nativeByteTests2D(this, arr);
  }

  @Test
  public void charTests2D() {
    char[][] arr = {
      {0, 1, 2, 3, 4, 5},
      {6, 7, 8, 9, 10, 11},
      {12, 13, 14, 15, 16, 17}
    };
    nativeCharTests2D(this, arr);
  }

  @Test
  public void shortTests2D() {
    short[][] arr = {
      {0, 1, 2, 3, 4, 5},
      {6, 7, 8, 9, 10, 11},
      {12, 13, 14, 15, 16, 17}
    };
    nativeShortTests2D(this, arr);
  }

  @Test
  public void intTests2D() {
    int[][] arr = {
      {0, 1, 2, 3, 4, 5},
      {6, 7, 8, 9, 10, 11},
      {12, 13, 14, 15, 16, 17}
    };
    nativeIntTests2D(this, arr);
  }

  @Test
  public void longTests2D() {
    long[][] arr = {
      {0, 1, 2, 3, 4, 5},
      {6, 7, 8, 9, 10, 11},
      {12, 13, 14, 15, 16, 17}
    };
    nativeLongTests2D(this, arr);
  }

  @Test
  public void floatTests2D() {
    float[][] arr = {
      {0.f, 1.f, 2.f, 3.f, 4.f, 5.f},
      {6.f, 7.f, 8.f, 9.f, 10.f, 11.f},
      {12.f, 13.f, 14.f, 15.f, 16.f, 17.f}
    };
    nativeFloatTests2D(this, arr);
  }

  @Test
  public void doubleTests2D() {
    double[][] arr = {
      {0, 1, 2, 3, 4, 5},
      {6, 7, 8, 9, 10, 11},
      {12, 13, 14, 15, 16, 17}
    };
    nativeDoubleTests2D(this, arr);
  }

  @Test
  public void objectTests2D() {
    ObjectTestHelper[][] arr = {
      {new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(1, 1, 1), new ObjectTestHelper(2, 2, 2)},
      {new ObjectTestHelper(3, 3, 3), new ObjectTestHelper(4, 4, 4), new ObjectTestHelper(5, 5, 5)},
      {new ObjectTestHelper(6, 6, 6), new ObjectTestHelper(7, 7, 7), new ObjectTestHelper(8, 8, 8)},
    };

    object2D(0, arr);
    nativeObjectTests2D(this, arr);
  }
}
