#pragma once

#include "process_manager.h"
#include "scheduler.h"
#include "memory_manager.h"
#include "filesystem.h"
#include "system_monitor.h"
#include <string>

class Shell {
public:
    Shell(ProcessManager& pm, Scheduler& sched, MemoryManager& mm, FileSystem& fs, SystemMonitor& sm);

    void run();

private:
    ProcessManager& pm;
    Scheduler& sched;
    MemoryManager& mm;
    FileSystem& fs;
    SystemMonitor& sm;

    void parse_command(const std::string& command);
    void print_help();
};
