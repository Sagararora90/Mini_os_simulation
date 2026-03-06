#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>

class Logger {
public:
    static void init(const std::string& filename);
    static void log(const std::string& message);
    static void close();

private:
    static std::ofstream log_file;
    static std::string current_time();
};
