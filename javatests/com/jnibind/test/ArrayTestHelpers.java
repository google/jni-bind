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

import static com.google.common.truth.Truth.assertThat;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

/** Helpers functions to assert array behaviour from native. */
final class ArrayTestHelpers {
  private ArrayTestHelpers() {}

  /** Rank 0 Verification Methods. */
  static void assertString(String lhs, String rhs) {
    assertEquals(lhs, rhs);
  }

  static void assertObject(int objectMemberOffset, ObjectTestHelper arr) {
    assertTrue(
        arr.isEqualTo(
            new ObjectTestHelper(objectMemberOffset, objectMemberOffset, objectMemberOffset)));
  }

  /** Rank 1 Verification Methods. */
  static void assertBoolean1D(boolean baseOffset, boolean stride, boolean[] arr) {
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

  static void assertByte1D(byte baseOffset, byte stride, byte[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * stride + baseOffset, arr[i]);
    }
  }

  static void assertChar1D(char baseOffset, char stride, char[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * stride + baseOffset, arr[i]);
    }
  }

  static void assertShort1D(short baseOffset, short stride, short[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * stride + baseOffset, arr[i]);
    }
  }

  static void assertInt1D(int baseOffset, int stride, int[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * stride + baseOffset, arr[i]);
    }
  }

  static void assertLong1D(long baseOffset, long stride, long[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * stride + baseOffset, arr[i]);
    }
  }

  static void assertFloat1D(float baseOffset, float stride, float[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i * stride + baseOffset, arr[i], 1.f);
    }
  }

  static void assertDouble1D(double baseOffset, double stride, double[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals((double) i * stride + baseOffset, arr[i], 0);
    }
  }

  static void assertString1D(String[] arr, boolean isFooBazBar) {
    if (isFooBazBar) {
      assertThat(arr).hasLength(3);
      assertEquals("Foo", arr[0]);
      assertEquals("Baz", arr[1]);
      assertEquals("Bar", arr[2]);
    } else {
      assertNotEquals("Foo", arr[0]);
      assertNotEquals("Baz", arr[1]);
      assertNotEquals("Bar", arr[2]);
    }
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

  /** Rank 2 Verification Methods. */
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
