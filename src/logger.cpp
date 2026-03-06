#include "logger.h"
#include <iostream>
#include <iomanip>

std::ofstream Logger::log_file;

void Logger::init(const std::string& filename) {
    log_file.open(filename, std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "Failed to open log file: " << filename << "\n";
    }
}

std::string Logger::current_time() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::string timestamp;
    char buffer[100];
#if defined(_MSC_VER)
    struct tm timeinfo;
    localtime_s(&timeinfo, &in_time_t);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", &timeinfo);
#else
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", std::localtime(&in_time_t));
#endif
    return std::string(buffer);
}

void Logger::log(const std::string& message) {
    std::string msg = "[" + current_time() + "] " + message;
    if (log_file.is_open()) {
        log_file << msg << std::endl;
    }
}

void Logger::close() {
    if (log_file.is_open()) {
        log_file.close();
    }
}
