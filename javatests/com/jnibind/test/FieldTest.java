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

import static com.google.common.truth.Truth.assertThat;

import org.junit.AfterClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/**
 * Exercises get and set functionality across rJni.
 *
 * <p>All functions accept a value which they will set on the helper. Then they will query the same
 * value and return it across rJni.
 */
@RunWith(JUnit4.class)
public class FieldTest {
  static {
    System.loadLibrary("field_test_jni");
  }

  public FieldTestHelper rJniTestHelper = new FieldTestHelper();

  static native void jniTearDown();

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  /** Int Field Test. */
  native int jniIntField(FieldTestHelper mockFieldTestHelper, int val);

  @Test
  public void intTest() {
    assertThat(jniIntField(rJniTestHelper, 12345)).isEqualTo(12345);
    assertThat(rJniTestHelper.intField).isEqualTo(12345);

    assertThat(jniIntField(rJniTestHelper, Integer.MAX_VALUE)).isEqualTo(Integer.MAX_VALUE);
    assertThat(rJniTestHelper.intField).isEqualTo(Integer.MAX_VALUE);

    assertThat(jniIntField(rJniTestHelper, Integer.MIN_VALUE)).isEqualTo(Integer.MIN_VALUE);
    assertThat(rJniTestHelper.intField).isEqualTo(Integer.MIN_VALUE);
  }

  /** Float Field Test. */
  native float jniFloatField(FieldTestHelper mockFieldTestHelper, float val);

  @Test
  public void floatTests() {
    assertThat(jniFloatField(rJniTestHelper, 12345)).isEqualTo(12345);
    assertThat(rJniTestHelper.floatField).isEqualTo(12345);

    assertThat(jniFloatField(rJniTestHelper, Float.MAX_VALUE)).isEqualTo(Float.MAX_VALUE);
    assertThat(rJniTestHelper.floatField).isEqualTo(Float.MAX_VALUE);

    assertThat(jniFloatField(rJniTestHelper, Float.MIN_VALUE)).isEqualTo(Float.MIN_VALUE);
    assertThat(rJniTestHelper.floatField).isEqualTo(Float.MIN_VALUE);
  }

  /** Double Field Test. */
  native double jniDoubleField(FieldTestHelper mockFieldTestHelper, double val);

  @Test
  public void doubleTests() {
    assertThat(jniDoubleField(rJniTestHelper, 12345)).isEqualTo(12345);
    assertThat(rJniTestHelper.doubleField).isEqualTo(12345);

    assertThat(jniDoubleField(rJniTestHelper, Double.MAX_VALUE)).isEqualTo(Double.MAX_VALUE);
    assertThat(rJniTestHelper.doubleField).isEqualTo(Double.MAX_VALUE);

    assertThat(jniDoubleField(rJniTestHelper, Double.MIN_VALUE)).isEqualTo(Double.MIN_VALUE);
    assertThat(rJniTestHelper.doubleField).isEqualTo(Double.MIN_VALUE);
  }
}
