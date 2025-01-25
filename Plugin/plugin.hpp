#pragma once

#ifdef DBG

#include <print>
#define dbg_println(...) std::println(__VA_ARGS__)

#else

#define dbg_println(...)

#endif

#include <windows.h>
#include <iostream>

#include <MinHookWrapper.hpp>
#include <RakHook/rakhook.hpp>

#include <samp.h>
#include "patches.hpp"

#ifndef VERSION
#define VERSION rakhook::detail::to_underlying(rakhook::samp_version())
#endif

class c_plugin
{
public:
	c_plugin(HMODULE hmodule);
	~c_plugin();

	static void game_loop();
	static HANDLE WINAPI create_file_w(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
	static void __fastcall dialog_close(c_dialog*, void*, uint8_t);

	static c_hook<void(*)()> game_loop_hook;
	static c_hook<decltype(&CreateFileW)> create_file_w_hook;
	static c_hook<void(__fastcall*)(c_dialog*, void*, uint8_t)> dialog_close_hook;

	static void create_textdraws();
	static void remove_textdraws();
	static void return_normal_radar_icons_size();
	static void show_spawn_change_dialog();
	static void send_spawn_change_response(uint8_t);
private:
	HMODULE hmodule;

	static std::vector<int> last_spawn_data;
};
inline c_hook<void(*)()> c_plugin::game_loop_hook = { 0x561B10 };
inline c_hook<decltype(&CreateFileW)> c_plugin::create_file_w_hook = {};
inline c_hook<void(__fastcall*)(c_dialog*, void*, uint8_t arg0)> c_plugin::dialog_close_hook = {};
inline std::vector<int> c_plugin::last_spawn_data = {};