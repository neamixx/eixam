#include <lua.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Script.hpp"

int main() {
    Script script("job.lua");
    script.run();
    return 0;
}
