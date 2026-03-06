#pragma once

#include <string>
#include <vector>
#include <memory>

enum class ProcessState {
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
};

struct Process {
    std::string id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    ProcessState state;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    
    Process(std::string id, int arrival_time, int burst_time, int priority);
};

class ProcessManager {
public:
    ProcessManager() = default;

    std::shared_ptr<Process> create_process(const std::string& id, int burst_time, int priority);
    bool kill_process(const std::string& id);
    const std::vector<std::shared_ptr<Process>>& get_process_table() const;
    std::shared_ptr<Process> get_process(const std::string& id);

private:
    std::vector<std::shared_ptr<Process>> process_table;
    int current_time_tick = 0;
};

std::string state_to_string(ProcessState state);
