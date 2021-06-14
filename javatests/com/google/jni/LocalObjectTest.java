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
 * Exercises basic behaviours with local objects, ranging from those passed down to JNI to those
 * constructed through rJNI.
 *
 * <p>This mostly focuses on lifecycle specific issues or issues relating to the use of objects or
 * signatures within methods or fields. For other signature tests see the method and fields tests.
 *
 * <p>Note, any object passed into JNI is always local.
 */
@RunWith(AndroidJUnit4ClassRunner.class)
public final class LocalObjectTest {
  static {
    System.loadLibrary("local_object_test_jni");
  }

  native ObjectTestHelper jniCreateNewObject();

  @Test
  public void createNewLocalObjectUsingNoArgConstructor() {
    assertThat(jniCreateNewObject()).isNotEqualTo(null);
  }

  native ObjectTestHelper jniCreateNewLocalObjectFromObject(Object object);

  @Test
  public void createNewLocalObjectFromObject() {
    Object innerObject = new Object();
    ObjectTestHelper object = jniCreateNewLocalObjectFromObject(innerObject);
    assertThat(object).isNotEqualTo(null);
    assertThat(object.objectVal).isEqualTo(innerObject);
  }

  native ObjectTestHelper jniCreateNewLocalObjectSetIntVal123();

  @Test
  public void createNewLocalObjectSetIntVal123() {
    ObjectTestHelper object = jniCreateNewLocalObjectSetIntVal123();
    assertThat(object).isNotEqualTo(null);
    assertThat(object.intVal1).isEqualTo(1);
    assertThat(object.intVal2).isEqualTo(2);
    assertThat(object.intVal3).isEqualTo(3);
  }

  @UsedByNative("local_object_test_jni.cc")
  ObjectTestHelper methodTakesLocalObjectReturnsNewObject(ObjectTestHelper object) {
    return new ObjectTestHelper(object.intVal1 + 5, object.intVal2 + 6, object.intVal3 + 7);
  }

  native ObjectTestHelper jniBuildNewObjectsFromExistingObjects(
      LocalObjectTest thisTestObject, ObjectTestHelper testHelperObject);

  @Test
  public void manipulateExistingObjectReturnSameObjectWithValPlusFive() {
    // See methodTakesLocalObjectReturnsNewObject.
    ObjectTestHelper objectFromJni =
        jniBuildNewObjectsFromExistingObjects(this, new ObjectTestHelper(3, 4, 5));
    assertThat(objectFromJni.intVal1).isEqualTo(8);
    assertThat(objectFromJni.intVal2).isEqualTo(10);
    assertThat(objectFromJni.intVal3).isEqualTo(12);
  }

  native ObjectTestHelper jniManipulateNewLocalObjectSetIntVal238(LocalObjectTest thisTestObject);

  @Test
  public void manipulatesMovedFromLocalObjectSetIntVal238() {
    ObjectTestHelper object = jniManipulateNewLocalObjectSetIntVal238(this);
    assertThat(object).isNotEqualTo(null);
    assertThat(object.intVal1).isEqualTo(7);
    assertThat(object.intVal2).isEqualTo(9);
    assertThat(object.intVal3).isEqualTo(15);
  }

  // Identical to above except creates a new Java object but passes as expiring
  // rvalue.  Materialized value (passed to Java) is {1, 5, 9};
  native ObjectTestHelper jniMaterializeNewLocalObjectSetIntVal159(LocalObjectTest thisTestObject);

  @Test
  public void materializeNewLocalObjectSetIntVal159() {
    ObjectTestHelper object = jniMaterializeNewLocalObjectSetIntVal159(this);
    assertThat(object).isNotEqualTo(null);
    assertThat(object.intVal1).isEqualTo(6);
    assertThat(object.intVal2).isEqualTo(11);
    assertThat(object.intVal3).isEqualTo(16);
  }
}
