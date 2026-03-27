#ifndef CONFIG_H
#define CONFIG_H

#include <vector>

#include "generator/market_data.h"

namespace config {
inline std::vector<market_data::Tick> tickers = {
    {"META", 59292, 0}, {"AAPL", 25164, 0}, {"AMZN", 20724, 0},
    {"GOOG", 28920, 0}, {"NFLX", 9092, 0},
};
inline int tick_interval = 100;
inline const char* multicast_ip = "239.255.0.1";
inline const int multicast_port = 50000;
}  // namespace config

#endif  // !CONFIG.H
