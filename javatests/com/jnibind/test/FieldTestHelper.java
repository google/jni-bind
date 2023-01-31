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
 * This object is used to set mock expectations for FieldTest as inner classes do not support
 * mocking and the class definition must match the filename.
 */
public class FieldTestHelper {
  public int intField = 0;
  public float floatField = 0;
  public double doubleField = 0;
  public String stringField = "";

  FieldTestHelper() {}

  FieldTestHelper(int intField, float floatField, double doubleField) {
    this.intField = intField;
    this.floatField = floatField;
    this.doubleField = doubleField;
  }

  boolean isEqualTo(FieldTestHelper rhs) {
    return intField == rhs.intField
        && floatField == rhs.floatField
        && doubleField == rhs.doubleField
        && stringField.equals(rhs.stringField);
  }
}
