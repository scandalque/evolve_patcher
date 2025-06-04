#include "config.hpp"
#include "plugin.hpp"
#include "utils.hpp"

#include <fstream>

void c_settings::load() {
	utils::log("config load called");

	std::ifstream file(this->path);

	if (!file) {
		utils::log("file not found, saving");
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

		utils::log("data[{}] = {}", key, std::stoi(value));
	}

	file.close();
}

void c_settings::save() {
	utils::log("config save called");
	if (!data.size()) {
		utils::log("data is empty");
		for (auto& patch_name : c_patches::get()->get_patches_names()) {
			data[patch_name] = true;
		}
		data["no_small_icons"] = true;
	}

	std::ofstream file(this->path);

	if (!file.is_open()) {
		utils::log("config save: !file.isopen");
		return;
	}

	for (auto& _data : this->data) {
		utils::log("config save: ", _data.first, _data.second);
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