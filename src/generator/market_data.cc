#include "generator/market_data.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <vector>

#include "config.h"

namespace market_data {

void generate() {
    for (auto& tick : config::tickers) {
        random_walk(tick);
    }
}

void random_walk(Tick& ticker) {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_int_distribution<> dist(-1, 1);
    int step = dist(mt);
    ticker.price_in_cents += step;
    if (ticker.price_in_cents < 0) ticker.price_in_cents = 0;
}

void add_symbol(std::string name, double price_in_cents) {
    auto now = std::chrono::system_clock::now();
    auto timestamp_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    config::tickers.emplace_back(name, price_in_cents, timestamp_ms);
}

int remove_symbol(std::string name) {
    auto it = std::find_if(config::tickers.begin(), config::tickers.end(),
                           [&name](const market_data::Tick& t) { return t.name == name; });
    if (it == config::tickers.end()) {
        return -1;
    }
    config::tickers.erase(it);

    return 0;
}

std::vector<Tick> get_symbols() { return config::tickers; }

}  // namespace market_data
