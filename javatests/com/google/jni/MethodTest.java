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

package com.google.jni;

import static com.google.common.truth.Truth.assertThat;
import static org.mockito.ArgumentMatchers.anyBoolean;
import static org.mockito.ArgumentMatchers.anyFloat;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyLong;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import androidx.test.internal.runner.junit4.AndroidJUnit4ClassRunner;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnit;
import org.mockito.junit.MockitoRule;

/**
 * This class exercises rJNI (Reverse Java Native Interface) calls. All native functions will call a
 * Java counterpart on MethodTestHelper which is a mock that can be used to verify expectations.
 *
 * <p>For the sake of simplicity, each test is separated by type and exercises an rJni call
 * returning their type and passing their own parameter, the same but with parameters of their type,
 * returning their type and passing multiples of their type, and lastly the same but with return
 * type void.
 *
 * <p>Because of how signatures are generated, this provides decent coverage.
 */
@RunWith(AndroidJUnit4ClassRunner.class)
public class MethodTest {
  static {
    System.loadLibrary("method_test_jni");
  }

  @Rule public final MockitoRule mockito = MockitoJUnit.rule();
  @Mock public MethodTestHelper rJniTestHelper;

  /** Void Method Tests. */
  native void jniVoidMethod(MethodTestHelper mockMethodTestHelper);

  native void jniVoidMethodTakesOneInt(MethodTestHelper mockMethodTestHelper, int i);

  native void jniVoidMethodTakesFiveInts(
      MethodTestHelper mockMethodTestHelper, int i1, int i2, int i3, int i4, int i5);

  @Test
  public void voidTests() {
    jniVoidMethod(rJniTestHelper);
    jniVoidMethodTakesOneInt(rJniTestHelper, 1);
    jniVoidMethodTakesFiveInts(rJniTestHelper, 1, 2, 3, 4, 5);

    verify(rJniTestHelper).voidMethod();
    verify(rJniTestHelper).voidMethodTakesOneInt(1);
    verify(rJniTestHelper).voidMethodTakesFiveInts(1, 2, 3, 4, 5);
  }

  /** Boolean Method Tests. */
  native boolean jniBooleanMethod(MethodTestHelper mockMethodTestHelper);

  native boolean jniBooleanMethodTakesOneBoolean(MethodTestHelper mockMethodTestHelper, boolean i);

  native boolean jniBooleanMethodTakesFiveBooleans(
      MethodTestHelper mockMethodTestHelper,
      boolean i1,
      boolean i2,
      boolean i3,
      boolean i4,
      boolean i5);

  @Test
  public void booleanTests() {
    when(rJniTestHelper.booleanMethod()).thenReturn(MethodTestHelper.DEFAULT_BOOLEAN);
    when(rJniTestHelper.booleanMethodTakesOneBoolean(anyBoolean()))
        .thenReturn(MethodTestHelper.DEFAULT_BOOLEAN);
    when(rJniTestHelper.booleanMethodTakesFiveBooleans(
            anyBoolean(), anyBoolean(), anyBoolean(), anyBoolean(), anyBoolean()))
        .thenReturn(MethodTestHelper.DEFAULT_BOOLEAN);

    assertThat(jniBooleanMethod(rJniTestHelper)).isEqualTo(MethodTestHelper.DEFAULT_BOOLEAN);
    assertThat(jniBooleanMethodTakesOneBoolean(rJniTestHelper, true))
        .isEqualTo(MethodTestHelper.DEFAULT_BOOLEAN);

    assertThat(jniBooleanMethodTakesFiveBooleans(rJniTestHelper, true, false, true, false, true))
        .isEqualTo(MethodTestHelper.DEFAULT_BOOLEAN);

    verify(rJniTestHelper).booleanMethod();
    verify(rJniTestHelper).booleanMethodTakesOneBoolean(true);
    verify(rJniTestHelper).booleanMethodTakesFiveBooleans(true, false, true, false, true);
  }

  /** Int Method Tests. */
  native int jniIntMethod(MethodTestHelper mockMethodTestHelper);

  native int jniIntMethodTakesOneInt(MethodTestHelper mockMethodTestHelper, int i);

  native int jniIntMethodTakesFiveInts(
      MethodTestHelper mockMethodTestHelper, int i1, int i2, int i3, int i4, int i5);

  @Test
  public void intTests() {
    when(rJniTestHelper.intMethod()).thenReturn(MethodTestHelper.DEFAULT_INT);
    when(rJniTestHelper.intMethodTakesOneInt(anyInt())).thenReturn(MethodTestHelper.DEFAULT_INT);
    when(rJniTestHelper.intMethodTakesFiveInts(anyInt(), anyInt(), anyInt(), anyInt(), anyInt()))
        .thenReturn(MethodTestHelper.DEFAULT_INT);

    assertThat(jniIntMethod(rJniTestHelper)).isEqualTo(MethodTestHelper.DEFAULT_INT);
    assertThat(jniIntMethodTakesOneInt(rJniTestHelper, 1)).isEqualTo(MethodTestHelper.DEFAULT_INT);

    assertThat(jniIntMethodTakesFiveInts(rJniTestHelper, 1, 2, 3, 4, 5))
        .isEqualTo(MethodTestHelper.DEFAULT_INT);


    verify(rJniTestHelper).intMethod();
    verify(rJniTestHelper).intMethodTakesOneInt(1);
    verify(rJniTestHelper).intMethodTakesFiveInts(1, 2, 3, 4, 5);
  }

  /** Long Method Tests. */
  native long jniLongMethod(MethodTestHelper mockMethodTestHelper);

  native long jniLongMethodTakesOneLong(MethodTestHelper mockMethodTestHelper, long i);

  native long jniLongMethodTakesFiveLongs(
      MethodTestHelper mockMethodTestHelper, long i1, long i2, long i3, long i4, long i5);

  @Test
  public void longTests() {
    when(rJniTestHelper.longMethod()).thenReturn(MethodTestHelper.DEFAULT_LONG);
    when(rJniTestHelper.longMethodTakesOneLong(anyLong()))
        .thenReturn(MethodTestHelper.DEFAULT_LONG);
    when(rJniTestHelper.longMethodTakesFiveLongs(
            anyLong(), anyLong(), anyLong(), anyLong(), anyLong()))
        .thenReturn(MethodTestHelper.DEFAULT_LONG);

    assertThat(jniLongMethod(rJniTestHelper)).isEqualTo(MethodTestHelper.DEFAULT_LONG);
    assertThat(jniLongMethodTakesOneLong(rJniTestHelper, 1))
        .isEqualTo(MethodTestHelper.DEFAULT_LONG);

    assertThat(jniLongMethodTakesFiveLongs(rJniTestHelper, 1, 2, 3, 4, 5))
        .isEqualTo(MethodTestHelper.DEFAULT_LONG);

    verify(rJniTestHelper).longMethod();
    verify(rJniTestHelper).longMethodTakesOneLong(1);
    verify(rJniTestHelper).longMethodTakesFiveLongs(1, 2, 3, 4, 5);
  }

  /** Float Method Tests. */
  native float jniFloatMethod(MethodTestHelper mockMethodTestHelper);

  native float jniFloatMethodTakesOneFloat(MethodTestHelper mockMethodTestHelper, float i);

  native float jniFloatMethodTakesFiveFloats(
      MethodTestHelper mockMethodTestHelper, float i1, float i2, float i3, float i4, float i5);

  /** Double Method Tests. */
  native double jniDoubleMethod(MethodTestHelper mockMethodTestHelper);

  native double jniDoubleMethodTakesOneDouble(MethodTestHelper mockMethodTestHelper, double i);

  native double jniDoubleMethodTakesFiveDoubles(
      MethodTestHelper mockMethodTestHelper, double i1, double i2, double i3, double i4, double i5);

  @Test
  public void floatTests() {
    when(rJniTestHelper.floatMethod()).thenReturn(MethodTestHelper.DEFAULT_FLOAT);
    when(rJniTestHelper.floatMethodTakesOneFloat(anyFloat()))
        .thenReturn(MethodTestHelper.DEFAULT_FLOAT);
    when(rJniTestHelper.floatMethodTakesFiveFloats(
            anyFloat(), anyFloat(), anyFloat(), anyFloat(), anyFloat()))
        .thenReturn(MethodTestHelper.DEFAULT_FLOAT);

    assertThat(jniFloatMethod(rJniTestHelper)).isEqualTo(MethodTestHelper.DEFAULT_FLOAT);
    assertThat(jniFloatMethodTakesOneFloat(rJniTestHelper, 1))
        .isEqualTo(MethodTestHelper.DEFAULT_FLOAT);

    assertThat(jniFloatMethodTakesFiveFloats(rJniTestHelper, 1, 2, 3, 4, 5))
        .isEqualTo(MethodTestHelper.DEFAULT_FLOAT);


    verify(rJniTestHelper).floatMethod();
    verify(rJniTestHelper).floatMethodTakesOneFloat(1);
    verify(rJniTestHelper).floatMethodTakesFiveFloats(1, 2, 3, 4, 5);
  }

  /** Overload Tests. */
  native int jniCallFooOverload1(MethodTestHelper mockMethodTestHelper);

  native int jniCallFooOverload2(MethodTestHelper mockMethodTestHelper);

  native int jniCallFooOverload3(MethodTestHelper mockMethodTestHelper);

  @Test
  public void overloadTests() {
    jniCallFooOverload1(rJniTestHelper);
    jniCallFooOverload2(rJniTestHelper);
    jniCallFooOverload3(rJniTestHelper);

    verify(rJniTestHelper).foo();
    verify(rJniTestHelper).foo(123.f);
    verify(rJniTestHelper).foo(123.f, 456.f);
  }
}
