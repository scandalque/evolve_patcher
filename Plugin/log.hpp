#pragma once

#include <format>
#include <chrono>
#include <fstream>
#include <string>

inline std::string get_current_time() {
    auto now = std::chrono::system_clock::now();
    auto now_local = std::chrono::current_zone()->to_local(now);
    return std::format("[{:%Y-%m-%d %H:%M:%S}] ", now_local);
}

inline void clear_log() {
    std::ofstream file("!!evolve_patcher.log", std::ios::trunc);
    if (!file.is_open()) {
        return;
    }
    file.close();
}


template<typename... Args>
inline void dbg_println(std::format_string<Args...> format, Args&&... args) {
    std::ofstream file("!!evolve_patcher.log", std::ios::app);
    if (!file.is_open()) {
        return;
    }

    file << get_current_time() << std::format(format, std::forward<Args>(args)...);
    file << std::endl;
    file.close(); 
}
