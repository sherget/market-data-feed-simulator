#ifndef MARKET_DATA_H
#define MARKET_DATA_H

#include <cstdint>
#include <string>
#include <vector>

namespace market_data {
struct Tick {
    std::string name;
    int price_in_cents;
    uint64_t timestamp;
};

void generate();

void random_walk(Tick& ticker);

void add_symbol(std::string name, double price);

std::vector<Tick> get_symbols();

int remove_symbol(std::string name);

}  // namespace market_data
#endif
