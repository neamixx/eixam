#ifndef SERVER_HPP
#define SERVER_HPP

#include <httplib.h>
#include <nlohmann/json.hpp>

class Server {
public:
    Server();
    void listen();
private:
    httplib::Server svr;
};

#endif // SERVER_HPP