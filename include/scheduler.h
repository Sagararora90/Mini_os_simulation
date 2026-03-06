#pragma once

#include "process_manager.h"
#include <vector>
#include <string>
#include <queue>
#include <memory>
#include <deque>

enum class SchedAlgo {
    FCFS,
    SJF,
    RR,
    PRIORITY
};

class Scheduler {
public:
    Scheduler(ProcessManager& pm);

    void add_to_ready_queue(std::shared_ptr<Process> process);
    void block_process(const std::string& id);
    void unblock_process(const std::string& id);
    
    void schedule(SchedAlgo algo, int time_quantum = 2);

    const std::deque<std::shared_ptr<Process>>& get_ready_queue() const;
    const std::vector<std::shared_ptr<Process>>& get_waiting_queue() const;
    std::shared_ptr<Process> get_running_process() const;
    SchedAlgo get_current_algo() const;

private:
    ProcessManager& process_manager;
    std::deque<std::shared_ptr<Process>> ready_queue;
    std::vector<std::shared_ptr<Process>> waiting_queue;
    std::vector<std::shared_ptr<Process>> terminated_list;
    std::shared_ptr<Process> running_process;
    SchedAlgo current_algo = SchedAlgo::FCFS;

    void run_fcfs();
    void run_sjf();
    void run_rr(int time_quantum);
    void run_priority();
    
    void print_gantt_chart(const std::vector<std::string>& execution_order);
    void calculate_metrics();
};
