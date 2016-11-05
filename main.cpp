﻿/*
** repository: https://github.com/trumanzhao/luna
** trumanzhao, 2016-11-01, trumanzhao@foxmail.com
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <locale>
#include <cstdint>
#include <signal.h>
#include "luna.h"
#include "tools.h"
#include "socket_io.h"
#include "socket_wapper.h"

static const char* g_usage =
u8R"---(usage: luna main.lua)---";

lua_State* g_lvm = nullptr;

#if defined(__linux) || defined(__APPLE__)
static void on_quit_signal(int signo)
{
    lua_call_global_function(g_lvm, "on_quit_signal", std::tie(), signo);
}
#endif

int main(int argc, const char* argv[])
{
#if defined(__linux) || defined(__APPLE__)
    tzset();
#endif

#ifdef _MSC_VER
	_tzset();
#endif

    setlocale(LC_ALL, "");

#if defined(__linux) || defined(__APPLE__)
    signal(SIGINT, on_quit_signal);
    signal(SIGTERM, on_quit_signal);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
#endif

    if (argc != 2)
    {
        puts(g_usage);
        return 1;
    }

	g_lvm = luaL_newstate();

	luaL_openlibs(g_lvm);

    lua_register_function(g_lvm, "get_file_time", get_file_time);
    lua_register_function(g_lvm, "get_time_ms", get_time_ms);
    lua_register_function(g_lvm, "sleep_ms", sleep_ms);
	lua_register_function(g_lvm, "create_socket_mgr", lua_create_socket_mgr);

	luaL_dofile(g_lvm, "base/base.lua");

    lua_call_global_function(g_lvm, "luna_entry", std::tie(), argv[1]);

	lua_close(g_lvm);
	return 0;
}


