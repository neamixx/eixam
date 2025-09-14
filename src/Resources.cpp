#include "Resources.hpp"
#include <statgrab.h>
#include <iostream>
#include <unistd.h> //for gethostname



Resources::Resources(/* args */)
{
    sg_error init_err = sg_init(0);
    if (init_err != SG_ERROR_NONE) {
        std::cerr << "sg_init failed, error: " << init_err << std::endl;
    }

    // Take a snapshot before querying stats
    sg_error snap_err = sg_snapshot();
    if (snap_err != SG_ERROR_NONE) {
        std::cerr << "sg_snapshot failed, error: " << snap_err << std::endl;
    }
}
Resources::~Resources()
{
    sg_shutdown();
}

int Resources::update()
{

    size_t entries = 0;
    sg_mem_stats* mem = sg_get_mem_stats(&entries);
    if (mem && entries > 0) {
        total_mem = mem->total / (1024 * 1024);    // MB
        used_mem  = mem->used  / (1024 * 1024);
        free_mem  = mem->free  / (1024 * 1024);
        std::cout << "Total: " << total_mem << " MB\n";
        std::cout << "Used : " << used_mem  << " MB\n";
        std::cout << "Free : " << free_mem  << " MB\n";
    } else {
        std::cerr << "Failed to get memory stats.\n";
    }


    // CPU stats
    size_t cpu_error = 0;
    sg_cpu_percents *cpu = sg_get_cpu_percents(&cpu_error);
    if (cpu && cpu_error == SG_ERROR_NONE) {
        std::cout << "CPU user   : " << cpu->user   << "%\n";
        std::cout << "CPU system : " << cpu->kernel << "%\n";
        std::cout << "CPU idle   : " << cpu->idle   << "%\n";
        cpu_user   = cpu->user;
        cpu_system = cpu->kernel;
        cpu_idle   = cpu->idle;
    } else {
        std::cerr << "Failed to get CPU stats, error code: " << cpu_error << std::endl;
    }

    // Host name
    hostname = std::string("node");
    /*
    char hostname_c[50];
    if (gethostname(hostname_c, sizeof(hostname_c)) == 0) {
        hostname = std::string(hostname_c);
    } else {
        perror("gethostname");
    }*/
    return 0;
}