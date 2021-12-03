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

import static com.google.common.truth.Truth.assertThat;
import static org.mockito.Mockito.verify;

import androidx.test.internal.runner.junit4.AndroidJUnit4ClassRunner;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnit;
import org.mockito.junit.MockitoRule;

@RunWith(AndroidJUnit4ClassRunner.class)
public final class StringTest {
  @Rule public final MockitoRule mockito = MockitoJUnit.rule();
  @Mock public StringTestHelper rJniStringTestHelper;

  static {
    System.loadLibrary("string_test_jni");
  }

  native void jniVoidMethodTakesString(StringTestHelper helper, String s);

  native void jniVoidMethodTakesTwoStrings(StringTestHelper helper, String s1, String s2);

  native void jniVoidMethodTakesFiveStrings(
      StringTestHelper helper, String s1, String s2, String s3, String s4, String s5);

  // Calls each of the above native methods which will then call the similarly named method on the
  // StringTestHelper.  E.g. jniVoidMethodTakesString calls voidMethodTakesString.
  @Test
  public void callsVoidMethodsThatTakeAString() {
    jniVoidMethodTakesString(rJniStringTestHelper, "SimpleTestString");
    jniVoidMethodTakesString(rJniStringTestHelper, "String with punctuation!@?@$!$#%$");
    jniVoidMethodTakesTwoStrings(rJniStringTestHelper, "Str1", "Str2");
    jniVoidMethodTakesFiveStrings(rJniStringTestHelper, "Str1", "Str2", "Str3", "Str4", "Str5");

    verify(rJniStringTestHelper).voidMethodTakesString("SimpleTestString");
    verify(rJniStringTestHelper).voidMethodTakesString("String with punctuation!@?@$!$#%$");
    verify(rJniStringTestHelper).voidMethodTakesTwoStrings("Str1", "Str2");
    verify(rJniStringTestHelper).voidMethodTakesFiveStrings("Str1", "Str2", "Str3", "Str4", "Str5");
  }

  native String jniStringMethodTakesString(StringTestHelper helper, String s);

  native String jniStringMethodTakesTwoStrings(StringTestHelper helper, String s1, String s2);

  native String jniStringMethodTakesFiveStrings(
      StringTestHelper helper, String s1, String s2, String s3, String s4, String s5);

  // Calls each of the above native methods which will then call the similarly named method on the
  // StringTestHelper.  The return will be the contatenation of each of the input arugments.
  @Test
  public void forwardsStringsProperly() {
    jniStringMethodTakesString(rJniStringTestHelper, "SimpleTestString");
    jniStringMethodTakesString(rJniStringTestHelper, "String with punctuation!@?@$!$#%$");
    jniStringMethodTakesTwoStrings(rJniStringTestHelper, "Str1", "Str2");
    jniStringMethodTakesFiveStrings(rJniStringTestHelper, "Str1", "Str2", "Str3", "Str4", "Str5");

    verify(rJniStringTestHelper).stringMethodTakesString("SimpleTestString");
    verify(rJniStringTestHelper).stringMethodTakesString("String with punctuation!@?@$!$#%$");
    verify(rJniStringTestHelper).stringMethodTakesTwoStrings("Str1", "Str2");
    verify(rJniStringTestHelper)
        .stringMethodTakesFiveStrings("Str1", "Str2", "Str3", "Str4", "Str5");
  }

  //   The above tests validate everything is passed through correctly.  The
  //   following statements test the same code, but exercise real
  //   implementation.  The implementation is always the same, take all the
  //   strings and return the concatenation.
  @Test
  public void passesForwardsAndReturnsStrings() {
    StringTestHelper testHelper = new StringTestHelper();
    assertThat(jniStringMethodTakesString(testHelper, "SimpleTestString"))
        .isEqualTo("SimpleTestString");
    jniStringMethodTakesString(testHelper, "");
    assertThat(jniStringMethodTakesString(testHelper, "String with punctuation!@?@$!$#%"))
        .isEqualTo("String with punctuation!@?@$!$#%");
    assertThat(
            jniStringMethodTakesString(
                testHelper, jniStringMethodTakesTwoStrings(testHelper, "Str1", "+Str2")))
        .isEqualTo("Str1+Str2");

    assertThat(
            jniStringMethodTakesFiveStrings(testHelper, "Str1", "+Str2", "+Str3", "+Str4", "+Str5"))
        .isEqualTo("Str1+Str2+Str3+Str4+Str5");
  }

  /*
   * TODO(b/174272629):  This should be simpler to do.  Right now there are no overloads for functions
   * returning string, meaning an explicit conversion from string_view must be made, which means
   * extra logic must be added just to represent an empty string.
   */
  /*
  @Test
  public void nullStringTests() {
    jniStringMethodTakesString(rJniStringTestHelper, null);
    verify(rJniStringTestHelper).stringMethodTakesString(null);
  }
  */
}
