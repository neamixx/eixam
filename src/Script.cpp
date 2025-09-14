#include "Script.hpp"

#define SCRIPT_PATH "../script/"


// Native function
int l_bash(lua_State* L) {
    // Expect 1 string argument
    if (!lua_isstring(L, 1)) {
        lua_pushstring(L, "bash expects a string");
        lua_error(L);
        return 0; // won't be reached (lua_error throws)
    }

    std::string cmd = lua_tostring(L, 1);

    std::cout << "[C++] Added command: " << cmd << "\n";

    return 0; // no values returned to Lua
}

int l_send_file(lua_State* L) {
    // Expect 1 string argument
    if (!lua_isstring(L, 1)) {
        lua_pushstring(L, "send_file expects a string");
        lua_error(L);
        return 0; // won't be reached (lua_error throws)
    }

    std::string filename = lua_tostring(L, 1);

    std::cout << "[C++] Sending file: " << filename << "\n";

    return 0; // no values returned to Lua
}



Script::Script(const std::string& path)
{
    script_path = path;
    

}

Script::~Script()
{
}

int Script::compose()
{

    LuaState = luaL_newstate();
    luaL_openlibs(LuaState);
    
    lua_newtable(LuaState);

    lua_pushcfunction(LuaState, l_bash);
    lua_setfield(LuaState, -2, "bash");

    lua_setglobal(LuaState, "job");

    std::string full_path = SCRIPT_PATH + script_path;
    if (luaL_dofile(LuaState, full_path.c_str()) != LUA_OK) {
        std::cerr << "Error loading script: " << lua_tostring(LuaState, -1) << "\n";
        lua_pop(LuaState, 1);
        lua_close(LuaState);
        return 1;
    }

    lua_getglobal(LuaState, "composing");
    if (lua_pcall(LuaState, 0, 0, 0) != LUA_OK) {
        std::cerr << "Error running function: " << lua_tostring(LuaState, -1) << "\n";
        lua_pop(LuaState, 1);
    }

    lua_close(LuaState);
    return 0;
}

int Script::action()
{
    return 0;
}

int Script::result()
{
    return 0;
}
