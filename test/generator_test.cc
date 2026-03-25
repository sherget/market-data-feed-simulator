#include <gtest/gtest.h>

#include "config.h"
#include "generator/market_data.h"

TEST(SymbolManagement, AddsNewSymbolToTickers) {
    int initial_size = config::tickers.size();
    market_data::add_symbol("TestTicker", 50000);
    int current_size = config::tickers.size();
    EXPECT_EQ(current_size, initial_size + 1);
}

TEST(SymbolManagement, RemovesSymbolFromTickersByName) {
    market_data::add_symbol("TestTicker", 50000);
    int initial_size = config::tickers.size();
    market_data::remove_symbol("TestTicker");
    int current_size = config::tickers.size();
    EXPECT_EQ(current_size, initial_size - 1);
}

TEST(RandomWalkTest, ValueStaysPositive) {
    market_data::Tick tick = {"TestTicker", 1, 0};
    int initial_value = tick.price_in_cents;
    for (int i = 0; i < 1000; i++) {
        market_data::random_walk(tick);
    }
    int current_value = tick.price_in_cents;
    EXPECT_GT(current_value, 0);
}

TEST(RandomWalkTest, PriceChangesByAtMostOne) {
    market_data::Tick tick = {"TestTicker", 10, 0};
    int initial_value = tick.price_in_cents;
    market_data::random_walk(tick);
    int difference = tick.price_in_cents - 10;
    EXPECT_TRUE(difference >= -1 && difference <= 1);
}
