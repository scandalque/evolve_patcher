#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

namespace utils {
    std::string to_upper(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    std::string to_lower(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }
	
	char* to_upper(const char* str) {
		size_t length = std::strlen(str);
		char* result = new char[length + 1]; // +1 для нуль-терминатора
		std::strcpy(result, str);
		std::transform(result, result + length, result, ::toupper);
		return result;
	}

	char* to_lower(const char* str) {
		size_t length = std::strlen(str);
		char* result = new char[length + 1]; // +1 для нуль-терминатора
		std::strcpy(result, str);
		std::transform(result, result + length, result, ::tolower);
		return result;
	}

    std::wstring to_lower(std::wstring& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    std::wstring to_upper(std::wstring& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    std::string remove_space_from_end_str(const std::string& str) {
        size_t end = str.find_last_not_of(" \t\n\r\f\v");
        return (end == std::string::npos) ? "" : str.substr(0, end + 1);
    }

    std::vector<int> decimal_to_binary(int decimal) {
        std::vector<int> binary;

        while (decimal > 0) {
            binary.push_back(decimal % 2);
            decimal /= 2;
        }
        std::reverse(binary.begin(), binary.end());

        return binary;
    }

    bool file_exists(const std::string& filepath) {
        DWORD attr = GetFileAttributesA(filepath.c_str());
        return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
    }
}