// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package com.jnibind.test;

/**
 * A subclass of {@link ClassLoaderHelperClass} that returns a different value.
 *
 * <p>This helps test when subclasses are placed in side of native wrappers for their parent
 * classes.
 */
class ClassLoaderHelperSubclass extends ClassLoaderHelperClass {
  public ClassLoaderHelperSubclass(int intVal) {
    super(intVal);
  }

  /** Returns the inverse of the value passed into the constructor */
  @Override
  public int getValue() {
    return -super.getValue();
  }

  /**
   * The wrapper currently does not have information about relationships between classes, so this
   * method explicitly returns an object as its parent class.
   */
  public ClassLoaderHelperClass castToParent() {
    return this;
  }
}
