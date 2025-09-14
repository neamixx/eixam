#include <string>
#include <vector>
#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <unordered_map>

#ifndef NETWORK_HPP
#define NETWORK_HPP

#define PORT 8044

struct Message{
    float cpu;
    float gpu;
    float ram;
    int group_id;
    int port;
};

class Network
{
private:

    std::unordered_map<std::string, websocketpp::connection_hdl> _peersWS;
    
    websocketpp::server<websocketpp::config::asio> _ws;

    void _handle_connect(websocketpp::connection_hdl hdl);
    void _handle_disconnect(websocketpp::connection_hdl hdl);

    std::unordered_map<std::string, int> _peers; //ip, ttl


public:
    Network(/* args */);
    ~Network();

    std::vector<char> read_file(const std::string& path);
<<<<<<< Updated upstream
    void send_file(const std::string& ip_dest, const std::string& endpoint, const std::string& file_path);

=======
    void send_file(const std::string& ip_dest, const std::string& file_path);
>>>>>>> Stashed changes
    void listen();
    void stop();

    // Peer management and message handling
    void heartbeat();
    void check_alive();
    void add_peer(const std::string& ip, int timestamp);
    void delete_peer(const std::string& ip);
    void listen_heartbeat();
    void send_heartbeat();
};

#endif // NETWORK_HPP