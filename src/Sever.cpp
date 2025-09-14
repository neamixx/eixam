
#include "Server.hpp"
#include "Resources.hpp"

extern Resources resources;

Server::Server() {
    // Constructor implementation (if needed)
    svr.Get("/hi", [&](const httplib::Request&, httplib::Response& res) {
        nlohmann::json j = {{"msg", resources.used_mem}};
        std::cout << j << std::endl;
        res.set_content(j.dump(), "application/json");
        std::cout << "Server initialized" << std::endl;
    });
}

void Server::listen() {
    svr.listen("0.0.0.0", 8080);
}