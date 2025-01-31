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
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 7C 56",
			"C3"
		},
		{
			"no_esc_transparency",
			evolve_processing,
			"53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 53 81 EC D0 00 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 EC 56 57 50 8D 45 F4 64 A3 00 00 00 00 8B 35",
			"C3"
		},
		{
			"no_binds",
			evolve_processing,
			"83 38 00 0F 84 B4 02 00 00",
			"31 C0 90"
		},
		{
			"no_framerate_vigilante",
			evolve_processing,
			"55 8B EC 83 EC 0C A1 ?? ?? ?? ?? 33 C5 89 45 FC 56 57 8D 45 F4",
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
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 0C A1 ?? ?? ?? ?? 33 C5 89 45 F0 53 56 50",
			"C3"
		},
		{
			"no_discord_activity",
			evolve_processing,
			"53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 53 81 EC 08 01 00 00",
			"C3"
		},
		{
			"no_new_auth",
			evolve_processing,
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 54 A1 ?? ?? ?? ?? 33 C5 89 45 EC",
			"C3"
		},
		{
			"no_radial_menu",
			evolve_processing,
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 2C 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 80 3D",
			"C3"
		},
		{
			"no_new_spawnscreen",
			evolve_processing,
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 54 56 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 80 3D ?? ?? ?? ?? ?? 0F 85 53 01 00 00",
			"C3"
		},
		{
			"no_voice_chat",
			evolve_processing,
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 58 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 80 3D",
			"C3"
		},
		{
			"no_custom_dialogs",
			evolve_processing,
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 2C 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 8B F2",
			"C3"
		},
		{
			"no_custom_nametags",
			evolve_processing,
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 54 56 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 80 3D ?? ?? ?? ?? ?? 0F 85 7C 01 00 00",
			"C3"
		},
		{
			"no_custom_menu",
			evolve_processing,
			"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 5C A1 ?? ?? ?? ?? 33 C5 89 45 F0 53 56 57 50 8D 45 F4 64 A3 00 00 00 00 80 3D",
			"C3"
		},
		{
			"no_custom_chat",
			evolve_processing,
			"55 8B EC 83 E4 F8 E8",
			"C3"
		}
	};
};