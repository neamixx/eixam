cmake_minimum_required(VERSION 3.15)
project(MyLuaProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")

# Find Lua (make sure it is installed)
find_package(Lua REQUIRED)

# Add WebSocket++ (header-only)
add_subdirectory(${THIRD_PARTY_DIR}/websocketpp)

# Add ASIO as a proper target
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${THIRD_PARTY_DIR}/asio/asio/include)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)  # standalone mode

# Collect source files
file(GLOB SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Include directories
set(INCLUDE_DIR 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
    "${THIRD_PARTY_DIR}/websocketpp/"
)

# Add executable
add_executable(MyLuaProject
    ${SOURCE_DIR}
    src/main.cpp
)

# Include directories
target_include_directories(MyLuaProject PRIVATE 
    ${LUA_INCLUDE_DIR} 
    ${INCLUDE_DIR}
)

# Link libraries
target_link_libraries(MyLuaProject PRIVATE 
    ${LUA_LIBRARIES}   # Lua
    websocketpp        # WebSocket++
    asio               # ASIO (INTERFACE target)
    pthread            
)
