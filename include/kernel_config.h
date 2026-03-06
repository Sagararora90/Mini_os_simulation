#pragma once

#include <string>

struct KernelConfig {
    std::string scheduler_type;
    int time_quantum;
    int total_memory;
    int max_processes;
    std::string log_level;

    // Load defaults
    KernelConfig() 
        : scheduler_type("ROUND_ROBIN"), time_quantum(2), 
          total_memory(1024), max_processes(50), log_level("INFO") {}
          
    void load_from_file(const std::string& filepath);
    void print_config() const;
};
