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

import org.junit.AfterClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Exercises array functionality for fields across rJNI. */
@RunWith(JUnit4.class)
public final class ArrayFieldTest {
  static {
    System.loadLibrary("array_field_test_jni");
  }

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  // Rank 1 Fields.
  public boolean[] booleanFieldRank1;
  public byte[] byteFieldRank1;
  public char[] charFieldRank1;
  public short[] shortFieldRank1;
  public int[] intFieldRank1;
  public long[] longFieldRank1;
  public float[] floatFieldRank1;
  public double[] doubleFieldRank1;
  public ObjectTestHelper[] objectFieldRank1;

  static native void jniTearDown();

  native void nativeBooleanTests(ArrayFieldTest arrayTest, boolean[] intArray);

  native void nativeByteTests(ArrayFieldTest arrayTest, byte[] byteArray);

  native void nativeCharTests(ArrayFieldTest arrayTest, char[] charArray);

  native void nativeShortTests(ArrayFieldTest arrayTest, short[] shortArray);

  native void nativeIntTests(ArrayFieldTest arrayTest, int[] intArray);

  native void nativeLongTests(ArrayFieldTest arrayTest, long[] longArray);

  native void nativeFloatTests(ArrayFieldTest arrayTest, float[] floatArray);

  native void nativeDoubleTests(ArrayFieldTest arrayTest, double[] doubleArray);

  native void nativeObjectTests(ArrayFieldTest arrayTest, ObjectTestHelper[] objectArray);

  @Test
  public void booleanTests() {
    boolean[] arr = {false, true, false};
    nativeBooleanTests(this, arr);
    assertThat(booleanFieldRank1).isEqualTo(arr);
  }

  /*
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
  */
}
