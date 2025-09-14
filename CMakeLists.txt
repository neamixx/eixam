cmake_minimum_required(VERSION 3.15)
project(MyLuaProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")

# Lua
find_package(Lua REQUIRED)

# Boost (Asio via Boost.System)
find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)


find_path(STATGRAB_INCLUDE_DIR statgrab.h)
find_library(STATGRAB_LIB statgrab)

# WebSocket++ com a llibreria d'headers
add_library(websocketpp INTERFACE)
target_include_directories(websocketpp INTERFACE
    "${THIRD_PARTY_DIR}/websocketpp"
)

add_subdirectory("${THIRD_PARTY_DIR}/json" json_build)

set(HTTPLIB_USE_ZSTD_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
add_subdirectory("${THIRD_PARTY_DIR}/cpp-httplib" httplib_build)

# Fonts
file(GLOB SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Executable
add_executable(MyLuaProject ${SRC})

# Includes del projecte i Lua
target_include_directories(MyLuaProject PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    ${LUA_INCLUDE_DIR}
    ${STATGRAB_INCLUDE_DIR}
)

# Enllaços (NO ASIO standalone, NO defines d'ASIO_STANDALONE)
target_link_libraries(MyLuaProject PRIVATE
    ${LUA_LIBRARIES}
    ${STATGRAB_LIB}
    Boost::system
    Threads::Threads
    websocketpp
    httplib::httplib
    nlohmann_json::nlohmann_json
)
