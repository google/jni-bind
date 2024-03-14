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

import android.os.Environment;
import androidx.test.internal.runner.junit4.AndroidJUnit4ClassRunner;
import androidx.test.platform.io.PlatformTestStorageRegistry;
import com.google.android.apps.common.proguard.UsedByNative;
import dalvik.system.PathClassLoader;
import java.io.File;
import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * Exercises basic behaviours with local objects, ranging from those passed down to JNI to those
 * constructed through rJNI.
 *
 * <p>This mostly focuses on lifecycle specific issues or issues relating to the use of objects or
 * signatures within methods or fields. For other signature tests see the method and fields tests.
 *
 * <p>Note, any object passed into JNI is always local.
 */
@RunWith(AndroidJUnit4ClassRunner.class)
public final class ClassLoaderTest {
  private static final String REMOTE_CLASS_PATH_KEY = "remote_class_path";

  static {
    System.loadLibrary("class_loader_test_jni");
  }

  @UsedByNative("class_loader_test_jni.cc")
  ObjectTestHelper methodTakesLocalObjectReturnsNewObject(ObjectTestHelper object) {
    return new ObjectTestHelper(object.intVal1 + 5, object.intVal2 + 6, object.intVal3 + 7);
  }

  native ObjectTestHelper jniBuildNewObjectsFromClassLoader(ClassLoader classLoader);

  @Test
  public void manipulateNewObjectsFromClassLoaderReturnsObjectWithValPlusFive() {
    // See methodTakesClassLoaderReturnsNewObject.
    ObjectTestHelper objectFromJni =
        jniBuildNewObjectsFromClassLoader(ClassLoaderTest.class.getClassLoader());
    assertThat(objectFromJni.intVal1).isEqualTo(6);
    assertThat(objectFromJni.intVal2).isEqualTo(8);
    assertThat(objectFromJni.intVal3).isEqualTo(10);
  }

  native ObjectTestHelper jniBuildNewObjectsFromDefaultClassLoader(ClassLoader classLoader);

  @Test
  public void manipulateNewObjectsFromDefaultClassLoaderReturnsObjectWithValPlusFive() {
    // See methodTakesClassLoaderReturnsNewObject.
    ObjectTestHelper objectFromJni =
        jniBuildNewObjectsFromDefaultClassLoader(ClassLoaderTest.class.getClassLoader());
    assertThat(objectFromJni.intVal1).isEqualTo(7);
    assertThat(objectFromJni.intVal2).isEqualTo(9);
    assertThat(objectFromJni.intVal3).isEqualTo(11);
  }

  native int jniBuildsRemoteSubclassFromClassLoader(ClassLoader classLoader, int value);

  native int jniBuildsRemoteClassFromClassLoader(ClassLoader classLoader, int value);

  @Test
  public void remoteClassLoadersCanHandleSubclasses() throws Exception {
    File customClassJar =
        new File(
            Environment.getExternalStorageDirectory(),
            "googletest/test_runfiles/google3/"
                + PlatformTestStorageRegistry.getInstance()
                    .getInputArgs()
                    .get(REMOTE_CLASS_PATH_KEY));
    assertThat(customClassJar.exists()).isTrue();
    ClassLoader customLoader = new PathClassLoader(customClassJar.toString(), null);

    // The subclass returns the inverse of the passed in value.
    assertThat(jniBuildsRemoteSubclassFromClassLoader(customLoader, 1)).isEqualTo(-1);
    assertThat(jniBuildsRemoteSubclassFromClassLoader(customLoader, 2)).isEqualTo(-2);
    assertThat(jniBuildsRemoteSubclassFromClassLoader(customLoader, 3)).isEqualTo(-3);
    assertThat(jniBuildsRemoteSubclassFromClassLoader(customLoader, 4)).isEqualTo(-4);

    // The parent class should still return the same value.
    assertThat(jniBuildsRemoteClassFromClassLoader(customLoader, 1)).isEqualTo(1);
    assertThat(jniBuildsRemoteClassFromClassLoader(customLoader, 2)).isEqualTo(2);
    assertThat(jniBuildsRemoteClassFromClassLoader(customLoader, 3)).isEqualTo(3);
    assertThat(jniBuildsRemoteClassFromClassLoader(customLoader, 4)).isEqualTo(4);
  }
}
