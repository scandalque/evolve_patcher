#include "config.hpp"
#include "plugin.hpp"

#include <fstream>

void c_settings::load() {
	dbg_println("c_settings load");

	std::ifstream file(this->path);

	if (!file) {
		save();
		return;
	}

	std::string key;
	std::string value;

	while (file >> key >> value) {
		if (key == "evolve_processing") {
			evolve_processing = value;
			continue;
		}
		data[key] = std::stoi(value);
	}

	file.close();
}

void c_settings::save() {
	if (!data.size()) {
		data = {
		{"textdraw_logotype", true},
		{"change_spawn_selection", true},
		{"fix_movement", true},
		{"no_esc_transparency", true},
		{"normal_radar_icons_size", true},
		{"no_binds", true},
		{"normal_radar", true},
		};
	}

	std::ofstream file(this->path);

	if (!file.is_open())
		return;

	for (auto& _data : this->data) {
		file << _data.first << " " << _data.second << std::endl;
	}

	file << "evolve_processing" << " " << evolve_processing << std::endl;

	file.close();
}

c_settings::c_settings() {
	dbg_println("c_settings constructor");
}

c_settings::~c_settings() {
	dbg_println("c_settings destructor");
	save();
}

c_settings* c_settings::get() {
	static c_settings instance;
	return &instance;
}