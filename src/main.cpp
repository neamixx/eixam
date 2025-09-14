#include <lua.hpp>
#include <iostream>
#include <string>
#include <vector>
//#include "Peer-server.hpp"
#include "Script.hpp"
#include "Network.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "extern.hpp"

#include <statgrab.h>
#include "Server.hpp"

int main() {

    Network net;
    Server server;
    std::thread t1([&server]() { server.listen(); });
    t1.join();
    return 0;
}
