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

package com.jnibind.android;

import static org.mockito.Mockito.verify;

import androidx.test.internal.runner.junit4.AndroidJUnit4ClassRunner;
import org.junit.AfterClass;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnit;
import org.mockito.junit.MockitoRule;

@RunWith(AndroidJUnit4ClassRunner.class)
public class ThreadTest {
  private static final String TAG = "ThreadTest";

  @Mock public ObjectTestHelper objectTestHelper;
  @Rule public final MockitoRule mockito = MockitoJUnit.rule();

  static {
    System.loadLibrary("thread_test_jni");
  }

  public ThreadTest() {}

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  @Test
  public void runsThreadedWork() {
    runsThreadedWorkOnObject(this);
    verify(objectTestHelper).foo();
  }

  // Tears down the JvmRef.
  static native void jniTearDown();

  // Creates a thread, waits until it finishes some work, uses passed object.
  // native void RunsThreadedWorkOnObject(ObjectTestHelper objectTestHelper);
  native void runsThreadedWorkOnObject(ThreadTest testFixture);
}
