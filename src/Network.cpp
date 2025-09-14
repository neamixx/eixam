#include "Network.hpp"

#include <iostream>
#include <fstream>

void Network::_handle_connect(websocketpp::connection_hdl hdl)
{
    websocketpp::server<websocketpp::config::asio>::connection_ptr con = _ws.get_con_from_hdl(hdl);
    std::string client_ip = con->get_remote_endpoint();

    std::cout << "Connection opened from IP: " << client_ip << std::endl;

    _peers[client_ip] = hdl;
}

void Network::_handle_disconnect(websocketpp::connection_hdl hdl)
{
    websocketpp::server<websocketpp::config::asio>::connection_ptr con = _ws.get_con_from_hdl(hdl);
    std::string client_ip = con->get_remote_endpoint();

    std::cout << "Connection closed from IP: " << client_ip << std::endl;

    _peers.erase(client_ip);
}

Network::Network()
{
    _ws.init_asio();
    
    //weird lambda syntax to bind member functions as handlers
    _ws.set_open_handler([this](websocketpp::connection_hdl hdl) { this->_handle_connect(hdl); });
    _ws.set_close_handler([this](websocketpp::connection_hdl hdl) { this->_handle_disconnect(hdl); });
    
}

Network::~Network()
{
}

std::vector<char> Network::read_file(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Could not open file " + path);
    return std::vector<char>((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
}

void Network::listen()
{
    _ws.listen(PORT);
    _ws.start_accept();
}

void Network::send_file(const std::string& ip_dest, const std::string& file_path)
{
    if (_peers.find(ip_dest) == _peers.end()) {
        std::cerr << "No connection found for IP: " << ip_dest << std::endl;
        return;
    }

    // Implementation for sending the file over the network
    std::cout << "[C++] Sending file over network: " << file_path << "\n";
    std::vector<char> file_data = read_file(file_path);
    if (file_data.empty()) {
        std::cerr << "Failed to read file: " << file_path << std::endl;
        return;
    }

    websocketpp::connection_hdl hdl = _peers[ip_dest];
    websocketpp::server<websocketpp::config::asio>::connection_ptr con = _ws.get_con_from_hdl(hdl);
    if (!con) {
        std::cerr << "Invalid connection handle for IP: " << ip_dest << std::endl;
        return;
    }

    // Send the file data over the WebSocket connection
    _ws.send(hdl, file_data.data(), file_data.size(), websocketpp::frame::opcode::binary);
}
