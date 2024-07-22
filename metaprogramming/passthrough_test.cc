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

#include "metaprogramming/passthrough.h"

#include <gtest/gtest.h>

using ::jni::metaprogramming::Passthrough;

template <typename T>
struct ReleaseObjectRef {
  static void Do(T& t) { t.val_0 = 0xabababab; }
};

static bool fail_happened = false;

struct A {
  A() {}
  A(int val_0) : val_0(val_0) {}
  A(int val_0, int val_1) : val_0(val_0), val_1(val_1) {}
  A(int val_0, int val_1, int val_2)
      : val_0(val_0), val_1(val_1), val_2(val_2) {}

  int Sum() { return val_0 + val_1 + val_2; }

  void Foo() {}

  ~A() {
    if (val_0 == 0xdeadbeef) {
      // never happens because custom dtor.
      fail_happened = true;
    }
  }

  int val_0 = 123;
  int val_1 = 456;
  int val_2 = 789;
};

bool operator==(const A& lhs, const A& rhs) {
  return lhs.val_0 == rhs.val_0 && lhs.val_1 == rhs.val_1 &&
         lhs.val_2 == rhs.val_2;
}

bool operator!=(A& lhs, A& rhs) { return !(lhs == rhs); }

namespace {

TEST(Passthrough, ConstructsI) {
  Passthrough<A> val{999};
  EXPECT_EQ(val, (A{999}));
}

TEST(Passthrough, ConstructsII) {
  Passthrough<A> val{999, 888};
  EXPECT_EQ(val, (A{999, 888}));
}

TEST(Passthrough, ConstructsIII) {
  Passthrough<A> val{999, 888, 777};
  EXPECT_EQ(val, (A{999, 888, 777}));
}

TEST(Passthrough, PeersThroughPointer) {
  Passthrough<A> val{};
  val->Foo();
}

TEST(Passthrough, PeersThroughDereference) {
  Passthrough<A> val{};
  (*val).Foo();
}

TEST(Passthrough, CustomDtorIsInvoked) {
  { Passthrough<A, ReleaseObjectRef<A>> val{0xdeadbeef}; }

  EXPECT_FALSE(fail_happened);

  { Passthrough<A> val{0xdeadbeef}; }
  EXPECT_TRUE(fail_happened);
}

}  // namespace
