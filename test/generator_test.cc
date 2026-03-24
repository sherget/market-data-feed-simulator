#include "generator/market_data.h"
#include <gtest/gtest.h>

TEST(GeneratorTest, HelloWorld) {
  EXPECT_EQ(market_data::greet("Test"), "Hello, Test!");
}
