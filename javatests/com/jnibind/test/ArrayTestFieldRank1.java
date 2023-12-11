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
    new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(1, 1, 1), new ObjectTestHelper(2, 2, 2)
  };

  native void nativeBooleanTests(ArrayTestFieldRank1 arrayTest);

  native void nativeByteTests(ArrayTestFieldRank1 arrayTest);

  native void nativeCharTests(ArrayTestFieldRank1 arrayTest);

  native void nativeShortTests(ArrayTestFieldRank1 arrayTest);

  native void nativeIntTests(ArrayTestFieldRank1 arrayTest);

  native void nativeLongTests(ArrayTestFieldRank1 arrayTest);

  native void nativeFloatTests(ArrayTestFieldRank1 arrayTest);

  native void nativeDoubleTests(ArrayTestFieldRank1 arrayTest);

  native void nativeStringTests(ArrayTestFieldRank1 arrayTest, String[] stringArray);

  native void nativeObjectTests(ArrayTestFieldRank1 arrayTest, ObjectTestHelper[] objectArray);

  @Test
  public void booleanTests() {
    nativeBooleanTests(this);
  }

  @Test
  public void byteTests() {
    nativeByteTests(this);
  }

  @Test
  public void charTests() {
    nativeCharTests(this);
  }

  @Test
  public void shortTests() {
    nativeShortTests(this);
  }

  @Test
  public void intTests() {
    nativeIntTests(this);
  }

  @Test
  public void longTests() {
    nativeLongTests(this);
  }

  @Test
  public void floatTests() {
    nativeFloatTests(this);
  }

  @Test
  public void doubleTests() {
    nativeDoubleTests(this);
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
