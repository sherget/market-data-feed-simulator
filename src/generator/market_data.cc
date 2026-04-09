#include "generator/market_data.h"

#include <chrono>
#include <random>
#include <string>
#include <vector>

#include "config.h"

namespace market_data {

MarketDataGenerator::MarketDataGenerator() : mt_(rd_()), dist_(-1, 1) {}

void MarketDataGenerator::set_multicast_server(std::shared_ptr<network::MulticastServer> server) {
    multicast_server_ = std::move(server);
}

void MarketDataGenerator::add_symbol(std::string name, int price_in_cents) {
    if (threads_.find(name) != threads_.end()) {
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto timestamp_ms = static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());

    Tick ticker{name, static_cast<int>(price_in_cents), timestamp_ms};
    tickers_[name] = ticker;

    threads_.try_emplace(name,
                         [this, name, price_in_cents]() { tick_worker(name, price_in_cents); });
    threads_[name].start();
}

void MarketDataGenerator::tick_worker(std::string name, int initial_price) {
    int price = initial_price;

    while (threads_[name].is_running()) {
        random_walk(price);
        auto now = std::chrono::system_clock::now();
        auto timestamp_ms = static_cast<uint64_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());

        Tick tick{name, price, timestamp_ms};
        tickers_[name] = tick;

        if (multicast_server_) {
            std::string message = "{\"name\":" + tick.name +
                                  ",\"price\":" + std::to_string(tick.price_in_cents) +
                                  ",\"timestamp\":" + std::to_string(tick.timestamp) + "}\n";
            multicast_server_->broadcast(message);
        }

        std::this_thread::sleep_for(std::chrono::microseconds(config::tick_interval));
    }
}

void MarketDataGenerator::random_walk(int& price) {
    int step = dist_(mt_);
    price += step;
    if (price < 0) {
        price = 0;
    }
}

int MarketDataGenerator::remove_symbol(std::string name) {
    auto it = threads_.find(name);
    if (it == threads_.end()) {
        return -1;
    }

    it->second.stop();
    it->second.join();
    threads_.erase(it);
    tickers_.erase(name);

    return 0;
}

std::vector<Tick> MarketDataGenerator::get_symbols() {
    std::vector<Tick> result;
    result.reserve(tickers_.size());
    for (const auto& [name, tick] : tickers_) {
        result.push_back(tick);
    }
    return result;
}

void MarketDataGenerator::stop_all() {
    for (auto& [name, thread] : threads_) {
        thread.stop();
    }
    for (auto& [name, thread] : threads_) {
        thread.join();
    }
    threads_.clear();
    tickers_.clear();
}

}  // namespace market_data
