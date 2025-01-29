#pragma once
#include <cstring>
#include <iostream>
#include <windows.h>
#include <sstream>
#include <vector>

//lemonhook))
namespace mem {
	class unprot {
	public:
		unprot(std::uintptr_t addr, std::size_t size);
		unprot(void* ptr, std::size_t size);
		~unprot();

	private:
		std::uintptr_t addr_ = 0;
		std::size_t	   size_ = 0;

		DWORD old_prot_ = 0;
	};

	unprot::unprot(std::uintptr_t addr, std::size_t size)
		: addr_(addr), size_(size) {
		VirtualProtect(reinterpret_cast<void*>(addr_), size_, PAGE_EXECUTE_READWRITE, &old_prot_);
	}

	unprot::unprot(void* ptr, std::size_t size)
		: addr_(reinterpret_cast<std::uintptr_t>(ptr)), size_(size) {
		VirtualProtect(reinterpret_cast<void*>(addr_), size_, PAGE_EXECUTE_READWRITE, &old_prot_);
	}

	unprot::~unprot() {
		VirtualProtect(reinterpret_cast<void*>(addr_), size_, old_prot_, &old_prot_);
	}

	void safe_copy(void* dst, const void* src, std::size_t size) {
		unprot unprot(dst, size);
		std::memcpy(dst, src, size);
	}

	void safe_copy(std::uintptr_t dst, std::uintptr_t src, std::size_t size) {
		safe_copy(reinterpret_cast<void*>(dst), reinterpret_cast<void*>(src), size);
	}

	void safe_set(void* dst, std::uint8_t value, std::size_t size) {
		unprot unprot(dst, size);
		std::memset(dst, value, size);
	}

	void safe_set(std::uintptr_t dst, std::uint8_t value, std::size_t size) {
		safe_set(reinterpret_cast<void*>(dst), value, size);
	}

	bool compare_bytes(const std::uint8_t* data, const std::uint8_t* bytes, const char* mask) {
		if (!data || !bytes || !mask) return false;
		for (; *mask; ++mask, ++data, ++bytes) {
			if (*mask == 'x' && std::memcmp(data, bytes, 1) != 0) {
				return false;
			}
		}
		return *mask == 0;
	}

	bool compare_bytes(const std::uint8_t* data, const char* bytes, const char* mask) {
		return compare_bytes(data, reinterpret_cast<const std::uint8_t*>(bytes), mask);
	}

	std::uintptr_t find_pattern(std::uintptr_t base, std::size_t len, const std::uint8_t* bytes, const char* mask) {
		for (auto i = 0u; i < len; ++i) {
			if (compare_bytes(reinterpret_cast<std::uint8_t*>(base + i), bytes, mask)) {
				return base + i;
			}
		}
		return 0;
	}

	std::size_t get_module_size(std::uintptr_t module) {
		return reinterpret_cast<IMAGE_NT_HEADERS*>(module + reinterpret_cast<IMAGE_DOS_HEADER*>(module)->e_lfanew)->OptionalHeader.SizeOfImage;
	}

	void find_all_patterns(std::uintptr_t base, std::size_t len, const char* bytes, const char* mask, std::vector<std::uintptr_t> &vec) {

		for (auto i = 0u; i < len; ++i) {
			if (compare_bytes(reinterpret_cast<std::uint8_t*>(base + i), reinterpret_cast<const std::uint8_t * >(bytes), mask)) {
				vec.push_back(base + i);
			}
		}
		return;
	}

	std::uintptr_t find_pattern(std::uintptr_t base, std::size_t len, const char* bytes, const char* mask) {
		return find_pattern(base, len, reinterpret_cast<const std::uint8_t*>(bytes), mask);
	}

	std::uintptr_t get_call_address(uintptr_t call_instruction_address) {
		BYTE call_instruction[5];
		safe_copy(call_instruction, (void*)call_instruction_address, sizeof(call_instruction));

		if (call_instruction[0] != 0xE8 && call_instruction[0] != 0xE9) {
			return 0;
		}

		int32_t relative_address = *(int32_t*)(call_instruction + 1);
		uintptr_t absolute_address = call_instruction_address + 5 + relative_address;

		return absolute_address;
	}

	//new

	std::pair<std::vector<std::uint8_t>, std::string> parse_pattern(const std::string& pattern) {
		std::vector<std::uint8_t> bytes;
		std::string mask;
		std::istringstream stream(pattern);
		std::string byte;

		while (stream >> byte) {
			if (byte == "??" || byte == "?") {
				bytes.push_back(0);
				mask += '?';
			}
			else {
				bytes.push_back(static_cast<std::uint8_t>(std::stoul(byte, nullptr, 16)));
				mask += 'x';
			}
		}

		return { bytes, mask };
	}

	std::uintptr_t find_pattern(const std::string& base, const std::string& pattern) {
		std::uintptr_t base_addr = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(base.c_str()));
		if (base_addr) {
			std::size_t len = get_module_size(base_addr);
			auto [bytes, mask] = parse_pattern(pattern);

			for (std::size_t i = 0; i < len; ++i) {
				if (compare_bytes(reinterpret_cast<std::uint8_t*>(base_addr + i), bytes.data(), mask.c_str())) {
					return base_addr + i;
				}
			}
		}
		return 0;
	}

	void safe_copy(void* dst, const std::string& byte_pattern) {
		auto bytes = parse_pattern(byte_pattern).first;
		unprot unprot(dst, bytes.size());
		std::memcpy(dst, bytes.data(), bytes.size());
	}

	void safe_set(void* dst, const std::string& byte_pattern) {
		auto bytes = parse_pattern(byte_pattern).first;
		unprot unprot(dst, bytes.size());
		std::memset(dst, bytes[0], bytes.size());
	}

	void safe_copy(std::uintptr_t dst, const std::string& byte_pattern) {
		safe_copy(reinterpret_cast<void*>(dst), byte_pattern);
	}

	void safe_set(std::uintptr_t dst, const std::string& byte_pattern) {
		safe_set(reinterpret_cast<void*>(dst), byte_pattern);
	}

}