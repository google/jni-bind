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

import static com.google.common.truth.Truth.assertThat;

import androidx.test.internal.runner.junit4.AndroidJUnit4ClassRunner;
import org.junit.Test;
import org.junit.runner.RunWith;

@RunWith(AndroidJUnit4ClassRunner.class)
public final class ContextTest {
  private static final String TAG = "ContextTest";

  static {
    System.loadLibrary("context_test_jni");
  }

  public ContextTest() {
    DoSetup();
  }

  @Test
  public void testGlobalObjectsPersistAcrossJniOverloads() {
    long native_context = CreateNativeContextObject(555);
    assertThat(QueryNativeObject(native_context).intVal1).isEqualTo(555);
    assertThat(QueryNativeObject(native_context).intVal1).isEqualTo(555);
    assertThat(QueryNativeObject(native_context).intVal1).isEqualTo(555);
    assertThat(QueryNativeObject(native_context).intVal1).isEqualTo(555);
    assertThat(ExtractNativeObject(native_context).intVal1).isEqualTo(555);
    assertThat(QueryNativeObject(native_context)).isEqualTo(null);
    assertThat(QueryNativeObject(native_context)).isEqualTo(null);
  }

  @Test
  public void testGlobalObjectsPersistAcrossJniOverloads2() {
    assertThat(CreateNativeContextObjectSetValToSum(5, 7).intVal1).isEqualTo(12);
  }

  @Test
  public void testGlobalObjectsBeDestroyed() {
    long native_context = CreateNativeContextObject(9876);
    assertThat(QueryNativeObject(native_context).intVal1).isEqualTo(9876);
    DestroyNativeContext(native_context);
  }

  // Some kind of "initial call".  This test suite doesn't override JNIOnload in order to emulate
  // situations where this may not be accessible to the caller (or cannot be overridden) but the
  // first call across JNI is known.
  native void DoSetup();

  // Causes native code to create an object that will be stored in native code and returned on a
  // subsequent call.  This can only be done with global objects and not local objects.
  native long CreateNativeContextObject(int val);

  // Returns an ObjectTestHelper with intVal1 set to the sum of the vals.
  native ObjectTestHelper CreateNativeContextObjectSetValToSum(int val1, int val2);

  // Returns a handle to a global object held within the inner native context.
  native ObjectTestHelper QueryNativeObject(long context);

  // Returns the handle to the native context, but simultaneously releases it in the process.
  native ObjectTestHelper ExtractNativeObject(long context);

  // Destroys the entire native context possibly releasing the contained global object.
  // This would be representative of when an app would be shutting down.
  native void DestroyNativeContext(long context);
}
