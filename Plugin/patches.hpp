#pragma once
#include "config.hpp"

#include <iostream>
#include <vector>

class c_patches
{
public:
	c_patches();
	~c_patches();

	void enable_patches();
	std::vector<std::string> get_patches_names();
	static c_patches* get();

private:
	struct _patches {
		std::string name;
		std::string module;
		std::string pattern;
		std::string new_bytes;
	};

	std::string evolve_processing = c_settings::get()->evolve_processing;

	std::vector<_patches> patches = {
		{
			"no_evolve_render",
			evolve_processing,
			"55 8B EC 81 EC E0 00 00 00 53 56 57 8D 7D E0 B9 08 00 00 00 B8 CC CC CC CC F3 AB B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 0F B6 05",
			"C3"
		},
		{
			"no_esc_transparency",
			evolve_processing,
			"53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 53 81 EC C0 03 00 00 56 57 8D BD B0 FD FF FF B9 90 00 00 00 B8 CC CC CC CC F3 AB A1 ?? ?? ?? ?? 33 C5 89 45 EC 50 8D 45 F4 64 A3 00 00 00 00 B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8",
			"C3"
		},
		{
			"no_binds",
			evolve_processing,
			"74 0C E8 ?? ?? ?? ?? 0F B6 C0 85 C0 74 07 B0 01",
			"EB"
		},
		{
			"no_framerate_vigilante",
			evolve_processing,
			"55 8B EC 81 EC 34 02 00 00 53 56 57 51 8D BD 8C FE FF FF B9 5D 00 00 00 B8 CC CC CC CC F3 AB 59 89 4D F8",
			"C3"
		},
		{
			"fix_aim_movement",
			"samp.dll",
			"51 56 57 8B F9 E8",
			"C3"
		},
		{
			"no_custom_radar",
			evolve_processing,
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC 80 01 00 00 53 56 57 51 8D BD 34 FF FF FF B9 30 00 00 00 B8 CC CC CC CC F3 AB 59 A1 ?? ?? ?? ?? 33 C5 89 45 F0",
			"C3"
		},
		{
			"no_discord_activity",
			evolve_processing,
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC 74 03 00 00 53 56 57 8D BD 00 FE FF FF B9 7D 00 00 00 B8 CC CC CC CC F3 AB A1 ?? ?? ?? ?? 33 C5 89 45 F0 50 8D 45 F4 64 A3 00 00 00 00 B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8",
			"C3"
		},
		{
			"no_new_auth",
			evolve_processing,
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 51 81 EC 64 01 00 00",
			"C3"
		},
		{
			"no_new_spawnscreen",
			evolve_processing,
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC 68 01 00 00 53 56 57 8D BD 4C FF FF FF B9 2A 00 00 00 B8 CC CC CC CC F3 AB A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 0F B6 05",
			"C3"
		},
		{//patch custom dialogs (кастомные эвольвовские диалоги несовместимы с плагинами и скриптами, которые сделаны на диалогах ;))
			"no_new_spawnscreen",
			evolve_processing,
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC 18 01 00 00 53 56 57 8D 7D 9C B9 16 00 00 00 B8 CC CC CC CC F3 AB A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? D9 1D",
			"C3"
		}
	};
};