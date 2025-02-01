#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <format>
#include <fstream>
#include <chrono>

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