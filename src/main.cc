#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "config.h"
#include "generator/market_data.h"

void helper_print_tickers(std::vector<market_data::Tick> tickers) {
    for (const auto& tick : tickers) {
        std::cout << tick.name << " " << tick.price_in_cents << " " << tick.timestamp << "\n";
    }
}

int main() {
    market_data::MarketDataGenerator generator;
    while (true) {
        generator.generate();
        helper_print_tickers(generator.get_symbols());
        std::cout << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(config::tick_interval));
    }
}
