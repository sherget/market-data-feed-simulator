#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "config.h"
#include "generator/market_data.h"
#include "network/udp_multicast.h"

void helper_print_tickers(std::vector<market_data::Tick> tickers) {
    for (const auto& tick : tickers) {
        std::cout << tick.name << " " << tick.price_in_cents << " " << tick.timestamp << "\n";
    }
}

void broadcaster(std::vector<market_data::Tick> tickers,
                 multicast::MulticastServer& multicast_server) {
    for (const auto& tick : tickers) {
        // use json dependency or write a small serializer (probably a good cpp syntax practice)
        std::string message = "{\"name\":" + tick.name +
                              ",\"price\":" + std::to_string(tick.price_in_cents) +
                              ",\"timestamp\":" + std::to_string(tick.timestamp) + "}\n";
        multicast_server.broadcast(message);
    }
}

int main() {
    market_data::MarketDataGenerator generator;
    multicast::MulticastServer multicast_server;
    multicast_server.run();
    while (true) {
        generator.generate();
        broadcaster(generator.get_symbols(), multicast_server);
        // helper_print_tickers(generator.get_symbols());
        std::this_thread::sleep_for(std::chrono::milliseconds(config::tick_interval));
    }
}
