#include "javatests/com/jnibind/test/modulo.h"

#include <limits>

#include <gtest/gtest.h>

using ::jni::Modulo;

namespace {

TEST(Modulo, SimpleIntTests) {
  EXPECT_EQ(Modulo<int>(0), 0);
  EXPECT_EQ(Modulo<int>(1), 1);
  EXPECT_EQ(Modulo<int>(std::numeric_limits<int>::max() - 1),
            std::numeric_limits<int>::max() - 1);
  EXPECT_EQ(Modulo<int>(std::numeric_limits<int>::max()), 0);

  EXPECT_EQ(Modulo<int>(0, 1), 1);
  EXPECT_EQ(Modulo<int>(0, 5), 5);
  EXPECT_EQ(Modulo<int>(1, 1), 2);
  EXPECT_EQ(Modulo<int>(1, 5), 6);
}

}  // namespace
