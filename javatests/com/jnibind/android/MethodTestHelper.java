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

package com.jnibind.android;

import com.google.android.apps.common.proguard.UsedByNative;

/**
 * This object is used to set mock expectations for MethodTest as inner classes do not support
 * mocking and the class definition must match the filename.
 */
public class MethodTestHelper {
  /** Void Method Tests. */
  @UsedByNative("method_test_jni.cc")
  public void voidMethod() {}

  @UsedByNative("method_test_jni.cc")
  public void voidMethodTakesOneInt(int i) {}

  @UsedByNative("method_test_jni.cc")
  public void voidMethodTakesFiveInts(int i1, int i2, int i3, int i4, int i5) {}

  /** Boolean Method Tests. */
  public static final boolean DEFAULT_BOOLEAN = false;

  @UsedByNative("method_test_jni.cc")
  public boolean booleanMethod() {
    return true;
  }

  @UsedByNative("method_test_jni.cc")
  public boolean booleanMethodTakesOneBoolean(boolean i) {
    return true;
  }

  @UsedByNative("method_test_jni.cc")
  public boolean booleanMethodTakesFiveBooleans(
      boolean i1, boolean i2, boolean i3, boolean i4, boolean i5) {
    return true;
  }

  /** Int Method Tests. */
  public static final int DEFAULT_INT = 12345;

  @UsedByNative("method_test_jni.cc")
  public int intMethod() {
    return -1;
  }

  @UsedByNative("method_test_jni.cc")
  public int intMethodTakesOneInt(int i) {
    return -1;
  }

  @UsedByNative("method_test_jni.cc")
  public int intMethodTakesFiveInts(int i1, int i2, int i3, int i4, int i5) {
    return -1;
  }

  /** Long Method Tests. */
  public static final long DEFAULT_LONG = 12345;

  @UsedByNative("method_test_jni.cc")
  public long longMethod() {
    return -1;
  }

  @UsedByNative("method_test_jni.cc")
  public long longMethodTakesOneLong(long i) {
    return -1;
  }

  @UsedByNative("method_test_jni.cc")
  public long longMethodTakesFiveLongs(long i1, long i2, long i3, long i4, long i5) {
    return -1;
  }

  /** Float Method Tests. */
  public static final float DEFAULT_FLOAT = 12345;

  @UsedByNative("method_test_jni.cc")
  public float floatMethod() {
    return -1;
  }

  @UsedByNative("method_test_jni.cc")
  public float floatMethodTakesOneFloat(float i) {
    return -1;
  }

  @UsedByNative("method_test_jni.cc")
  public float floatMethodTakesFiveFloats(float i1, float i2, float i3, float i4, float i5) {
    return -1;
  }

  /** Double Method Tests. */
  public static final double DEFAULT_DOUBLE = 12345;

  @UsedByNative("method_test_jni.cc")
  public double doubleMethod() {
    return -1;
  }

  @UsedByNative("method_test_jni.cc")
  public double doubleMethodTakesOneDouble(double i) {
    return -1;
  }

  @UsedByNative("method_test_jni.cc")
  public double doubleMethodTakesFiveDoubles(
      double i1, double i2, double i3, double i4, double i5) {
    return -1;
  }

  @UsedByNative("method_test_jni.cc")
  public int foo() {
    return -1;
  }

  @UsedByNative("method_test_jni.cc")
  public int foo(float f1) {
    return -1;
  }

  @UsedByNative("method_test_jni.cc")
  public int foo(float f1, float f2) {
    return -1;
  }
}
