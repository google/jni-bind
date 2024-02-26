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
public class ArrayTestFieldRank2 {
  static {
    System.load(
        System.getenv("JAVA_RUNFILES")
            + "/__main__/javatests/com/jnibind/test/libarray_test_field_rank_2_jni.so");
  }

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  static native void jniTearDown();

  public boolean[][] booleanArrayField = {
    {false, true, false},
    {true, false, true},
    {false, true, false}
  };

  byte[][] byteArrayField = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}
  };

  char[][] charArrayField = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
  };

  short[][] shortArrayField = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}
  };

  int[][] intArrayField = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}
  };

  long[][] longArrayField = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}
  };

  float[][] floatArrayField = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}
  };

  double[][] doubleArrayField = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}
  };

  String[][] stringArrayField = {
    {"BAD", "BAD", "BAD"},
    {"BAD", "BAD", "BAD"},
    {"BAD", "BAD", "BAD"}
  };

  ObjectTestHelper[][] objectArrayField = {
    {new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(0, 0, 0)},
    {new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(0, 0, 0)},
    {new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(0, 0, 0), new ObjectTestHelper(0, 0, 0)}
  };

  native void nativeBooleanTests(ArrayTestFieldRank2 arrayTest);

  native void nativeByteTests(ArrayTestFieldRank2 arrayTest);

  native void nativeCharTests(ArrayTestFieldRank2 arrayTest);

  native void nativeShortTests(ArrayTestFieldRank2 arrayTest);

  native void nativeIntTests(ArrayTestFieldRank2 arrayTest);

  native void nativeLongTests(ArrayTestFieldRank2 arrayTest);

  native void nativeFloatTests(ArrayTestFieldRank2 arrayTest);

  native void nativeDoubleTests(ArrayTestFieldRank2 arrayTest);

  native void nativeStringTests(ArrayTestFieldRank2 arrayTest);

  native void nativeObjectTests(ArrayTestFieldRank2 arrayTest);

  @Test
  public void booleanTests() {
    boolean[] arr = {false, false, false};
    nativeBooleanTests(this);
  }

  @Test
  public void byteTests() {
    byte[] arr = {0, 1, 2, 3, 4, 5};
    nativeByteTests(this);
  }

  @Test
  public void charTests() {
    char[] arr = {0, 1, 2, 3, 4, 5};
    nativeCharTests(this);
  }

  @Test
  public void shortTests() {
    short[] arr = {0, 1, 2, 3, 4, 5};
    nativeShortTests(this);
  }

  @Test
  public void intTests() {
    int[] arr = {0, 1, 2, 3, 4, 5};
    nativeIntTests(this);
  }

  @Test
  public void longTests() {
    long[] arr = {0, 1, 2, 3, 4, 5};
    nativeLongTests(this);
  }

  @Test
  public void floatTests() {
    float[] arr = {0, 1, 2, 3, 4, 5};
    nativeFloatTests(this);
  }

  @Test
  public void doubleTests() {
    double[] arr = {0, 1, 2, 3, 4, 5};
    nativeDoubleTests(this);
  }

  @Test
  public void stringTests() {
    // TODO(b/143908983): This is broken, but using regular `jobject` works.
  }

  @Test
  public void objectTests() {
    nativeObjectTests(this);
  }
}
