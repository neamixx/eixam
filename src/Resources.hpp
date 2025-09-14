#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>

class Resources
{
private:

public:
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