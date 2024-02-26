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

import static org.junit.Assert.assertTrue;

import org.junit.AfterClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/**
 * Exercises Builder pattern commonly used in construction.
 *
 * <p>e.g. Use cases like: Builder builder = new Builder(); builder.setX(1).setY(2).setZ(3);
 * builder.build();
 */
@RunWith(JUnit4.class)
public class BuilderTest {
  static {
    System.load(
        System.getenv("JAVA_RUNFILES")
            + "/__main__/javatests/com/jnibind/test/libbuilder_jni.so");
  }

  static native void nativeJniTeardown();

  @AfterClass
  public static void doShutDown() {
    nativeJniTeardown();
  }

  public static class Builder {
    private int valOne = 0;
    private int valTwo = 0;
    private int valThree = 0;

    public Builder() {}

    public Builder setOne(int val) {
      valOne = val;
      return this;
    }

    public Builder setTwo(int val) {
      valTwo = val;
      return this;
    }

    public Builder setThree(int val) {
      valThree = val;
      return this;
    }

    public ObjectTestHelper build() {
      return new ObjectTestHelper(valOne, valTwo, valThree);
    }
  }

  native ObjectTestHelper useBuilderToCreateObject();

  @Test
  public void constructsExpectedObjectWithSimpleBuilder() {
    assertTrue(useBuilderToCreateObject().isEqualTo(new ObjectTestHelper(111, 222, 333)));
  }
}
