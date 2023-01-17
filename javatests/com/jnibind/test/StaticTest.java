/*
 * Copyright 2022 Google LLC
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
import static org.junit.Assert.assertTrue;

import org.junit.AfterClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** This classes calls native methods which subsequently call static ones. */
@RunWith(JUnit4.class)
public final class StaticTest {
  static {
    System.loadLibrary("static_test_jni");
  }

  static native void jniTearDown();

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  /** Method Tests. */
  native void voidMethodTestNative();

  native byte byteMethodTestNative();

  native char charMethodTestNative();

  native short shortMethodTestNative();

  native int intMethodTestNative();

  native long longMethodTestNative();

  native float floatMethodTestNative();

  native double doubleMethodTestNative();

  native ObjectTestHelper objectMethodTestNative();

  native String complexMethodTestNative(int a, float b, String c, long[][] d);

  @Test
  public void voidMethod() {
    assertThat(StaticTestHelper.voidCalled).isFalse();
    voidMethodTestNative();
    assertThat(StaticTestHelper.voidCalled).isTrue();
  }

  @Test
  public void byteMethod() {
    assertThat(byteMethodTestNative()).isEqualTo(123);
  }

  @Test
  public void charMethod() {
    assertThat(charMethodTestNative()).isEqualTo('a');
  }

  @Test
  public void shortMethod() {
    assertThat(shortMethodTestNative()).isEqualTo(30);
  }

  @Test
  public void intMethod() {
    assertThat(intMethodTestNative()).isEqualTo(789);
  }

  @Test
  public void longMethod() {
    assertThat(longMethodTestNative()).isEqualTo(1011);
  }

  @Test
  public void floatMethod() {
    assertThat(floatMethodTestNative()).isEqualTo(1213.0f);
  }

  @Test
  public void doubleMethod() {
    assertThat(doubleMethodTestNative()).isEqualTo(1415.0);
  }

  @Test
  public void objectMethod() {
    assertTrue(objectMethodTestNative().isEqualTo(new ObjectTestHelper(1, 2, 3)));
  }

  @Test
  public void complexMethod() {
    long[][] longVal = new long[6][6];
    longVal[4][5] = 6;

    assertThat(complexMethodTestNative(1, 2, "3", longVal)).isEqualTo("COMPLEX");
    longVal[4][5] = 0;
    assertThat(complexMethodTestNative(1, 2, "3", longVal)).isEqualTo("FAIL");
  }

  /** Field Tests. */
  native boolean booleanFieldTestNative(boolean val);

  native byte byteFieldTestNative(byte val);

  native char charFieldTestNative(char val);

  native short shortFieldTestNative(short val);

  native int intFieldTestNative(int val);

  native long longFieldTestNative(long val);

  native float floatFieldTestNative(float val);

  native double doubleFieldTestNative(double val);

  native ObjectTestHelper objectFieldTestNative(ObjectTestHelper val);

  @Test
  public void booleanField() {
    assertThat(booleanFieldTestNative(true)).isTrue();
  }

  @Test
  public void byteField() {
    assertThat(byteFieldTestNative((byte) 5)).isEqualTo((byte) 5);
  }

  @Test
  public void charField() {
    assertThat(charFieldTestNative('a')).isEqualTo('a');
  }

  @Test
  public void shortField() {
    assertThat(shortFieldTestNative((short) 30)).isEqualTo((short) 30);
  }

  @Test
  public void intField() {
    assertThat(intFieldTestNative(789)).isEqualTo(789);
  }

  @Test
  public void longField() {
    assertThat(longFieldTestNative((long) 1011)).isEqualTo((long) 1011);
  }

  @Test
  public void floatField() {
    assertThat(floatFieldTestNative(1213.0f)).isEqualTo(1213.0f);
  }

  @Test
  public void doubleField() {
    assertThat(doubleFieldTestNative(1415.0)).isEqualTo(1415.0);
  }

  @Test
  public void objectField() {
    assertTrue(
        objectFieldTestNative(new ObjectTestHelper(1, 2, 3))
            .isEqualTo(new ObjectTestHelper(1, 2, 3)));
  }
}
