 #include "plugin.hpp"
#include "patches.hpp"
#include "memory.hpp"
#include "utils.hpp"

#include <thread>
#include <windows.h>

c_patches* c_patches::get() {
	static c_patches instance;
	return &instance;
}

std::vector<std::string> c_patches::get_patches_names() {
	std::vector<std::string> __patches;
	for (auto& patch : patches) {
		__patches.push_back(patch.name);
	}

	return __patches;
}

void c_patches::enable_patches() {
	utils::log("enable patches called");

	std::thread([&] {
		c_settings* cfg = c_settings::get();

		while (!GetModuleHandleA(evolve_processing.c_str())) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		std::uintptr_t evolve_create_hook_addr = 0;
		evolve_create_hook_addr = mem::find_pattern(evolve_processing, "55 8B EC 81 EC D8 00 00 00 53 56 57 8D 7D E8 B9 06 00 00 00 B8 CC CC CC CC F3 AB B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B F4");

		if (evolve_create_hook_addr) {
			c_plugin::evolve_create_hook_.set_adr(evolve_create_hook_addr);
			c_plugin::evolve_create_hook_.add(&c_plugin::evolve_create_hook);

			utils::log("evolve create hook hooked. addr: 0x{:x}", evolve_create_hook_addr);
		}
		else {
			utils::log("evolve create hook don't hooked (not found addr)");
		}

		for (auto& patch : patches) {
			if (cfg->data.find(patch.name) != cfg->data.end() && !cfg->data[patch.name]) {
				utils::log("skip {} (not enabled)", patch.name);
				continue;
			}

			std::uintptr_t addr = mem::find_pattern(patch.module, patch.pattern);
			if (addr) {
				mem::safe_copy(addr, patch.new_bytes);
				utils::log("patched {}. addr: 0x{:x}", patch.name, addr);
			}
			else {
				utils::log("skip {} (not found addr). patch module: {}", patch.name, patch.module);
			}
		}
		}).detach();
}

c_patches::c_patches() = default;

c_patches::~c_patches() = default;