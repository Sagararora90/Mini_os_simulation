#include "kernel_config.h"
#include "logger.h"
#include "process_manager.h"
#include "scheduler.h"
#include "memory_manager.h"
#include "filesystem.h"
#include "system_monitor.h"
#include "shell.h"
#include <iostream>

int main() {
    std::cout << "Loading kernel configuration...\n";
    KernelConfig config;
    config.load_from_file("config/kernel.conf");
    config.print_config();

    Logger::init("data/os.log");
    Logger::log("MiniOS Kernel Booting...");

    ProcessManager pm;
    Scheduler sched(pm);
    MemoryManager mm(config.total_memory);  // configure via config!
    FileSystem fs;
    SystemMonitor sm(pm, sched, mm, fs);
    
    // Pass config info to Shell? We can modify shell to accept Config.
    Shell shell(pm, sched, mm, fs, sm);
    
    std::cout << "MiniOS started successfully.\n";
    shell.run();

    Logger::log("MiniOS Shutdown Complete.");
    Logger::close();

    return 0;
}
