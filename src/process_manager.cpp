#include "process_manager.h"
#include "logger.h"
#include <iostream>

Process::Process(std::string id, int arrival_time, int burst_time, int priority)
    : id(std::move(id)), arrival_time(arrival_time), burst_time(burst_time), 
      remaining_time(burst_time), priority(priority), state(ProcessState::NEW),
      waiting_time(0), turnaround_time(0), completion_time(0) {}

std::string state_to_string(ProcessState state) {
    switch (state) {
        case ProcessState::NEW: return "NEW";
        case ProcessState::READY: return "READY";
        case ProcessState::RUNNING: return "RUNNING";
        case ProcessState::WAITING: return "WAITING";
        case ProcessState::TERMINATED: return "TERMINATED";
        default: return "UNKNOWN";
    }
}

std::shared_ptr<Process> ProcessManager::create_process(const std::string& id, int burst_time, int priority) {
    auto process = std::make_shared<Process>(id, current_time_tick, burst_time, priority);
    process_table.push_back(process);
    
    Logger::log("Process " + id + " created in NEW state. Transitions to READY.");
    process->state = ProcessState::READY;
    
    return process;
}

bool ProcessManager::kill_process(const std::string& id) {
    for (auto it = process_table.begin(); it != process_table.end(); ++it) {
        if ((*it)->id == id) {
            (*it)->state = ProcessState::TERMINATED;
            Logger::log("Process " + id + " killed. Transitioned to TERMINATED.");
            return true;
        }
    }
    return false;
}

const std::vector<std::shared_ptr<Process>>& ProcessManager::get_process_table() const {
    return process_table;
}

std::shared_ptr<Process> ProcessManager::get_process(const std::string& id) {
    for (const auto& p : process_table) {
        if (p->id == id) {
            return p;
        }
    }
    return nullptr;
}
