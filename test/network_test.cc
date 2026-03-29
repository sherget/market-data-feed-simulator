#include <gtest/gtest.h>

#include <string>

#include "config.h"
#include "network/multicast.h"
#include "network/multicast_subscriber.h"

class MulticastTest : public ::testing::Test {
   protected:
    network::MulticastServer server;
    network::MulticastSubscriber subscriber;
};

TEST_F(MulticastTest, SubscriberReceivesData) {
    subscriber.subscribe(config::multicast_ip, config::multicast_port);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    server.run();
    server.broadcast("{\"name\":\"FOOBA\",\"price\":1337,\"timestamp\": 0}");

    std::string data = subscriber.receive();
    EXPECT_NE(data.find("FOOBA"), std::string::npos);
}
