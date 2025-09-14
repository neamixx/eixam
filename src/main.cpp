#include <lua.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Network.hpp"

int main() {
    Network network;
    network.listen();

    return 0;
}
