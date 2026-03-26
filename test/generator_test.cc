#include <gtest/gtest.h>

#include "generator/market_data.h"

class MarketDataGeneratorTest : public ::testing::Test {
   protected:
    market_data::MarketDataGenerator generator;
};

TEST_F(MarketDataGeneratorTest, AddsNewSymbolToTickers) {
    int initial_size = generator.get_symbols().size();
    generator.add_symbol("TestTicker", 50000);
    int current_size = generator.get_symbols().size();
    EXPECT_EQ(current_size, initial_size + 1);
}

TEST_F(MarketDataGeneratorTest, RemovesSymbolFromTickersByName) {
    generator.add_symbol("TestTicker", 50000);
    int initial_size = generator.get_symbols().size();
    generator.remove_symbol("TestTicker");
    int current_size = generator.get_symbols().size();
    EXPECT_EQ(current_size, initial_size - 1);
}

TEST_F(MarketDataGeneratorTest, GenerateKeepsPricesNonNegative) {
    generator.add_symbol("TestTicker", 1);
    for (int i = 0; i < 100; i++) {
        generator.generate();
        for (const auto& tick : generator.get_symbols()) {
            EXPECT_GE(tick.price_in_cents, 0);
        }
    }
}

// To make this test 100% deterministic we have to check for timestamp changes in addition to price
// changes because there is a slim random chance that all default tickers random walk to their
// original price.
TEST_F(MarketDataGeneratorTest, GenerateActuallyChangesPrices) {
    auto before = generator.get_symbols();
    bool any_changed = false;

    for (int i = 0; i < 100; i++) {
        generator.generate();
    }

    auto after = generator.get_symbols();
    for (size_t i = 0; i < before.size(); i++) {
        if (before[i].price_in_cents != after[i].price_in_cents) {
            any_changed = true;
            break;
        }
    }

    if (!any_changed) {
        if (before[0].timestamp != after[0].timestamp) {
            any_changed = true;
        }
    }
    EXPECT_TRUE(any_changed);
}
