#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <lua.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Script
{
private:
    lua_State* LuaState;
    std::string script_path;

    
public:
    Script(const std::string& path);
    ~Script();

    int compose();
    int action();
    int result();
};

#endif