#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>
#include <unordered_map>

struct Message{
    float cpu;
    float totalcpu;
    float totalram;
    float gpu;
    float ram;
    char hostname[20];
    int group_id;
    int port;
};

struct InfoPeer{
    int ttl; //time to live
    struct Message last_msg;
};



class Resources
{
private:

public:
    std::unordered_map<std::string, InfoPeer> _peers; //ip, InfoPeer

    int total_mem;
    int used_mem;
    int free_mem;

    float cpu_user;
    float cpu_system;
    float cpu_idle;

    std::string hostname;
    
    Resources(/* args */);
    ~Resources();

    int update();

    int total_memory();
    int used_memory();
    int free_memory();

};
#endif // RESOURCES_HPP