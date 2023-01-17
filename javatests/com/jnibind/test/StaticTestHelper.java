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

/**
 * Static callbacks for StaticTest.
 *
 * <p>Unfortunately, Mockito doesn't offer an easy mechanism for mocking static methods, so we just
 * set some static variables and place expectations on them.
 */
final class StaticTestHelper {
  private StaticTestHelper() {}

  /** Method Tests. */
  public static boolean voidCalled = false;

  public static void voidFunc() {
    voidCalled = true;
  }

  public static byte byteFunc() {
    return 123;
  }

  public static char charFunc() {
    return 'a';
  }

  public static short shortFunc() {
    return 30;
  }

  public static int intFunc() {
    return 789;
  }

  public static long longFunc() {
    return 1011;
  }

  public static float floatFunc() {
    return 1213.0f;
  }

  public static double doubleFunc() {
    return 1415.0;
  }

  public static ObjectTestHelper objectFunc() {
    return new ObjectTestHelper(1, 2, 3);
  }

  public static String complexFunc(int a, float b, String c, long[][] d) {
    if (a == 1 && b == 2 && "3".equals(c) && d[4][5] == 6) {
      return "COMPLEX";
    } else {
      return "FAIL";
    }
  }

  /** Field Tests. */
  public static boolean booleanField = false;

  public static byte byteField = 0;
  public static char charField = 0;
  public static short shortField = 0;
  public static int intField = 0;
  public static long longField = 0;
  public static float floatField = 0;
  public static double doubleField = 0;
  public static ObjectTestHelper objectField = null;
}
