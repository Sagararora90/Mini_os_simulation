#include "kernel_config.h"
#include <iostream>
#include <fstream>
#include <sstream>

void KernelConfig::load_from_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open kernel config at " << filepath 
                  << ". Using default configuration.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::size_t eq_pos = line.find('=');
        if (eq_pos != std::string::npos) {
            std::string key = line.substr(0, eq_pos);
            std::string val = line.substr(eq_pos + 1);

            // Basic parsing logic
            if (key == "SCHEDULER") scheduler_type = val;
            else if (key == "TIME_QUANTUM") time_quantum = std::stoi(val);
            else if (key == "TOTAL_MEMORY") total_memory = std::stoi(val);
            else if (key == "MAX_PROCESSES") max_processes = std::stoi(val);
            else if (key == "LOG_LEVEL") log_level = val;
        }
    }
}

void KernelConfig::print_config() const {
    std::cout << "\nKernel Configuration\n";
    std::cout << "---------------------\n";
    std::cout << "Scheduler     : " << scheduler_type << "\n";
    std::cout << "Time Quantum  : " << time_quantum << "\n";
    std::cout << "Total Memory  : " << total_memory << " KB\n";
    std::cout << "Max Processes : " << max_processes << "\n";
    std::cout << "Log Level     : " << log_level << "\n\n";
}
