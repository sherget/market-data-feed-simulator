#ifndef UDP_MULTICAST_H
#define UDP_MULTICAST_H

#include <netinet/in.h>

#include <string>

namespace multicast {
class MulticastServer {
   public:
    MulticastServer();
    ~MulticastServer();
    void run();
    void stop();
    void broadcast(const std::string& input);

   private:
    int sock_{-1};
    sockaddr_in addr_{};
    bool running_{false};
};
}  // namespace multicast

#endif  // !UDP_MULTICAST_H
