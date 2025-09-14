#include "Network.hpp"

#include <iostream>
#include <fstream>

void Network::_handle_connect(websocketpp::connection_hdl hdl)
{
    websocketpp::server<websocketpp::config::asio>::connection_ptr con = _ws.get_con_from_hdl(hdl);
    std::string client_ip = con->get_remote_endpoint();

    std::cout << "Connection opened from IP: " << client_ip << std::endl;

    std::string resource = con->get_resource();

    std::cout << "Incoming request for resource: " << resource << std::endl;

}

void Network::_handle_disconnect(websocketpp::connection_hdl hdl)
{
    websocketpp::server<websocketpp::config::asio>::connection_ptr con = _ws.get_con_from_hdl(hdl);
    std::string client_ip = con->get_remote_endpoint();

    std::cout << "Connection closed from IP: " << client_ip << std::endl;
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

    _ws.run();
}

void Network::send_file(const std::string& ip_dest, const std::string& endpoint, const std::string& file_path)
{

    std::vector<char> file_data = read_file(file_path);
    if (file_data.empty()) {
        std::cerr << "Failed to read file: " << file_path << std::endl;
        return;
    }

    try
    {
        websocketpp::client<websocketpp::config::asio_client> c;
        c.init_asio();

        //log error channels
        c.clear_access_channels(websocketpp::log::alevel::all);
        c.clear_error_channels(websocketpp::log::elevel::all);

        websocketpp::lib::error_code ec;
        std::string uri = "ws://" + ip_dest + ":4044/" + endpoint;

        auto con = c.get_connection(uri, ec);
        if (ec) {
            std::cerr << "Connection error: " << ec.message() << "\n";
            throw std::runtime_error("Failed to create WebSocket connection");
        }

        c.connect(con);

        std::thread asio_thread([&c]() { c.run(); });

        while (con->get_state() != websocketpp::session::state::open) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        con->send(file_data.data(), file_data.size(), websocketpp::frame::opcode::binary);std::cout << "File sent: " << file_data.size() << " bytes\n";

        // Close connection
        con->close(websocketpp::close::status::normal, "done");

        std::cout << "File sent: " << file_data.size() << " bytes\n";
        
        asio_thread.join();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}
