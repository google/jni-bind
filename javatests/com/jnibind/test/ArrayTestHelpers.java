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
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

/** Helpers functions to assert array behaviour from native. */
final class ArrayTestHelpers {
  private ArrayTestHelpers() {}

  static void assertBoolean1D(boolean testForTrue, boolean[] arr) {
    for (int i = 0; i < arr.length; i++) {
      if (testForTrue) {
        assertTrue(arr[i]);
      } else {
        assertFalse(arr[i]);
      }
    }
  }

  static void assertByte1D(byte baseOffset, byte[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  static void assertChar1D(char baseOffset, char[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  static void assertShort1D(short baseOffset, short[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  static void assertInt1D(int baseOffset, int[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  static void assertLong1D(long baseOffset, long[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  static void assertFloat1D(float baseOffset, float[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i], 1.f);
    }
  }

  static void assertDouble1D(double baseOffset, double[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals((double) i + baseOffset, arr[i], 0);
    }
  }

  static void assertString1D(String[] arr) {
    assertEquals(3, arr.length);
    assertEquals("Foo", arr[0]);
    assertEquals("Baz", arr[1]);
    assertEquals("Bar", arr[2]);
  }

  static void assertObject1D(int objectMemberOffset, ObjectTestHelper[] arr) {
    for (int i = 0; i < arr.length; i++) {
      int idxPlusOffset = i + objectMemberOffset;
      ObjectTestHelper dummyThatMatchesIdx =
          new ObjectTestHelper(idxPlusOffset, idxPlusOffset, idxPlusOffset);
      assertTrue(dummyThatMatchesIdx.isEqualTo(arr[i]));
    }
  }

  static void assertObjectArrayOfNulls1D(ObjectTestHelper[] arr) {
    for (ObjectTestHelper element : arr) {
      assertNull(element);
    }
  }

  /** 2D Tests RJNI declarations. * */
  static void assertBoolean2D(boolean testForTrue, boolean[][] arr) {
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

  static void assertByte2D(byte baseOffset, byte[][] arr) {
    byte curVal = baseOffset;
    for (byte[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j]);
        curVal++;
      }
    }
  }

  static void assertChar2D(char baseOffset, char[][] arr) {
    char curVal = baseOffset;
    for (char[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j]);
        curVal++;
      }
    }
  }

  static void assertShort2D(short baseOffset, short[][] arr) {
    short curVal = baseOffset;
    for (short[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j]);
        curVal++;
      }
    }
  }

  static void assertInt2D(int baseOffset, int[][] arr) {
    int curVal = baseOffset;
    for (int[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j]);
        curVal++;
      }
    }
  }

  static void assertLong2D(long baseOffset, long[][] arr) {
    long curVal = baseOffset;
    for (long[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j]);
        curVal++;
      }
    }
  }

  static void assertFloat2D(float baseOffset, float[][] arr) {
    float curVal = baseOffset;
    for (float[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j], 0.0f);
        curVal++;
      }
    }
  }

  static void assertDouble2D(double baseOffset, double[][] arr) {
    double curVal = baseOffset;
    for (double[] element : arr) {
      for (int j = 0; j < element.length; j++) {
        assertEquals(curVal, element[j], 0.0);
        curVal++;
      }
    }
  }

  static void assertObject2D(int objectMemberOffset, ObjectTestHelper[][] arr) {
    int curVal = objectMemberOffset;
    for (ObjectTestHelper[] objectList : arr) {
      for (ObjectTestHelper element : objectList) {
        assertTrue(element.isEqualTo(new ObjectTestHelper(curVal, curVal, curVal)));
        curVal++;
      }
    }
  }
}
