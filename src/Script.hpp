#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <lua.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <boost/asio.hpp>

class Script
{
private:
    lua_State* LuaState;
    std::string script_path;
    
    int _execute_bash(const std::string& cmd);

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
public:
    Script(const std::string& path);
    ~Script();

    int compose();
    int action();

};

#endif