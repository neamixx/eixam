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

class Network
{
private:

    std::unordered_map<std::string, websocketpp::connection_hdl> _peers;

    websocketpp::server<websocketpp::config::asio> _ws;

    void _handle_connect(websocketpp::connection_hdl hdl);
    void _handle_disconnect(websocketpp::connection_hdl hdl);


public:
    Network(/* args */);
    ~Network();

    std::vector<char> read_file(const std::string& path);
    void send_file(const std::string& ip_dest, const std::string& endpoint, const std::string& file_path);

    void listen();
    void stop();
};

#endif