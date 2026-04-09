#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "config.h"
#include "generator/market_data.h"
#include "network/multicast.h"
#include "network/multicast_subscriber.h"

void print_usage(const char* program) {
    std::cout << "Usage: " << program << " <mode> [options]\n";
    std::cout << "Modes:\n";
    std::cout << " publisher - Start market data generator and broadcaster\n";
    std::cout << " subscriber - Subscribe to multicast feed\n";
    std::cout << "Options:\n";
    std::cout << " --ticks N - Limit number of ticks (publisher mode)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    int max_ticks = 0;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--ticks" && i + 1 < argc) {
            max_ticks = std::stoll(argv[i + 1]);
        }
    }

    std::string mode = argv[1];

    if (mode == "publisher") {
        auto multicast_server = std::make_shared<network::MulticastServer>();
        multicast_server->run();

        market_data::MarketDataGenerator generator;
        generator.set_multicast_server(multicast_server);

        generator.add_symbol("META", 59292);
        generator.add_symbol("AAPL", 25164);
        generator.add_symbol("AMZN", 20724);
        generator.add_symbol("GOOG", 28920);
        generator.add_symbol("NFLX", 9092);

        if (max_ticks > 0) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(max_ticks * config::tick_interval / 1000));
            generator.stop_all();
        } else {
            std::this_thread::sleep_for(std::chrono::hours(24));
        }

    } else if (mode == "subscriber") {
        network::MulticastSubscriber subscriber;
        subscriber.subscribe(config::multicast_ip, config::multicast_port);
        while (true) {
            std::string data = subscriber.receive();
            std::cout << data << std::endl;
        }
    } else {
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}
