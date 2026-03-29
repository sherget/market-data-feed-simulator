#include "network/multicast.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

#include "config.h"

namespace network {
MulticastServer::MulticastServer() : sock_{-1} {};

MulticastServer::~MulticastServer() {
    if (sock_ >= 0) {
        close(sock_);
        sock_ = -1;
    }
};

void MulticastServer::run() {
    this->sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->sock_ < 0) {
        return;
    }

    this->addr_.sin_family = AF_INET;
    this->addr_.sin_port = htons(config::multicast_port);
    inet_pton(AF_INET, config::multicast_ip, &this->addr_.sin_addr);

    unsigned char ttl = 1;
    setsockopt(this->sock_, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));

    this->running_ = true;
}

void MulticastServer::stop() { this->running_ = false; }

void MulticastServer::broadcast(const std::string& input) {
    sendto(this->sock_, input.c_str(), input.size(), 0, (sockaddr*)&this->addr_,
           sizeof(this->addr_));
}
}  // namespace network
