#include "plugin.hpp"
#include "patches.hpp"
#include "memory.hpp"

#include <thread>
#include <windows.h>

c_patches* c_patches::get() {
	static c_patches instance;
	return &instance;
}

void c_patches::enable_patches() {
	std::thread([&] {
		c_settings* cfg = c_settings::get();

		while (!GetModuleHandleA(cfg->evolve_processing.c_str())) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100u));
		}
		
		for (auto& patch : patches) {
			if (cfg->data.find(patch.name) != cfg->data.end() && !cfg->data[patch.name]) {
				dbg_println("[erp patcher]: skip {} (not enabled)", patch.name);
				continue;
			}

			std::uintptr_t addr = mem::find_pattern(patch.module, patch.pattern);
			if (addr) {
				mem::safe_copy(addr, patch.new_bytes);
				dbg_println("[erp patcher]: patched {}. addr: 0x{:x}", patch.name, addr);
			}
			else {
				dbg_println("[erp patcher]: skip {} (not found addr). patch module: {}", patch.name, patch.module);
			}
		}

		
		std::uintptr_t evolve_create_hook_addr = mem::find_pattern(evolve_processing, "55 8B EC 83 EC 34 A1 ?? ?? ?? ?? 33 C5 89 45 FC 8B 45 08 53 56 8B D9");
		if (evolve_create_hook_addr) {
			c_plugin::evolve_create_hook_.set_adr(evolve_create_hook_addr);
			c_plugin::evolve_create_hook_.add(&c_plugin::evolve_create_hook);

			dbg_println("[erp patcher]: evolve create hook hooked. addr: 0x{:x}", evolve_create_hook_addr);
		}
		else {
			dbg_println("[erp patcher]: evolve create hook don't hooked (not found addr)");
		}
		}).detach();
}

c_patches::c_patches() = default;

c_patches::~c_patches() = default;