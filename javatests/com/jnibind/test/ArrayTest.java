// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package com.jnibind.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import androidx.test.internal.runner.junit4.AndroidJUnit4ClassRunner;
import org.junit.Test;
import org.junit.runner.RunWith;

/** Exercises array functionality arcross rJNI. */
@RunWith(AndroidJUnit4ClassRunner.class)
public class ArrayTest {
  static {
    System.loadLibrary("array_test_jni");
  }

  native void nativeBooleanTests(ArrayTest arrayTest, boolean[] intArray);

  native void nativeByteTests(ArrayTest arrayTest, byte[] byteArray);

  native void nativeCharTests(ArrayTest arrayTest, char[] charArray);

  native void nativeShortTests(ArrayTest arrayTest, short[] shortArray);

  native void nativeIntTests(ArrayTest arrayTest, int[] intArray);

  native void nativeLongTests(ArrayTest arrayTest, long[] longArray);

  native void nativeFloatTests(ArrayTest arrayTest, float[] floatArray);

  native void nativeDoubleTests(ArrayTest arrayTest, double[] doubleArray);

  native void nativeObjectTests(ArrayTest arrayTest, ObjectTestHelper[] objectArray);

  void rJniBooleanArray(boolean testForTrue, boolean[] arr) {
    for (int i = 0; i < arr.length; i++) {
      if (testForTrue) {
        assertTrue(arr[i]);
      } else {
        assertFalse(arr[i]);
      }
    }
  }

  void rJniByteArray(byte baseOffset, byte[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  void rJniCharArray(char baseOffset, char[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  void rJniShortArray(short baseOffset, short[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  void rJniIntArray(int baseOffset, int[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  void rJniLongArray(long baseOffset, long[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i]);
    }
  }

  void rJniFloatArray(float baseOffset, float[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals(i + baseOffset, arr[i], 1.f);
    }
  }

  void rJniDoubleArray(double baseOffset, double[] arr) {
    for (int i = 0; i < arr.length; i++) {
      assertEquals((double) i + baseOffset, arr[i], 0);
    }
  }

  void rJniObjectArray(int objectMemberOffset, ObjectTestHelper[] arr) {
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
  public void objectTests() {
    ObjectTestHelper[] objectTestHelper = {
      new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(1, 1, 1), new ObjectTestHelper(2, 2, 2)
    };

    nativeObjectTests(this, objectTestHelper);
  }
}
