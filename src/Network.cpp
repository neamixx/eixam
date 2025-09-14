#include "Network.hpp"

#include <iostream>
#include <fstream>
#include <thread>

#include "Resources.hpp"

extern Resources resources;

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
    std::cout << "Network initialized, listening on port " << PORT << std::endl;
    _ws.init_asio();
    //weird lambda syntax to bind member functions as handlers
    _ws.set_open_handler([this](websocketpp::connection_hdl hdl) { this->_handle_connect(hdl); });
    _ws.set_close_handler([this](websocketpp::connection_hdl hdl) { this->_handle_disconnect(hdl); });
    std::thread t1([this]() { this->listen_heartbeat(); });
    std::thread t2([this]() { this->heartbeat(); });
    t1.detach();
    t2.detach();

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

// Communication management functions
void Network::heartbeat(){

    while (true)
    {
        resources.update();
        std::cout<<"heartbeat thread started"<<std::endl;
        std::cout<<"cpu: "<<resources.cpu_user<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        check_alive();
        send_heartbeat();
    }
}


void Network::send_heartbeat() {
    using boost::asio::ip::udp;
    try {
        boost::asio::io_context io_context;
        udp::socket socket(io_context);
        socket.open(udp::v4());

        // Set TTL for multicast (optional)
        socket.set_option(boost::asio::ip::multicast::hops(1));

        // Multicast address (all peers should listen on this)
        std::string multicast_address = "239.255.0.1"; // example multicast IP
        unsigned short multicast_port = 5005;

        struct Message msg;
        msg.cpu = resources.cpu_user;
        msg.gpu = 0;
        msg.ram = resources.used_mem;
        msg.group_id = 0;
        msg.port = 5005;

        udp::endpoint multicast_endpoint(boost::asio::ip::make_address(multicast_address), multicast_port);

        // Send message
        socket.send_to(boost::asio::buffer(&msg, sizeof(msg)), multicast_endpoint);

        // Optional: print for debugging
        std::cout << "Sent heartbeat to multicast group " 
                  << multicast_address << ":" << multicast_port << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Heartbeat send failed: " << e.what() << std::endl;
    }
}

void Network::check_alive(){
    std::cout<<"ajudaaa"<<std::endl;
    const int TIMEOUT_MS = 5000; // 5 sec
    auto now = std::chrono::high_resolution_clock::now();
    int current_time = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    for (auto it = _peers.begin(); it != _peers.end(); ) {
        if (current_time - it->second.ttl > TIMEOUT_MS) {
            std::cout << "Peer " << it->first << " is no longer alive." << std::endl;
            it = _peers.erase(it);
        } else {
            ++it;
        }
    }
}

void Network::listen_heartbeat(){
    using boost::asio::ip::udp;
    boost::asio::io_context io_context;
    udp::endpoint listener_endpoint(udp::v4(), 5005);
    udp::socket socket(io_context);
    socket.open(listener_endpoint.protocol());
    socket.set_option(boost::asio::socket_base::reuse_address(true));
    socket.bind(listener_endpoint);
    socket.set_option(boost::asio::ip::multicast::join_group(
    boost::asio::ip::make_address("239.255.0.1").to_v4()));
    Message msg;
    
    while(true) {
        udp::endpoint sender_endpoint;
        size_t length = socket.receive_from(boost::asio::buffer(&msg, sizeof(msg)), sender_endpoint);
        struct InfoPeer info;
        info.last_msg = msg;
        std::cout << msg.cpu << " " << msg.gpu << " " << msg.ram << " " << msg.group_id << " " << msg.port << std::endl;
        auto now = std::chrono::high_resolution_clock::now();
        int timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        add_peer(sender_endpoint.address().to_string(), timestamp, msg);
        std::cout<<"epp"<<std::endl;
        
    }
}


void Network::add_peer(const std::string& ip, int timestamp, struct Message msg){
    if(_peers.find(ip) == _peers.end()){
        InfoPeer info;
        info.ttl = timestamp;
        info.last_msg = msg;
        _peers[ip] = info;
        std::cout << "!!!Added new peer!!!: " << ip << std::endl;
    } else {
        _peers[ip].ttl = timestamp; //update ttl
        _peers[ip].last_msg = msg; //update last message
        std::cout << "Updated peer: " << ip << std::endl;
    }

}