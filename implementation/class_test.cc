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

#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::Class;
using ::jni::Constructor;
using ::jni::Extends;
using ::jni::Field;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Static;
using ::jni::test::JniTest;

////////////////////////////////////////////////////////////////////////////////
// Constructor Tests.
////////////////////////////////////////////////////////////////////////////////

/*
 Class unfortunately requires a matrix of viable arguments because it filters
 all the inputs and infers types through CTAD (which makes for a nicer API).

 Below tests compilation where:
 E => jni::Extends arg(s) present.
 C => jni::Constructor arg(s) present.
 S => jni::Static arg(s) present.
 M => jni::Method arg(s) present.
 F => jni::Field arg(s) present.

 To make the tests more optically simple to inspect, each of the above are
 encoded as a binary string.  i.e.
          ESCMF
          10001

For 10001, both Extends and a Field are used.
*/

static constexpr Class kParent{"kClass"};

static constexpr Extends e = Extends{kParent};
static constexpr Static s = Static{Method{"Foo", jni::Return{}}};
static constexpr Constructor c = Constructor{jint{}};
static constexpr Method m = Method{"Foo", jni::Return{}};
static constexpr Field f = Field{"field", int{}};

static constexpr Class k00000{"c"};
static constexpr Class k00001{"c", f};
static constexpr Class k00010{"c", m};
static constexpr Class k00011{"c", m, f};
static constexpr Class k00100{"c", s};
static constexpr Class k00101{"c", s, f};
static constexpr Class k00111{"c", s, m, f};
static constexpr Class k01000{"c", c};
static constexpr Class k01001{"c", c, f};
static constexpr Class k01010{"c", c, m};
static constexpr Class k01011{"c", c, m, f};
static constexpr Class k01100{"c", c, s};
static constexpr Class k01101{"c", c, s, f};
static constexpr Class k01111{"c", c, s, m, f};

static constexpr Class k10000{"c", e};
static constexpr Class k10001{"c", e, f};
static constexpr Class k10010{"c", e, m};
static constexpr Class k10011{"c", e, m, f};
static constexpr Class k10100{"c", e, s};
static constexpr Class k10101{"c", e, s, f};
static constexpr Class k10111{"c", e, s, m, f};
static constexpr Class k11000{"c", e, c};
static constexpr Class k11001{"c", e, c, f};
static constexpr Class k11010{"c", e, c, m};
static constexpr Class k11011{"c", e, c, m, f};
static constexpr Class k11100{"c", e, c, s};
static constexpr Class k11101{"c", e, c, s, f};
static constexpr Class k11111{"c", e, c, s, m, f};

// This test is just a smoke test for the above classes to silence clang.
TEST_F(JniTest, ClassTest_AllClassesInstantiate) {
  LocalObject<k00000> obj_k00000{};
  LocalObject<k00001> obj_k00001{};
  LocalObject<k00010> obj_k00010{};
  LocalObject<k00011> obj_k00011{};
  LocalObject<k00100> obj_k00100{};
  LocalObject<k00101> obj_k00101{};
  LocalObject<k00111> obj_k00111{};
  LocalObject<k01000> obj_k01000{123};
  LocalObject<k01001> obj_k01001{123};
  LocalObject<k01010> obj_k01010{123};
  LocalObject<k01011> obj_k01011{123};
  LocalObject<k01100> obj_k01100{123};
  LocalObject<k01101> obj_k01101{123};
  LocalObject<k01111> obj_k01111{123};

  LocalObject<k10000> obj_k10000{};
  LocalObject<k10001> obj_k10001{};
  LocalObject<k10010> obj_k10010{};
  LocalObject<k10011> obj_k10011{};
  LocalObject<k10100> obj_k10100{};
  LocalObject<k10101> obj_k10101{};
  LocalObject<k10111> obj_k10111{};
  LocalObject<k11000> obj_k11000{123};
  LocalObject<k11001> obj_k11001{123};
  LocalObject<k11010> obj_k11010{123};
  LocalObject<k11011> obj_k11011{123};
  LocalObject<k11100> obj_k11100{123};
  LocalObject<k11101> obj_k11101{123};
  LocalObject<k11111> obj_k11111{123};
}

////////////////////////////////////////////////////////////////////////////////
// Equality Tests.
////////////////////////////////////////////////////////////////////////////////

static constexpr Class kClass1{"classOne"};
static constexpr Class kClass1Prime{"classOne"};
static constexpr Class kClass2{"classTwo"};

static_assert(kClass1 == kClass1);
static_assert(kClass1 == kClass1Prime);
static_assert(!(kClass2 == kClass1Prime));

}  // namespace
