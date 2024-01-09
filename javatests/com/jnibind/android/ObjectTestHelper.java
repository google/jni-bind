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

/** Simple object for use with RJni object tests. */
public class ObjectTestHelper {
  public ObjectTestHelper() {
    intVal1 = 0;
    intVal2 = 0;
    intVal3 = 0;
    objectVal = null;
  }

  public ObjectTestHelper(Object objectVal) {
    this.objectVal = objectVal;
  }

  public ObjectTestHelper(int intVal) {
    this.intVal1 = intVal;
  }

  public ObjectTestHelper(int intVal, int intVal2) {
    this.intVal1 = intVal;
    this.intVal2 = intVal2;
  }

  public ObjectTestHelper(int intVal1, int intVal2, int intVal3) {
    this.intVal1 = intVal1;
    this.intVal2 = intVal2;
    this.intVal3 = intVal3;
  }

  public ObjectTestHelper returnNewObjectWithFieldSetToSum(int val1, int val2) {
    return new ObjectTestHelper(val1 + val2);
  }

  public void foo() {}

  public ObjectTestHelper returnNewObjectWithFieldSetToSum(ObjectTestHelper rhs) {
    return new ObjectTestHelper(
        this.intVal1 + rhs.intVal1, this.intVal2 + rhs.intVal2, this.intVal3 + rhs.intVal3);
  }

  public boolean isEqualTo(ObjectTestHelper rhs) {
    return this.intVal1 == rhs.intVal1
        && this.intVal2 == rhs.intVal2
        && this.intVal3 == rhs.intVal3;
  }

  public int intVal1;
  public int intVal2;
  public int intVal3;
  public Object objectVal;
}
