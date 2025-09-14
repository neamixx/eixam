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

void server() {
    httplib::Server svr;
    std::cout <<"hola"<< std::endl;
        svr.Get("/hi", [](const httplib::Request&, httplib::Response& res) {
        nlohmann::json j = {{"msg", "Hello World!"}};
        res.set_content(j.dump(), "application/json");   // IMPORTANT: content-type
    });

    svr.listen("0.0.0.0", 8080);
}

#include <statgrab.h>

int main() {


    std::thread t(server);
    Network net;
    std::thread t1(server);
    t1.join();
    return 0;
}
