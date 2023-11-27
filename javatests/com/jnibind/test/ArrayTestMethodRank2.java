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
  native void nativeBooleanTests2D(boolean[][] intArray);

  native void nativeByteTests2D(byte[][] byteArray);

  native void nativeCharTests2D(char[][] charArray);

  native void nativeShortTests2D(short[][] shortArray);

  native void nativeIntTests2D(int[][] intArray);

  native void nativeLongTests2D(long[][] longArray);

  native void nativeFloatTests2D(float[][] floatArray);

  native void nativeDoubleTests2D(double[][] doubleArray);

  native void nativeObjectTests2D(ObjectTestHelper[][] objectArray);

  /** 2D Tests. * */
  @Test
  public void booleanTests2D() {
    boolean[][] arr = {
      {false, true, false},
      {true, false, true},
      {false, true, false}
    };

    nativeBooleanTests2D(arr);
  }

  @Test
  public void byteTests2D() {
    byte[][] arr = {
      {0, 1, 2, 3, 4, 5},
      {6, 7, 8, 9, 10, 11},
      {12, 13, 14, 15, 16, 17}
    };
    nativeByteTests2D(arr);
  }

  @Test
  public void charTests2D() {
    char[][] arr = {
      {0, 1, 2, 3, 4, 5},
      {6, 7, 8, 9, 10, 11},
      {12, 13, 14, 15, 16, 17}
    };
    nativeCharTests2D(arr);
  }

  @Test
  public void shortTests2D() {
    short[][] arr = {
      {0, 1, 2, 3, 4, 5},
      {6, 7, 8, 9, 10, 11},
      {12, 13, 14, 15, 16, 17}
    };
    nativeShortTests2D(arr);
  }

  @Test
  public void intTests2D() {
    int[][] arr = {
      {0, 1, 2, 3, 4, 5},
      {6, 7, 8, 9, 10, 11},
      {12, 13, 14, 15, 16, 17}
    };
    nativeIntTests2D(arr);
  }

  @Test
  public void longTests2D() {
    long[][] arr = {
      {0, 1, 2, 3, 4, 5},
      {6, 7, 8, 9, 10, 11},
      {12, 13, 14, 15, 16, 17}
    };
    nativeLongTests2D(arr);
  }

  @Test
  public void floatTests2D() {
    float[][] arr = {
      {0.f, 1.f, 2.f, 3.f, 4.f, 5.f},
      {6.f, 7.f, 8.f, 9.f, 10.f, 11.f},
      {12.f, 13.f, 14.f, 15.f, 16.f, 17.f}
    };
    nativeFloatTests2D(arr);
  }

  @Test
  public void doubleTests2D() {
    double[][] arr = {
      {0, 1, 2, 3, 4, 5},
      {6, 7, 8, 9, 10, 11},
      {12, 13, 14, 15, 16, 17}
    };
    nativeDoubleTests2D(arr);
  }

  @Test
  public void objectTests2D() {
    ObjectTestHelper[][] arr = {
      {new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(1, 1, 1), new ObjectTestHelper(2, 2, 2)},
      {new ObjectTestHelper(3, 3, 3), new ObjectTestHelper(4, 4, 4), new ObjectTestHelper(5, 5, 5)},
      {new ObjectTestHelper(6, 6, 6), new ObjectTestHelper(7, 7, 7), new ObjectTestHelper(8, 8, 8)},
    };

    nativeObjectTests2D(arr);
  }
}
