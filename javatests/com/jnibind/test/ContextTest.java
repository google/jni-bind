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

import static com.google.common.truth.Truth.assertThat;

import org.junit.AfterClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

@RunWith(JUnit4.class)
public final class ContextTest {
  private static final String TAG = "ContextTest";

  static {
    System.load(
        System.getenv("JAVA_RUNFILES")
            + "/_main/javatests/com/jnibind/test/libcontext_test_jni.so");
  }

  public ContextTest() {
    DoSetup();
  }

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  @Test
  public void testGlobalObjectsPersistAcrossJniOverloads() {
    long nativeContext = nativeCreateContext(555);
    assertThat(nativeQueryObject(nativeContext).intVal1).isEqualTo(555);
    assertThat(nativeQueryObject(nativeContext).intVal1).isEqualTo(555);
    assertThat(nativeQueryObject(nativeContext).intVal1).isEqualTo(555);
    assertThat(nativeQueryObject(nativeContext).intVal1).isEqualTo(555);
    assertThat(nativeExtractObject(nativeContext).intVal1).isEqualTo(555);
    assertThat(nativeQueryObject(nativeContext)).isEqualTo(null);
    assertThat(nativeQueryObject(nativeContext)).isEqualTo(null);
  }

  @Test
  public void testGlobalObjectsPersistAcrossJniOverloads2() {
    assertThat(nativeCreateContextSetValToSum(5, 7).intVal1).isEqualTo(12);
  }

  @Test
  public void testGlobalObjectsBeDestroyed() {
    long nativeContext = nativeCreateContext(9876);
    assertThat(nativeQueryObject(nativeContext).intVal1).isEqualTo(9876);
    nativeDestroyContext(nativeContext);
  }

  @Test
  public void testObjectsOriginatedFromJavaPersistWhenPromotedToGlobal() {
    ObjectTestHelper objectTestHelper = new ObjectTestHelper(9876);
    long nativeContext = nativeCreateContextWithPromotion(objectTestHelper);
    assertThat(nativeQueryObject(nativeContext).intVal1).isEqualTo(9876);
    objectTestHelper.intVal1 = 1234;
    assertThat(nativeQueryObject(nativeContext).intVal1).isEqualTo(1234);

    // Despite the object being deleted here, the native object pins.
    objectTestHelper = null;
    assertThat(nativeQueryObject(nativeContext).intVal1).isEqualTo(1234);

    nativeDestroyContext(nativeContext);
  }

  @Test
  public void testObjectsOriginatedFromJavaPersistWhenCopied() {
    ObjectTestHelper objectTestHelper = new ObjectTestHelper(9876);
    long nativeContext = nativeCreateContextWithCopy(objectTestHelper);
    assertThat(nativeQueryObject(nativeContext).intVal1).isEqualTo(9876);
    objectTestHelper.intVal1 = 1234;
    assertThat(nativeQueryObject(nativeContext).intVal1).isEqualTo(1234);

    // Despite the object being deleted here, the native object pins.
    objectTestHelper = null;
    assertThat(nativeQueryObject(nativeContext).intVal1).isEqualTo(1234);

    nativeDestroyContext(nativeContext);
  }

  // Some kind of "initial call".  This test suite doesn't override JNIOnload in order to emulate
  // situations where this may not be accessible to the caller (or cannot be overridden) but the
  // first call across JNI is known.
  native void DoSetup();

  // Tears down the JvmRef.
  static native void jniTearDown();

  // Causes native code to create an object that will be stored in native code and returned on a
  // subsequent call.  This can only be done with global objects and not local objects.
  native long nativeCreateContext(int val);

  // Takes the passed in context and stores copy in the context, returned as long.
  native long nativeCreateContextWithPromotion(ObjectTestHelper val);

  // Takes the passed in context and stores copy in the context, returned as long.
  native long nativeCreateContextWithCopy(ObjectTestHelper val);

  // Returns an ObjectTestHelper with intVal1 set to the sum of the vals.
  native ObjectTestHelper nativeCreateContextSetValToSum(int val1, int val2);

  // Returns a handle to a global object held within the inner native context.
  native ObjectTestHelper nativeQueryObject(long context);

  // Returns the handle to the native context, but simultaneously releases it in the process.
  native ObjectTestHelper nativeExtractObject(long context);

  // Destroys the entire native context possibly releasing the contained global object.
  // This would be representative of when an app would be shutting down.
  native void nativeDestroyContext(long context);
}
