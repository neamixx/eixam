
#include "Server.hpp"
#include "Resources.hpp"

extern Resources resources;

Server::Server() {
    svr.Get("/allsys", [&](const httplib::Request&, httplib::Response& res) {
    nlohmann::json peers_json;
    for (const auto& [key, peer] : resources._peers) {
        peers_json[key] = {
            {"message", {
                {"cpu", peer.last_msg.cpu},
                {"ram", peer.last_msg.ram/peer.last_msg.totalram*100},
                {"gpu", "GPU unavailable"},
                {"temp", 53},
                {"hostname", peer.last_msg.hostname},
            }}
        };
    }

    nlohmann::json j = {{"msg", peers_json}};
    std::cout << j << std::endl;
    res.set_content(j.dump(), "application/json");
    std::cout << "Server initialized" << std::endl;
});

    svr.Get("/sys", [&](const httplib::Request&, httplib::Response& res) {
    nlohmann::json peers_json;

    // Calculate averages and sums
    double cpu_sum = 0.0;
    double ram_sum = 0.0;
    double ram_total_sum = 0.0;
    int gpu_count = 0;
    int peer_count = 0;

    for (const auto& [key, peer] : resources._peers) {
        cpu_sum += peer.last_msg.cpu;
        ram_sum += peer.last_msg.ram;
        ram_total_sum += peer.last_msg.totalram;
        peer_count++;
       
    }

    double cpu_avg = peer_count > 0 ? cpu_sum / peer_count : 0.0;
    double ram_avg = peer_count > 0 ? (ram_sum / ram_total_sum) * 100 : 0.0;

    nlohmann::json j = {
            {"cpu_avg", cpu_avg},
            {"ram_usage_percent", ram_avg},
            {"gpu", "GPU unavailable"}
    };
    std::cout << j << std::endl;
    res.set_content(j.dump(), "application/json");
    std::cout << "Server initialized" << std::endl;
});};

void Server::listen() {
    svr.listen("0.0.0.0", 8080);
}