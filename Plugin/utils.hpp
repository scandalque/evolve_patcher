#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <format>
#include <fstream>
#include <chrono>

// all shit here

namespace utils {
    inline std::string log_path = "";

    inline std::string get_current_time() {
        auto now = std::chrono::system_clock::now();
        auto now_local = std::chrono::current_zone()->to_local(now);
        return std::format("[{:%Y-%m-%d %H:%M:%S}] ", now_local);
    }

    inline void clear_log() {
        std::ofstream file(log_path, std::ios::trunc);
        if (!file.is_open()) {
            return;
        }
        file.close();
    }

    template<typename... Args>
    inline void log(std::format_string<Args...> format, Args&&... args) {
        std::ofstream file(log_path, std::ios::app);
        if (!file.is_open()) {
            return;
        }

        std::string log_text = std::format(format, std::forward<Args>(args)...);

#ifdef DBG
        std::cout << "[erp patcher]: " << log_text << std::endl;
#endif

        file << get_current_time() << log_text << std::endl;
        file.close();
    }

    inline std::string to_upper(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    inline std::string to_lower(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }
	
    inline char* to_upper(const char* str) {
		size_t length = std::strlen(str);
		char* result = new char[length + 1];
		std::strcpy(result, str);
		std::transform(result, result + length, result, ::toupper);
		return result;
	}

    inline char* to_lower(const char* str) {
		size_t length = std::strlen(str);
		char* result = new char[length + 1];
		std::strcpy(result, str);
		std::transform(result, result + length, result, ::tolower);
		return result;
	}

    inline std::wstring to_lower(std::wstring& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    inline std::wstring to_upper(std::wstring& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    inline std::string remove_space_from_end_str(const std::string& str) {
        size_t end = str.find_last_not_of(" \t\n\r\f\v");
        return (end == std::string::npos) ? "" : str.substr(0, end + 1);
    }

    inline std::vector<int> decimal_to_binary(int decimal) {
        std::vector<int> binary;

        while (decimal > 0) {
            binary.push_back(decimal % 2);
            decimal /= 2;
        }
        std::reverse(binary.begin(), binary.end());

        return binary;
    }

    inline bool file_exists(const std::string& filepath) {
        DWORD attr = GetFileAttributesA(filepath.c_str());
        return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
    }
}

namespace dx9 {
    inline void* set_vtable_pointer(void* class_ptr, std::size_t index, void* value) {
        void** vtbl = *reinterpret_cast<void***>(class_ptr);
        void* prev = vtbl[index];
        vtbl[index] = value;
        return prev;
    }

    inline std::uintptr_t find_device(std::uint32_t Len) {
        static std::uintptr_t base = [](std::size_t Len) {
            std::string path_to(MAX_PATH, '\0');
            if (auto size = GetSystemDirectoryA(path_to.data(), MAX_PATH)) {
                path_to.resize(size);
                path_to += "\\d3d9.dll";
                std::uintptr_t dwObjBase = reinterpret_cast<std::uintptr_t>(LoadLibraryA(path_to.c_str()));
                while (dwObjBase++ < dwObjBase + Len) {
                    if (*reinterpret_cast<std::uint16_t*>(dwObjBase + 0x00) == 0x06C7 &&
                        *reinterpret_cast<std::uint16_t*>(dwObjBase + 0x06) == 0x8689 &&
                        *reinterpret_cast<std::uint16_t*>(dwObjBase + 0x0C) == 0x8689) {
                        dwObjBase += 2;
                        break;
                    }
                }
                return dwObjBase;
            }
            return std::uintptr_t(0);
            }(Len);
        return base;
    }

    inline std::uintptr_t get_function_address(int VTableIndex) {
        return (*reinterpret_cast<std::uintptr_t**>(find_device(0x128000)))[VTableIndex];
    }
}