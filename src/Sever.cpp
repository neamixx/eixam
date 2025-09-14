#include "Server.hpp"

Server::Server() {
    // Constructor implementation (if needed)
    svr.Get("/hi", [](const httplib::Request&, httplib::Response& res) {
        nlohmann::json j = {{"msg", "Hello World!"}};
        res.set_content(j.dump(), "application/json");   // IMPORTANT: content-type
    });
}

void Server::listen() {
    svr.listen("0.0.0.0", 8080);
}