#include "Network.hpp"

#include <iostream>
#include <fstream>
#include <thread>

#include "Resources.hpp"

#include <boost/asio.hpp>
#include <iostream>
#include <fstream>

using boost::asio::ip::tcp;

extern Resources resources;

Network::Network(std::string name)
{
    std::cout << "Network initialized, listening on port " << PORT << std::endl;
    //std::thread listen_thread([this]() { this->listen(); });
    std::thread t1([this]() { this->listen_heartbeat(); });
    std::thread t2([this]() { this->heartbeat(); });
    
    //listen_thread.detach();
    t1.detach();
    t2.detach();

    this->name = name;
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
    const char* port = "4044";
    const char* out_filename = "received_file.lua";

    try {
        boost::asio::io_context io;

        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), std::stoi(port)));
        tcp::socket socket(io);

        std::cout << "Waiting for connection on port " << port << "...\n";
        acceptor.accept(socket);

        std::ofstream outfile(out_filename, std::ios::binary);
        if (!outfile) {
            std::cerr << "Could not open output file.\n";
            throw std::runtime_error("File open error");
        }

        char buffer[4096];
        boost::system::error_code ec;
        std::size_t n;

        while ((n = socket.read_some(boost::asio::buffer(buffer), ec)) > 0) {
            outfile.write(buffer, n);
        }

        if (ec != boost::asio::error::eof) {
            throw boost::system::system_error(ec);
        }

        std::cout << "File received and saved as " << out_filename << "\n";
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void Network::send_file(const std::string& ip_dest, const std::string& endpoint, const std::string& file_path)
{

    std::vector<char> file_data = read_file(file_path);
    if (file_data.empty()) {
        std::cerr << "Failed to read file: " << file_path << std::endl;
        return;
    }

    try {
        boost::asio::io_context io;

        // Resolve host/port
        tcp::resolver resolver(io);
        auto endpoints = resolver.resolve(ip_dest.c_str(), "4044");

        // Create socket
        tcp::socket socket(io);

        // Connect
        boost::asio::connect(socket, endpoints);

        // Open file
        std::ifstream file(file_path, std::ios::binary);
        if (!file) {
            std::cerr << "Could not open file: " << file_path << "\n";
            throw std::runtime_error("File open error");
        }

        // Send file in chunks
        char buffer[4096];
        while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
            std::size_t n = file.gcount(); // bytes read
            boost::asio::write(socket, boost::asio::buffer(buffer, n));
        }

        std::cout << "File sent successfully.\n";
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

// Communication management functions
void Network::heartbeat(){

    while (true)
    {
        resources.update();
        //std::cout<<"heartbeat thread started"<<std::endl;
        //std::cout<<"cpu: "<<resources.cpu_user<<std::endl;
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

        Message msg;
        msg.cpu = resources.cpu_user;
        msg.totalcpu = resources.cpu_system;
        msg.gpu = 0;
        msg.ram = resources.used_mem;
        msg.totalram = resources.total_mem;
        msg.group_id = 0;
        strncpy(msg.hostname, name.c_str(), sizeof(msg.hostname) - 1);
        msg.hostname[sizeof(msg.hostname) - 1] = '\0';
        msg.port = 5005;

        udp::endpoint multicast_endpoint(boost::asio::ip::make_address(multicast_address), multicast_port);

        // Send message
        socket.send_to(boost::asio::buffer(&msg, sizeof(msg)), multicast_endpoint);

        // Optional: print for debugging
        //std::cout << "Sent heartbeat to multicast group " 
          //        << multicast_address << ":" << multicast_port << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Heartbeat send failed: " << e.what() << std::endl;
    }
}

void Network::check_alive(){
    const int TIMEOUT_MS = 5000; // 5 sec
    auto now = std::chrono::high_resolution_clock::now();
    int current_time = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    for (auto it = resources._peers.begin(); it != resources._peers.end(); ) {
        if (current_time - it->second.ttl > TIMEOUT_MS) {
            std::cout << "Peer " << it->first << " is no longer alive." << std::endl;
            it = resources._peers.erase(it);
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
    Message msg = {};
    
    while(true) {
        udp::endpoint sender_endpoint;
        size_t length = socket.receive_from(boost::asio::buffer(&msg, sizeof(msg)), sender_endpoint);
        std::cout << "Received heartbeat from " << msg.hostname << std::endl;
        

        //InfoPeer info;
        //  info.last_msg = msg;
        //std::cout << msg.cpu << " " << msg.gpu << " " << msg.ram << " " << msg.group_id << " " << msg.port << std::endl;
        auto now = std::chrono::high_resolution_clock::now();
        int timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        add_peer(sender_endpoint.address().to_string(), timestamp, msg);
        //std::cout<<"epp"<<std::endl;
        
    }
}


void Network::add_peer(const std::string& ip, int timestamp, struct Message& msg){
    if(resources._peers.find(ip) == resources._peers.end()){
        InfoPeer info;
        info.ttl = timestamp;
        info.last_msg = msg;
        resources._peers[ip] = info;
        std::cout << "Added new peer: " << ip << std::endl;
    } else {
        resources._peers[ip].ttl = timestamp; //update ttl
        resources._peers[ip].last_msg = msg; //update last message
        //std::cout << "Updated peer: " << ip << std::endl;
    }

}