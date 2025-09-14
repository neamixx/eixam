#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <string>

struct Peer {
    int id;
    int ttl;
    std::string ip;
    unsigned short port;
};

struct Message {
    std::string content;
    std::string sender_ip;
    unsigned short sender_port;
};

int main() {
    using boost::asio::ip::udp;
    boost::asio::io_context io_context;

    udp::socket socket(io_context, udp::endpoint(udp::v4(), 5005));
    std::vector<Peer> peers;
    int next_id = 1;

    while (true) {
        char data[1024];
        udp::endpoint sender_endpoint;
        size_t length = socket.receive_from(boost::asio::buffer(data), sender_endpoint);

        std::string message(data, length);
        std::cout << "Received message: " << message
                  << " from " << sender_endpoint.address().to_string()
                  << ":" << sender_endpoint.port() << std::endl;

        bool found = false;
        for (const auto& peer : peers) {
            if (peer.ip == sender_endpoint.address().to_string() && peer.port == sender_endpoint.port()) {
                found = true;
                break;
            }
        }
        if (!found) {
            peers.push_back({next_id++, 60, sender_endpoint.address().to_string(), sender_endpoint.port()});
            std::cout << "Added new peer: " << sender_endpoint.address().to_string()
                      << ":" << sender_endpoint.port() << std::endl;
        }
    }
    return 0;
}