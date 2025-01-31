  #include "plugin.hpp"
#include "config.hpp"
#include "data.hpp"
#include "utils.hpp"

#include <RakNet/PacketEnumerations.h>
#include <RakNet/StringCompressor.h>

#include <regex>
#include <vector>
#include <format>
#include <filesystem>
#include <set>

#ifdef DBG
#include <intrin.h>
#pragma intrinsic(_ReturnAddress)
#endif

void c_plugin::remove_textdraws() {
	for (auto& td : data::textdraws) {
		RakNet::BitStream* bs = new RakNet::BitStream();

		bs->Write(td.id);
		rakhook::emul_rpc(135, *bs);

		delete bs;
	}
}

void c_plugin::create_textdraws() {
	c_settings* cfg = c_settings::get();

	if (!cfg->data["no_evolve_render"])
		return;

	std::string host_name(c_netgame::get()->ref()->hostname);
	std::string server_name;
	std::regex pattern(R"(Server:\s*([^|]+))");
	std::smatch match;
	if (std::regex_search(host_name, match, pattern)) {
		if (match.size() > 1) {
			auto string = utils::remove_space_from_end_str(match[1].str());
			memcpy(data::textdraws[19].text, utils::to_upper(string.c_str()), string.size());
		}
	}
	else return;

	for (auto &td : data::textdraws) {
		RakNet::BitStream* bs = new RakNet::BitStream();
		
		bs->Write(td.id);
		bs->Write(td.flags);
		bs->Write(td.letter_width);
		bs->Write(td.letter_height);
		bs->Write(td.letter_color);
		bs->Write(td.line_width);
		bs->Write(td.line_height);
		bs->Write(td.box_color);
		bs->Write(td.shadow);
		bs->Write(td.outline);
		bs->Write(td.background_color);
		bs->Write(td.style);
		bs->Write(td.selectable);
		bs->Write(td.x);
		bs->Write(td.y);
		bs->Write(td.model_id);
		bs->Write(td.rotation_x);
		bs->Write(td.rotation_y);
		bs->Write(td.rotation_z);
		bs->Write(td.zoom);
		bs->Write(td.color_1);
		bs->Write(td.color_2);
		bs->Write(td.size);
		bs->Write(td.text, td.size);

		rakhook::emul_rpc(134, *bs);
		delete bs;
	}
}

void c_plugin::return_normal_radar_icons_size() {
	for (auto& p : data::icons) {
		**reinterpret_cast<float**>(p.addr) = p.new_value;
	}
}

void c_plugin::send_spawn_change_response(uint8_t value) {
	c_settings* cfg = c_settings::get();

	if (!cfg->data["no_new_spawnscreen"])
		return;

	RakNet::BitStream* bs = new RakNet::BitStream();

	bs->Write<uint8_t>(251);
	bs->Write<uint32_t>(9);
	bs->Write<uint8_t>(value);

	rakhook::send(bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, (char)0);

	delete bs;
}

void c_plugin::show_spawn_change_dialog() {
	c_settings* cfg = c_settings::get();

	if (!cfg->data["no_new_spawnscreen"])
		return;

	auto text = std::format("{:s}Спавн\n{:s}Недвижимое имущество\n{:s}Дом на колесах\n{:s}Частная яхта\n{:s}Семья",
		(last_spawn_data[1] ? "Последнее место выхода\n" : ""),
		(last_spawn_data[2] ? "{ffffff}" : "{afafaf}"),
		(last_spawn_data[3] ? "{ffffff}" : "{afafaf}"),
		(last_spawn_data[5] ? "{ffffff}" : "{afafaf}"),
		(last_spawn_data[4] ? "{ffffff}" : "{afafaf}"));
		
	c_dialog::get()->ref()->show(655, 2, "{ffffff}Возрождение | {ae433d}Настройки", text.c_str(), "»", "«", 0);
}

void __fastcall c_plugin::dialog_close(c_dialog* _this, void* edx, uint8_t arg0) {
	if(_this->id == 655 && arg0) {
		int listitem = c_dialog::get()->get_listitem(_this->listbox, -1);

		if (last_spawn_data[1]) {
			std::vector<uint8_t> r{ 0,1,2,3,5,4 };
			if (listitem > 1 && !last_spawn_data[r[listitem]]) {
				show_spawn_change_dialog();
				return;
			}
			send_spawn_change_response(r[listitem]);
		}
		else {
			std::vector<uint8_t> r{ 1,2,3,5,4 };
			if (listitem > 0 && !last_spawn_data[r[listitem]]) {
				show_spawn_change_dialog();
				return;
			}
			send_spawn_change_response(r[listitem]);
		}
	}
	return dialog_close_hook.call_original(_this, edx, arg0);
}

#ifdef DBG
std::string get_caller_module(void* func)
{
	HMODULE h_module = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)func, &h_module);

	if (h_module != nullptr)
	{
		TCHAR sz_module[MAX_PATH];
		if (GetModuleFileName(h_module, sz_module, MAX_PATH))
			return std::filesystem::path(sz_module).filename().string();
	}

	return "";
}
#endif

int __fastcall c_plugin::evolve_create_hook(void* addr, void* cb, void** orig) {

#ifdef DBG
	static std::map<std::uintptr_t, int> hook_counts;

	static int hook_count = 1;
	std::string module_name = get_caller_module(addr);
	std::uintptr_t module_base_addr = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(module_name.c_str()));
	std::uintptr_t offset = (reinterpret_cast<std::uintptr_t>(addr) - module_base_addr);

	hook_counts[reinterpret_cast<std::uintptr_t>(addr)]++;
	dbg_println("[{}] evolve_create_hook: 0x{:x} [{} + 0x{:x}] hook counts on this address: {}", hook_count, reinterpret_cast<std::uintptr_t>(addr), module_name,
		offset, hook_counts[reinterpret_cast<std::uintptr_t>(addr)]);
	hook_count++;
#endif

	// remove checks for entering other servers
	std::uintptr_t samp_addr = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("samp.dll"));
	if (samp_addr) {
		if (reinterpret_cast<void*>(samp_addr + 0xad70) == addr) {
			return 0;
		}
	}

	if (create_file_a_addr == addr) {
		dbg_println("[erp patcher] set new callback for create file a hook");
		create_file_a_cb = cb;
		create_file_a_orig = orig;
		return evolve_create_hook_.call_original(addr, &c_plugin::create_file_a, orig);
	}

	return evolve_create_hook_.call_original(addr, cb, orig);
}

HANDLE WINAPI c_plugin::create_file_a(LPCSTR filename, DWORD desired_access, DWORD share_mode, LPSECURITY_ATTRIBUTES security_attr,
	DWORD creation_dispotion, DWORD flags_attr, HANDLE template_file) {
	std::string _filename(filename);
	utils::to_lower(_filename);
	if (_filename.ends_with("effectspc.txd") || _filename.ends_with("effects.fxp") || _filename.ends_with("particle.txd")) {
		dbg_println("[erp patcher] hooked create file a for {}", _filename);
		return reinterpret_cast<decltype(&create_file_a)>(*create_file_a_orig)
			(filename, desired_access, share_mode, security_attr, creation_dispotion, flags_attr, template_file);
	}

	return reinterpret_cast<decltype(&create_file_a)>(create_file_a_cb)
		(filename, desired_access, share_mode, security_attr, creation_dispotion, flags_attr, template_file);
}

void c_plugin::game_loop() {
	static bool initialized = false;
	static bool hwnd_initialized = false;

	if (initialized || !rakhook::initialize() || c_chat::get()->ref() == nullptr)
		return game_loop_hook.call_original();

	initialized = true;
	StringCompressor::AddReference();

	dialog_close_hook.set_adr(rakhook::samp_addr(offsets::dialog::close[VERSION]));
	dialog_close_hook.add(&c_plugin::dialog_close);

	if(c_settings::get()->data["no_small_icons"])
		return_normal_radar_icons_size();

	rakhook::on_receive_packet += [](Packet* p) -> bool {
		if (+*(p->data) == 251 && c_settings::get()->data["no_new_spawnscreen"]) {
			RakNet::BitStream* bs = new RakNet::BitStream(p->data, p->length, false);

			bs->IgnoreBits(8);
			uint32_t event_type;
			bs->Read(event_type);

			if (event_type == 9) {
				uint8_t value;
				bs->Read(value);

				last_spawn_data = utils::decimal_to_binary(value);

				show_spawn_change_dialog();
				return false;

			}
			delete bs;
		}
		return true;
		};

	rakhook::on_send_rpc += [](int& id, RakNet::BitStream* bs, PacketPriority& priority, PacketReliability& reliability, char& ord_channel, bool& sh_timestamp) -> bool {
		c_settings* cfg = c_settings::get();

		if (id == 52 && cfg->data["no_evolve_render"]) {
			create_textdraws();
		}
		return true;
		};

	return game_loop_hook.call_original();
}

c_plugin::c_plugin(HMODULE hmodule) : hmodule(hmodule)
{
#ifdef DBG
	[]{
		if (!AllocConsole())
			return;

		FILE* f;
		freopen_s(&f, "CONOUT$", "w", stdout);
		freopen_s(&f, "CONOUT$", "w", stderr);
		freopen_s(&f, "CONIN$", "r", stdin);
		SetConsoleOutputCP(1251);
		}();
#endif

	c_settings* cfg = c_settings::get();
	c_patches* patches = c_patches::get();

	char mod_path[MAX_PATH] = { 0 };
	GetModuleFileNameA(hmodule, mod_path, MAX_PATH);
	std::filesystem::path path(mod_path);
	
	if (path.filename() != "!!evolve_patcher.asi") {
		MessageBoxA(NULL, "Обнаружено переименование файла\nДля корректной работы переименуйте плагин в !!evolve_patcher.asi", "evolve patcher", 1);
	}
	
	cfg->path = std::filesystem::path(mod_path).replace_extension("cfg").string();

	cfg->load();
	
	
	auto kernel32_addr = LoadLibraryA("kernel32.dll");
	if (kernel32_addr) {
		dbg_println("kernel32 loaded, addr: {:x}", reinterpret_cast<std::uintptr_t>(kernel32_addr));
		create_file_a_addr = reinterpret_cast<void*>(GetProcAddress(kernel32_addr, "CreateFileA"));
		dbg_println("createfilea addr: {:x}", reinterpret_cast<std::uintptr_t>(create_file_a_addr));
	}

	patches->enable_patches();

	game_loop_hook.add(&c_plugin::game_loop);
}


c_plugin::~c_plugin()
{
	c_settings* cfg = c_settings::get();

	cfg->save();

	game_loop_hook.remove();
	remove_textdraws();

	rakhook::destroy();
}
