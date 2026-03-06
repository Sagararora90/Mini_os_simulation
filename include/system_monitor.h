#pragma once

#include "process_manager.h"
#include "scheduler.h"
#include "memory_manager.h"
#include "filesystem.h"

class SystemMonitor {
public:
    SystemMonitor(ProcessManager& pm, Scheduler& sched, MemoryManager& mm, FileSystem& fs);

    void display_system_status() const;

private:
    ProcessManager& pm;
    Scheduler& sched;
    MemoryManager& mm;
    FileSystem& fs;
};
