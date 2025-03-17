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

import java.util.ArrayList;
import java.util.List;
import org.junit.AfterClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/**
 * Tests demonstrating type erasure in JNI using templated types.
 * This shows how to use kJavaLangObject for handling generic types.
 */
@RunWith(JUnit4.class)
public final class TypeErasureTest {
  static {
    System.load(
        System.getenv("JAVA_RUNFILES")
            + "/_main/javatests/com/jnibind/test/libtype_erasure_test_jni.so");
  }

  static native void jniTearDown();

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }
  
  // Test generic method with type erasure
  @Test
  public void testGenericMethodWithTypeErasure() {
    // Test with different types
    Integer intValue = 42;
    String stringValue = "hello";
    Float floatValue = 3.14f;
    
    // Call JNI method that will use our generic echo method
    Object intResult = jniEchoGenericValue(intValue);
    Object stringResult = jniEchoGenericValue(stringValue);
    Object floatResult = jniEchoGenericValue(floatValue);
    
    // Verify original values preserved through type erasure
    assertThat(intResult).isEqualTo(intValue);
    assertThat(stringResult).isEqualTo(stringValue);
    assertThat(floatResult).isEqualTo(floatValue);
    
    // Verify type preservation
    assertThat(intResult).isInstanceOf(Integer.class);
    assertThat(stringResult).isInstanceOf(String.class);
    assertThat(floatResult).isInstanceOf(Float.class);
  }
  
  /**
   * Generic echo method that returns whatever is passed to it.
   * This demonstrates how Java's generics work with JNI's type erasure.
   * When called from native code, JNI will treat this as Object -> Object.
   */
  public <T> T echoGenericValue(T value) {
    return value;
  }
  
  /**
   * Helper method to get the size of any generic list.
   * When called from native code, the List<T> will be treated as List<Object>
   * but the JVM maintains the correct element types.
   */
  public <T> int getGenericListSize(List<T> list) {
    return list.size();
  }
  
  // Native methods
  native Object jniEchoGenericValue(Object value);
}
