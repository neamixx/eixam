#include <iostream>
#include <random>


#include "Resources.hpp"
extern Resources resources;

std::string pick_machine() {

    std::unordered_map<std::string, InfoPeer> peers = resources._peers;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, peers.size() - 1);

    // Pick a random index
    size_t index = dist(gen);

    // Walk to that index
    auto it = peers.begin();
    std::advance(it, index);

    if (it == peers.end()) {
        throw std::runtime_error("No peers available");
    }

    if (resources.hostname.empty()) {
        throw std::runtime_error("Hostname is empty");
    }

    if (it->second.last_msg.hostname == resources.hostname) {

        return pick_machine();
    }
    return it->first; // Return the IP address

}
