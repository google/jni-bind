#include "javatests/com/jnibind/test/clamp.h"

#include <limits>

#include <gtest/gtest.h>

using ::jni::Clamp;

namespace {

TEST(Clamp, SimpleIntTests) {
  EXPECT_EQ(Clamp<int>(0), 0);
  EXPECT_EQ(Clamp<int>(1), 1);
  EXPECT_EQ(Clamp<int>(std::numeric_limits<int>::max() - 1),
            std::numeric_limits<int>::max() - 1);
  EXPECT_EQ(Clamp<int>(std::numeric_limits<int>::max()), 0);

  EXPECT_EQ(Clamp<int>(0, 1), 1);
  EXPECT_EQ(Clamp<int>(0, 5), 5);
  EXPECT_EQ(Clamp<int>(1, 1), 2);
  EXPECT_EQ(Clamp<int>(1, 5), 6);
}

}  // namespace
