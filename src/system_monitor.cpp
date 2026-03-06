#include "system_monitor.h"
#include <iostream>
#include <iomanip>

SystemMonitor::SystemMonitor(ProcessManager& pm, Scheduler& sched, MemoryManager& mm, FileSystem& fs)
    : pm(pm), sched(sched), mm(mm), fs(fs) {}

void SystemMonitor::display_system_status() const {
    std::cout << "\n========================================\n";
    std::cout << "          MiniOS SYSTEM STATUS          \n";
    std::cout << "========================================\n";

    // --- Scheduler & Queues ---
    std::string algo;
    switch (sched.get_current_algo()) {
        case SchedAlgo::FCFS: algo = "First Come First Serve"; break;
        case SchedAlgo::SJF: algo = "Shortest Job First"; break;
        case SchedAlgo::RR: algo = "Round Robin"; break;
        case SchedAlgo::PRIORITY: algo = "Priority"; break;
    }
    std::cout << "CPU Scheduler    : " << algo << "\n";
    
    auto rp = sched.get_running_process();
    std::cout << "Running Process  : " << (rp ? rp->id : "IDLE") << "\n\n";

    std::cout << "Ready Queue      : ";
    auto& rq = sched.get_ready_queue();
    if (rq.empty()) std::cout << "(empty)";
    for (const auto& p : rq) std::cout << p->id << " ";
    std::cout << "\n";

    std::cout << "Waiting Queue    : ";
    auto& wq = sched.get_waiting_queue();
    if (wq.empty()) std::cout << "(empty)";
    for (const auto& p : wq) std::cout << p->id << " ";
    std::cout << "\n\n";

    // --- Memory Manager ---
    std::cout << "Memory Usage\n";
    std::cout << "Total   : " << mm.get_total_size() << " KB\n";
    std::cout << "Used    : " << mm.get_used_size() << " KB\n";
    std::cout << "Free    : " << (mm.get_total_size() - mm.get_used_size()) << " KB\n\n";

    // --- File System ---
    std::cout << "File System Summary\n";
    std::cout << "Total Files : " << fs.get_total_files() << "\n\n";

    // --- Process Table ---
    std::cout << "Process Table\n";
    std::cout << "---------------------------------------\n";
    std::cout << std::left << std::setw(6) << "PID" 
              << std::setw(12) << "STATE" 
              << std::setw(8) << "BURST"
              << std::setw(8) << "PRIORITY" << "\n";
              
    for (const auto& p : pm.get_process_table()) {
        std::cout << std::left << std::setw(6) << p->id 
                  << std::setw(12) << state_to_string(p->state) 
                  << std::setw(8) << p->burst_time 
                  << std::setw(8) << p->priority << "\n";
    }
    std::cout << "========================================\n\n";
}
