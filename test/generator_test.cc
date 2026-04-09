#include <gtest/gtest.h>

#include "generator/market_data.h"

class MarketDataGeneratorTest : public ::testing::Test {
   protected:
    market_data::MarketDataGenerator generator;
};

TEST_F(MarketDataGeneratorTest, AddsNewSymbol) {
    int initial_size = generator.get_symbols().size();
    generator.add_symbol("TestTicker", 50000);
    int current_size = generator.get_symbols().size();
    EXPECT_EQ(current_size, initial_size + 1);
}

TEST_F(MarketDataGeneratorTest, RemovesSymbolByName) {
    generator.add_symbol("TestTicker", 50000);
    int initial_size = generator.get_symbols().size();
    generator.remove_symbol("TestTicker");
    int current_size = generator.get_symbols().size();
    EXPECT_EQ(current_size, initial_size - 1);
}

TEST_F(MarketDataGeneratorTest, RemoveNonExistentSymbolReturnsError) {
    int result = generator.remove_symbol("NonExistent");
    EXPECT_EQ(result, -1);
}

TEST_F(MarketDataGeneratorTest, GetSymbolsReturnsAllSymbols) {
    generator.add_symbol("AAPL", 10000);
    generator.add_symbol("GOOG", 20000);
    auto symbols = generator.get_symbols();
    EXPECT_EQ(symbols.size(), 2);
}

TEST_F(MarketDataGeneratorTest, DuplicateSymbolNotAdded) {
    generator.add_symbol("AAPL", 10000);
    int size_before = generator.get_symbols().size();
    generator.add_symbol("AAPL", 20000);
    int size_after = generator.get_symbols().size();
    EXPECT_EQ(size_before, size_after);
}

TEST_F(MarketDataGeneratorTest, StopAllClearsSymbols) {
    generator.add_symbol("AAPL", 10000);
    generator.add_symbol("GOOG", 20000);
    EXPECT_EQ(generator.get_symbols().size(), 2);
    generator.stop_all();
    EXPECT_EQ(generator.get_symbols().size(), 0);
}
