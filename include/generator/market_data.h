#ifndef MARKET_DATA_H
#define MARKET_DATA_H

#include <cstdint>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "generator/thread.h"
#include "network/multicast.h"

namespace market_data {

struct Tick {
    std::string name;
    int price_in_cents;
    uint64_t timestamp;
};

class MarketDataGenerator {
   public:
    MarketDataGenerator();
    void set_multicast_server(std::shared_ptr<network::MulticastServer> server);
    void add_symbol(std::string name, int price);
    int remove_symbol(std::string name);
    std::vector<Tick> get_symbols();
    void stop_all();

   private:
    void random_walk(int& price);
    void tick_worker(std::string name, int initial_price);

    std::shared_ptr<network::MulticastServer> multicast_server_;
    std::map<std::string, Tick> tickers_;
    std::map<std::string, Thread> threads_;
    std::random_device rd_;
    std::mt19937 mt_;
    std::uniform_int_distribution<int> dist_{-1, 1};
};

}  // namespace market_data
#endif
