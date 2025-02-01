#include "config.hpp"
#include "plugin.hpp"

#include <fstream>

void c_settings::load() {
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
			{"no_evolve_render", true},
			{"no_esc_transparency", true},
			{"no_binds", true},
			{"no_framerate_vigilante", true},
			{"fix_aim_movement", true},
			{"no_custom_radar", true},
			{"no_discord_activity", true},
			{"no_new_auth", true},
			{"no_radial_menu", true},
			{"no_new_spawnscreen", true},
			{"no_voice_chat", false},
			{"no_custom_dialogs", true},
			{"no_custom_nametags", true},
			{"no_small_icons", true},
			{"no_custom_chat", true},
			{"no_custom_menu", false},
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

c_settings::c_settings() = default;

c_settings::~c_settings() {
	save();
}

c_settings* c_settings::get() {
	static c_settings instance;
	return &instance;
}