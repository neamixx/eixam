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
    
    std::string script_path;
    
    int _execute_bash(const std::string& cmd);

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
public:
    Script(const std::string& path);
    ~Script();

    static int compose(std::string str);
    int action();

};

#endif