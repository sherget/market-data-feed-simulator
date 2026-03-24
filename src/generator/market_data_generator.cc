#include "generator/market_data.h"

namespace market_data {
std::string greet(const std::string &name) {
  return name.empty() ? "Hello, World!" : "Hello, " + name + "!";
}
} // namespace market_data
