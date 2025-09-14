
#include "Server.hpp"
#include "Resources.hpp"

extern Resources resources;

Server::Server() {
    svr.Get("/hi", [&](const httplib::Request&, httplib::Response& res) {
    nlohmann::json peers_json;
    for (const auto& [key, peer] : resources._peers) {
        peers_json[key] = {
            {"ttl", peer.ttl},
            {"last_msg", {
                {"cpu", peer.last_msg.cpu},
                {"totalcpu", peer.last_msg.totalcpu},
                {"totalram", peer.last_msg.totalram},
                {"gpu", peer.last_msg.gpu},
                {"ram", peer.last_msg.ram},
                {"temp", peer.last_msg.temp},
                {"hostname", peer.last_msg.hostname},
                {"group_id", peer.last_msg.group_id},
                {"port", peer.last_msg.port}
            }}
        };
    }

    nlohmann::json j = {{"msg", peers_json}};
    std::cout << j << std::endl;
    res.set_content(j.dump(), "application/json");
    std::cout << "Server initialized" << std::endl;
});
}

void Server::listen() {
    svr.listen("0.0.0.0", 8080);
}