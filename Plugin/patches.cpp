#include "plugin.hpp"
#include "patches.hpp"
#include "memory.hpp"

#include <thread>
#include <windows.h>

c_patches* c_patches::get() {
	static c_patches instance;
	return &instance;
}

void c_patches::disable_patches() {
	for (auto& patch : patches) {
		dbg_println("disable_patches: patch name: {}", patch.name);
		mem::safe_copy(reinterpret_cast<void*>(patch.addr), reinterpret_cast<void*>(patch.old_bytes), patch.size); 

		if (patch.old_bytes != nullptr) {
			delete[] patch.old_bytes;
			patch.old_bytes = nullptr;
		}
	}
}

void c_patches::enable_patches() {
	std::thread([&] {
		c_settings* cfg = c_settings::get();

		while (!GetModuleHandleA(cfg->evolve_processing.c_str())) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100u));
		}

		for (auto& patch : patches) {
			if (!cfg->data[patch.name]) {
				dbg_println("enable_patches: skip {} (not enabled)", patch.name);
				continue;
			}
			patch.old_bytes = new char[patch.size];
			if (patch.addr) {
				mem::safe_copy(reinterpret_cast<void*>(patch.old_bytes), reinterpret_cast<void*>(patch.addr), patch.size);
				mem::safe_copy(reinterpret_cast<void*>(patch.addr), reinterpret_cast<void*>(patch.new_bytes), patch.size);

				dbg_println("enable_patches: patched by address. patch name: {}. patch addr: {:x}", patch.name, patch.addr);
			}
			else {
				std::uintptr_t module_base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(patch.module.c_str()));
				patch.addr = mem::find_pattern(module_base, mem::get_module_size(module_base), patch.bytes, patch.mask);

				if (!patch.addr) {
					dbg_println("enable_patches: skip {} (not founded addr by pattern)", patch.name);
					continue;
				}

				mem::safe_copy(reinterpret_cast<void*>(patch.old_bytes), reinterpret_cast<void*>(patch.addr), patch.size);
				mem::safe_copy(reinterpret_cast<void*>(patch.addr), reinterpret_cast<void*>(patch.new_bytes), patch.size);

				dbg_println("enable_patches: patched by pattern. patch name: {}. patch addr: {:x}", patch.name, patch.addr);
			}
		}
		}).detach();
}

c_patches::c_patches() {
	dbg_println("c_patches constructor");
}

c_patches::~c_patches() {
	dbg_println("c_patches destructor");
	disable_patches();
}