#ifndef MULTICAST_SUBSCRIBER_H
#define MULTICAST_SUBSCRIBER_H

#include <netinet/in.h>

#include <string>
#include <vector>

namespace network {
class MulticastSubscriber {
   public:
    MulticastSubscriber();
    ~MulticastSubscriber();
    void subscribe(const char* ip, int port);
    void add_filter(std::vector<std::string> ticker_names);
    void remove_filter(std::vector<std::string> ticker_names);
    std::string receive();

   private:
    int sock_{-1};
    sockaddr_in addr_{};
    std::vector<std::string> filter_{};
};
}  // namespace network

#endif  // !MULTICAST_SUBSCRIBER_H
