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

	if (!cfg->data["textdraw_logotype"])
		return;

	std::string host_name(c_netgame::get()->ref()->hostname);
	std::string server_name;
	std::regex pattern(R"(Server:\s*([^|]+))");
	std::smatch match;
	if (std::regex_search(host_name, match, pattern)) {
		if (match.size() > 1) {
			auto string = utils::remove_space_from_end_str(match[1].str());
			memcpy(data::textdraws[19].text, utils::to_upper(string.c_str()), string.size());
			//data::textdraws[19].text = utils::to_upper(string.c_str());
		}
	}

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

		//bs->Write(reinterpret_cast<char*>(&td), sizeof(data::st_textdraw));
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

	if (!cfg->data["change_spawn_selection"])
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

	if (!cfg->data["change_spawn_selection"])
		return;

	dbg_println("show_spawn_change_dialog");
	
	auto text = std::format("{:s}Спавн\n{:s}Недвижимое имущество\n{:s}Дом на колесах\n{:s}Частная яхта\n{:s}Семья",
		(last_spawn_data[1] ? "Последнее место выхода\n" : ""),
		(last_spawn_data[2] ? "{ffffff}" : "{afafaf}"),
		(last_spawn_data[3] ? "{ffffff}" : "{afafaf}"),
		(last_spawn_data[5] ? "{ffffff}" : "{afafaf}"),
		(last_spawn_data[4] ? "{ffffff}" : "{afafaf}"));
		

	dbg_println("text format");

	c_dialog::get()->ref()->show(655, 2, "{ffffff}Возрождение | {ae433d}Настройки", text.c_str(), "»", "«", 0);

	dbg_println("showed");
}

void __fastcall c_plugin::dialog_close(c_dialog* _this, void* edx, uint8_t arg0) {
	if(_this->id == 655) {
		int listitem = c_dialog::get()->get_listitem(_this->listbox, -1);
		dbg_println("listitem: {}", listitem);

		if (last_spawn_data[1]) {
			switch (listitem) {
			case 0: {
				send_spawn_change_response(0);
				break;
			}
			case 1: {
				send_spawn_change_response(1);
				break;
			}
			case 2: {
				if (!last_spawn_data[2]) {
					show_spawn_change_dialog();
					return;
				}
				send_spawn_change_response(2);
				break;
			}
			case 3: {
				if (!last_spawn_data[3]) {
					show_spawn_change_dialog();
					return;
				}
				send_spawn_change_response(3);
				break;
			}
			case 4: {
				if (!last_spawn_data[5]) {
					show_spawn_change_dialog();
					return;
				}
				send_spawn_change_response(5);
				break;
			}
			case 5: {
				if (!last_spawn_data[4]) {
					show_spawn_change_dialog();
					return;
				}
				send_spawn_change_response(4);
				break;
			}
			}
		}
		else {
			switch (listitem) {
			case 0: {
				send_spawn_change_response(1);
				break;
			}
			case 1: {
				if (!last_spawn_data[2]) {
					show_spawn_change_dialog();
					return;
				}
				send_spawn_change_response(2);
				break;
			}
			case 2: {
				if (!last_spawn_data[3]) {
					show_spawn_change_dialog();
					return;
				}
				send_spawn_change_response(3);
				break;
			}
			case 3: {
				if (!last_spawn_data[5]) {
					show_spawn_change_dialog();
					return;
				}
				send_spawn_change_response(5);
				break;
			}
			case 4: {
				if (!last_spawn_data[4]) {
					show_spawn_change_dialog();
					return;
				}
				send_spawn_change_response(4);
				break;
			}
			}
		}
	}
	return dialog_close_hook.call_original(_this, edx, arg0);
}

HANDLE WINAPI c_plugin::create_file_w(LPCWSTR filename, DWORD desired_access, DWORD share_mode, LPSECURITY_ATTRIBUTES security_attr,
	DWORD creation_dispotion, DWORD flags_attr, HANDLE template_file) {

	std::wstring str_filename(filename);
	utils::to_lower(str_filename);

	//std::wcout << filename << std::endl;

	if (str_filename.ends_with(L"evolve\\effectspc.txd") && utils::file_exists("models\\effectspc.txd")) {
		//std::wcout << L"hooked" << std::endl;
		return create_file_w_hook.call_original(L"models\\effectsPC.txd", desired_access, share_mode, security_attr,
			creation_dispotion, flags_attr, template_file);
	}

	if (str_filename.ends_with(L"evolve\\effects.fxp") && utils::file_exists("models\\effects.fxp")) {
		//std::wcout << L"hooked" << std::endl;
		return create_file_w_hook.call_original(L"models\\effects.fxp", desired_access, share_mode, security_attr,
			creation_dispotion, flags_attr, template_file);
	}

	if (str_filename.ends_with(L"evolve\\particle.txd") && utils::file_exists("models\\particle.txd")) {
		//std::wcout << L"hooked" << std::endl;
		return create_file_w_hook.call_original(L"models\\particle.txd", desired_access, share_mode, security_attr,
			creation_dispotion, flags_attr, template_file);
	}

	return create_file_w_hook.call_original(filename, desired_access, share_mode, security_attr,
		creation_dispotion, flags_attr, template_file);;
}

void c_plugin::game_loop() {
	static bool initialized = false;

	if (initialized || !rakhook::initialize() || c_chat::get()->ref() == nullptr)
		return game_loop_hook.call_original();

	initialized = true;
	StringCompressor::AddReference();

	dialog_close_hook.set_adr(rakhook::samp_addr(offsets::dialog::close[VERSION]));
	dialog_close_hook.add(&c_plugin::dialog_close);

	return_normal_radar_icons_size();

	rakhook::on_receive_packet += [](Packet* p) -> bool {
		if (+*(p->data) == 251 && c_settings::get()->data["change_spawn_selection"]) {
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

		if (id == 52 && cfg->data["textdraw_logotype"]) {
			create_textdraws();
		}
		return true;
		};

	return game_loop_hook.call_original();
}

c_plugin::c_plugin(HMODULE hmodule) : hmodule(hmodule)
{
	dbg_println("c_plugin constructor");
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
	cfg->path = std::filesystem::path(mod_path).replace_extension("cfg").string();

	cfg->load();
	patches->enable_patches();
	auto kernel32_addr = LoadLibraryA("kernel32.dll");
	if(kernel32_addr)
		create_file_w_hook.set_adr(reinterpret_cast<std::uintptr_t>(GetProcAddress(kernel32_addr, "CreateFileW")));
	create_file_w_hook.add(&c_plugin::create_file_w);

	game_loop_hook.add(&c_plugin::game_loop);
}


c_plugin::~c_plugin()
{
	dbg_println("c_plugin denstructor");

	c_settings* cfg = c_settings::get();

	cfg->save();

	game_loop_hook.remove();
	create_file_w_hook.remove();
	remove_textdraws();

	rakhook::destroy();
}
