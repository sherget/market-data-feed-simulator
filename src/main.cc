#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "config.h"
#include "generator/market_data.h"
#include "network/multicast.h"
#include "network/multicast_subscriber.h"

void helper_print_tickers(std::vector<market_data::Tick> tickers) {
    for (const auto& tick : tickers) {
        std::cout << tick.name << " " << tick.price_in_cents << " " << tick.timestamp << "\n";
    }
}
void publish(std::vector<market_data::Tick> tickers, network::MulticastServer& multicast_server) {
    for (const auto& tick : tickers) {
        // TODO: Use json dependency or write a small serializer
        std::string message = "{\"name\":" + tick.name +
                              ",\"price\":" + std::to_string(tick.price_in_cents) +
                              ",\"timestamp\":" + std::to_string(tick.timestamp) + "}\n";
        multicast_server.broadcast(message);
    }
}

void print_usage(const char* program) {
    std::cout << "Usage: " << program << " <mode> [tickers...]\n";
    std::cout << "Modes:\n";
    std::cout << " publisher - Start market data generator and broadcaster\n";
    std::cout << " subscriber [tickers...] - Subscribe to multicast feed\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
    }

    std::string mode = argv[1];

    if (mode == "publisher") {
        market_data::MarketDataGenerator generator;
        network::MulticastServer multicast_server;

        multicast_server.run();

        while (true) {
            generator.generate();
            publish(generator.get_symbols(), multicast_server);
            // helper_print_tickers(generator.get_symbols());
            std::this_thread::sleep_for(std::chrono::milliseconds(config::tick_interval));
        }
    } else if (mode == "subscriber") {
        network::MulticastSubscriber subscriber;
        subscriber.subscribe(config::multicast_ip, config::multicast_port);
        while (true) {
            std::string data = subscriber.receive();
            std::cout << data << std::endl;
        }
        subscriber.receive();
    } else {
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}
