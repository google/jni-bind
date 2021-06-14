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

package com.google.jni;

import static com.google.common.truth.Truth.assertThat;

import androidx.test.internal.runner.junit4.AndroidJUnit4ClassRunner;
import com.google.android.apps.common.proguard.UsedByNative;
import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * Exercises basic behaviours with global objects, ranging from those passed down to JNI to those
 * constructed through rJNI.
 *
 * <p>This mostly focuses on lifecycle specific issues or issues relating to the use of objects or
 * signatures within methods or fields. For other signature tests see the method and fields tests.
 *
 * <p>Note, any object passed into JNI is always local.
 */
@RunWith(AndroidJUnit4ClassRunner.class)
public final class GlobalObjectTest {
  static {
    System.loadLibrary("global_object_test_jni");
  }

  native ObjectTestHelper jniCreateNewObject();

  @Test
  public void createNewGlobalObjectUsingNoArgConstructor() {
    assertThat(jniCreateNewObject()).isNotEqualTo(null);
  }

  native ObjectTestHelper jniCreateNewGlobalObjectSetIntVal123();

  @Test
  public void createNewGlobalObjectSetIntVal123() {
    ObjectTestHelper object = jniCreateNewGlobalObjectSetIntVal123();
    assertThat(object).isNotEqualTo(null);
    assertThat(object.intVal1).isEqualTo(1);
    assertThat(object.intVal2).isEqualTo(2);
    assertThat(object.intVal3).isEqualTo(3);
  }

  // Takes a test helper and returns the output of |methodTakesGlobalObjectReturnsNewObject|.
  native ObjectTestHelper jniBuildNewObjectsFromExistingObjects(
      GlobalObjectTest testHelperObject, ObjectTestHelper objectToMutate);

  @UsedByNative("global_object_test_jni.cc")
  ObjectTestHelper methodTakesGlobalObjectReturnsNewObject(ObjectTestHelper object) {
    return new ObjectTestHelper(object.intVal1 + 5, object.intVal2 + 6, object.intVal3 + 7);
  }

  @Test
  public void manipulateExistingObjectReturnSameObjectWithValPlusFive() {
    // intVal1 = 3 + 5 = 8.  See methodTakesGlobalObjectReturnsNewObject.
    ObjectTestHelper objectFromJni =
        jniBuildNewObjectsFromExistingObjects(this, new ObjectTestHelper(3, 4, 5));
    assertThat(objectFromJni.intVal1).isEqualTo(8);
    assertThat(objectFromJni.intVal2).isEqualTo(10);
    assertThat(objectFromJni.intVal3).isEqualTo(12);
  }

  /*
   * TODO(b/143908983):  This test is currently broken.
  @Test
  public void manipulateExistingObjectReturnSameObjectWithValPlusFive() {
    // intVal = 3 + 5 = 8.  See methodTakesGlobalObjectReturnsNewObject.
    assertThat(jniBuildNewObjectsFromExistingObjects(this, new ObjectTestHelper(3)).intVal)
        .isEqualTo(8);
  }
  */

  native ObjectTestHelper jniManipulateNewGlobalObjectSetIntVal238(GlobalObjectTest thisTestObject);

  @Test
  public void manipulatesMovedFromGlobalObjectSetIntVal238() {
    ObjectTestHelper object = jniManipulateNewGlobalObjectSetIntVal238(this);
    assertThat(object).isNotEqualTo(null);
    assertThat(object.intVal1).isEqualTo(7);
    assertThat(object.intVal2).isEqualTo(9);
    assertThat(object.intVal3).isEqualTo(15);
  }

  // Identical to above except creates a new Java object but passes as expiring
  // rvalue.  Materialized value (passed to Java) is {1, 5, 9};
  native ObjectTestHelper jniMaterializeNewGlobalObjectSetIntVal159(
      GlobalObjectTest thisTestObject);

  @Test
  public void materializeNewGlobalObjectSetIntVal159() {
    ObjectTestHelper object = jniMaterializeNewGlobalObjectSetIntVal159(this);
    assertThat(object).isNotEqualTo(null);
    assertThat(object.intVal1).isEqualTo(6);
    assertThat(object.intVal2).isEqualTo(11);
    assertThat(object.intVal3).isEqualTo(16);
  }
}
