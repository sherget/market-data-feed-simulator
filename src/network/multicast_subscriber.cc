#include "network/multicast_subscriber.h"

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <vector>

namespace network {

MulticastSubscriber::MulticastSubscriber() : sock_{-1} {};

MulticastSubscriber::~MulticastSubscriber() {
    if (sock_ >= 0) {
        close(sock_);
        sock_ = -1;
    }
};

void MulticastSubscriber::subscribe(const char* ip, int port) {
    this->sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->sock_ < 0) {
        return;
    }

    int reuse = 1;
    setsockopt(this->sock_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    this->addr_.sin_family = AF_INET;
    this->addr_.sin_port = htons(port);
    this->addr_.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(this->sock_, (sockaddr*)&this->addr_, (socklen_t)sizeof(this->addr_));

    ip_mreq mreq;
    inet_pton(AF_INET, ip, &mreq.imr_multiaddr);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(this->sock_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
};

// TODO: Implement filters
void MulticastSubscriber::add_filter(std::vector<std::string> ticker_names) {};

void MulticastSubscriber::remove_filter(std::vector<std::string> ticker_names) {};

std::string MulticastSubscriber::receive() {
    char buf[4096];
    fflush(stdout);
    // Apparently this is a unsafe function we have to remember to subrtact 1 from our buffer size
    // to prevent a bufferoverflow.
    ssize_t len = recv(this->sock_, buf, sizeof(buf) - 1, 0);
    if (len > 0) {
        buf[len] = '\0';
        return std::string(buf);
    }

    return "";
};

}  // namespace network
