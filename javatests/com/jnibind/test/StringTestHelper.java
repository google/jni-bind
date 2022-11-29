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

/**
 * This object is used to set mock expectations for StringTest as inner classes do not support
 * mocking and the class definition must match the filename.
 */
public class StringTestHelper {
  /** Void Methods. */

  public void voidMethodTakesString(String s) {}


  public void voidMethodTakesTwoStrings(String s1, String s2) {}


  public void voidMethodTakesFiveStrings(String s1, String s2, String s3, String s4, String s5) {}

  /** String Methods. */

  public String stringMethodTakesString(String s) {
    return s;
  }


  public String stringMethodTakesTwoStrings(String s1, String s2) {
    return s1 + s2;
  }


  public String stringMethodTakesFiveStrings(
      String s1, String s2, String s3, String s4, String s5) {
    return s1 + s2 + s3 + s4 + s5;
  }
}
