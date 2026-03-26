#ifndef MARKET_DATA_H
#define MARKET_DATA_H

#include <cstdint>
#include <random>
#include <string>
#include <vector>

namespace market_data {

struct Tick {
    std::string name;
    int price_in_cents;
    uint64_t timestamp;
};

class MarketDataGenerator {
   public:
    MarketDataGenerator();
    void generate();
    void add_symbol(std::string name, double price);
    std::vector<Tick> get_symbols();
    int remove_symbol(std::string name);

   private:
    void random_walk(Tick& ticker);
    std::random_device rd_;
    std::mt19937 mt_;
    std::uniform_int_distribution<int> dist_{-1, 1};
};
}  // namespace market_data
#endif
