#include <iostream>
#include <vector>

#include "generator/market_data.h"

void helper_print_tickers(std::vector<market_data::Tick> tickers) {
    for (const auto& tick : tickers) {
        std::cout << tick.name << " " << tick.price_in_cents << " " << tick.timestamp << "\n";
    }
}

int main() {
    while (true) {
        market_data::generate();
        helper_print_tickers(market_data::get_symbols());
        std::cout << "\n";
    }
}
