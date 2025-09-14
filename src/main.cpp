#include <lua.hpp>
#include <iostream>
#include <string>
#include <vector>
//#include "Peer-server.hpp"
#include "Script.hpp"
#include "Network.hpp"
#include <nlohmann/json.hpp>
#include <httplib.h>

void server() {
    httplib::Server svr;
    std::cout <<"hola"<< std::endl;
        svr.Get("/hi", [](const httplib::Request&, httplib::Response& res) {
        nlohmann::json j = {{"msg", "Hello World!"}};
        res.set_content(j.dump(), "application/json");   // IMPORTANT: content-type
    });

    svr.listen("0.0.0.0", 8080);
}

int main() {
    //boost::asio::io_context io;
    //PeerServer server(io, 12345);
    //io.run();

    //Script script("job.lua");
    //script.run();
    server();
    
    //std::thread t(server);

    Network net;
    return 0;
}
