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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import java.net.URL;
import java.net.URLClassLoader;
import java.util.jar.JarFile;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/**
 * Exercises basic behaviours with local objects, ranging from those passed down to JNI to those
 * constructed through rJNI.
 *
 * <p>This mostly focuses on lifecycle specific issues or issues relating to the use of objects or
 * signatures within methods or fields. For other signature tests see the method and fields tests.
 *
 * <p>Note, any object passed into JNI is always local.
 */
@RunWith(JUnit4.class)
public final class ClassLoaderTest {
  private static final String REMOTE_CLASS_PATH_KEY = "remote_class_path";
  private static final String JAR_CONTAINING_CLASSES =
      System.getenv("JAVA_RUNFILES")
          + "/_main/javatests/com/jnibind/test/ClassLoaderHelperClass_deploy.jar";

  private static JarFile jarFile;
  private static URL[] urlArray;
  private static URLClassLoader urlClassLoader;

  static {
    System.load(
        System.getenv("JAVA_RUNFILES")
            + "/_main/javatests/com/jnibind/test/libclass_loader_test_jni.so");
  }

  @BeforeClass
  public static void doSetup() throws Exception {
    jarFile = new JarFile(JAR_CONTAINING_CLASSES);
    urlArray = new URL[] {new URL("jar:file:" + JAR_CONTAINING_CLASSES + "!/")};
    urlClassLoader = URLClassLoader.newInstance(urlArray);
  }

  static native void jniTearDown();

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  @Test
  public void smokeTestEnsuringLoaderCanLoadClass() throws Exception {
    Class<?> classInstance = urlClassLoader.loadClass("com.jnibind.test.ClassLoaderHelperClass");
    assertNotNull(classInstance.getDeclaredConstructor().newInstance());
  }

  native Object jniBuildNewObjectsFromClassLoader(ClassLoader classLoader);

  @Test
  public void manipulateNewObjectsFromClassLoaderReturnsObjectWithValPlusFive() {
    Object newObject = jniBuildNewObjectsFromClassLoader(urlClassLoader);
    assertEquals("com.jnibind.test.ClassLoaderHelperClass", newObject.getClass().getName());
  }
}
