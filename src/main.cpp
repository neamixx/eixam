#include <lua.hpp>
#include <iostream>
#include <string>
#include <vector>
//#include "Peer-server.hpp"
#include "Script.hpp"
#include "Network.hpp"

#include <statgrab.h>

int main() {

        sg_error init_err = sg_init(0);
    if (init_err != SG_ERROR_NONE) {
        std::cerr << "sg_init failed, error: " << init_err << std::endl;
        return 1;
    }

    // Take a snapshot before querying stats
    sg_error snap_err = sg_snapshot();
    if (snap_err != SG_ERROR_NONE) {
        std::cerr << "sg_snapshot failed, error: " << snap_err << std::endl;
        return 1;
    }

    // Memory stats
    size_t mem_error = 0;
    sg_mem_stats *mem = sg_get_mem_stats(&mem_error);
    if (mem && mem_error == SG_ERROR_NONE) {
        std::cout << "Total memory: " << mem->total / 1024 << " KB\n";
        std::cout << "Used memory : " << mem->used / 1024 << " KB\n";
        std::cout << "Free memory : " << mem->free / 1024 << " KB\n";
    } else {
        std::cerr << "Failed to get memory stats, error code: " << mem_error << std::endl;
    }

    // CPU stats
    size_t cpu_error = 0;
    sg_cpu_percents *cpu = sg_get_cpu_percents(&cpu_error);
    if (cpu && cpu_error == SG_ERROR_NONE) {
        std::cout << "CPU user   : " << cpu->user   << "%\n";
        std::cout << "CPU system : " << cpu->kernel << "%\n";
        std::cout << "CPU idle   : " << cpu->idle   << "%\n";
    } else {
        std::cerr << "Failed to get CPU stats, error code: " << cpu_error << std::endl;
    }

    sg_shutdown();

    Network net;
    return 0;
}
