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

#include "IPC.hpp"

int main(int argc, char* argv[]) {
    
    std::string name;
    if (argc == 2) {
        name = argv[1];
    } else {
        name = "bili";
    }

    std::thread ipc_thread( []() { wait_for_script(); } );
    ipc_thread.detach();

    Network network(name);
    Server server;
    std::thread t1([&server]() { server.listen(); });
    t1.join();
    return 0;
}
