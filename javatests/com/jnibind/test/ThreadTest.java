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

import static org.mockito.Mockito.verify;

import org.junit.AfterClass;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnit;
import org.mockito.junit.MockitoRule;

@RunWith(JUnit4.class)
public final class ThreadTest {
  private static final String TAG = "ThreadTest";

  @Mock public ObjectTestHelper objectTestHelper;
  @Rule public final MockitoRule mockito = MockitoJUnit.rule();

  static {
    System.load(
        System.getenv("JAVA_RUNFILES")
            + "/_main/javatests/com/jnibind/test/libthread_test_jni.so");
  }

  public ThreadTest() {
    DoSetup();
  }

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  @Test
  public void runsThreadedWork() {
    RunsThreadedWorkOnObject(objectTestHelper);
    verify(objectTestHelper).foo();
  }

  // Setup JvmRef.
  native void DoSetup();

  // Tears down the JvmRef.
  static native void jniTearDown();

  // Creates a thread, waits until it finishes some work, uses passed object.
  native void RunsThreadedWorkOnObject(ObjectTestHelper objectTestHelper);
}
