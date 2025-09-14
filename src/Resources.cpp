#include "Resources.hpp"
#include <statgrab.h>
#include "Resources.hpp"

Resources::Resources(/* args */)
{
}
Resources::~Resources()
{
}
int Resources::total_memory(){
    
}
int Resources::memory(){
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
}