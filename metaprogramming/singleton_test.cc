#include "metaprogramming/singleton.h"

#include <memory>

#include <gtest/gtest.h>

using ::jni::metaprogramming::Singleton;

namespace {

TEST(Singleton, RetrievesValueForTrivialType) {
  Singleton<int>::val = 100;
  EXPECT_EQ(Singleton<int>::val, 100);
}

TEST(Singleton, RetrievesComplexType) {
  struct A {
    int val;

    bool operator==(const A& rhs) const { return val == rhs.val; }
  };

  Singleton<A>::val = A{.val = 100};
  EXPECT_EQ((Singleton<A>::val), A{.val = 100});
}

TEST(Singleton, RetrievesComplexTypeNonTrivialCtor) {
  struct A {
    std::unique_ptr<int> ptr;

    bool operator==(const A& rhs) const { return ptr == rhs.ptr; }
  };

  Singleton<A>::val = A{.ptr = std::make_unique<int>(123)};
  EXPECT_NE((Singleton<A>::val.ptr), nullptr);
}

TEST(Singleton, AllowsTemplatedType) {
  Singleton<std::unique_ptr<int>>::val = std::make_unique<int>(123);
  EXPECT_EQ(*(Singleton<std::unique_ptr<int>>::val), 123);
}

}  // namespace
