#include <gtest/gtest.h>

#include "generator/market_data.h"

TEST(GeneratorTest, HelloWorld) { EXPECT_EQ(market_data::greet("Test"), "Hello, Test!"); }
