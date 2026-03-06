#include "scheduler.h"
#include "logger.h"
#include <iostream>
#include <algorithm>

Scheduler::Scheduler(ProcessManager& pm) : process_manager(pm), running_process(nullptr) {}

void Scheduler::add_to_ready_queue(std::shared_ptr<Process> process) {
    if (process && process->state != ProcessState::TERMINATED) {
        process->state = ProcessState::READY;
        ready_queue.push_back(process);
        Logger::log("Process " + process->id + " added to Ready Queue.");
    }
}

void Scheduler::block_process(const std::string& id) {
    auto p = process_manager.get_process(id);
    if (!p) return;
    
    // Remove from ready queue if present
    auto it = std::remove(ready_queue.begin(), ready_queue.end(), p);
    if (it != ready_queue.end()) {
        ready_queue.erase(it, ready_queue.end());
    }

    if (running_process && running_process->id == id) {
        running_process = nullptr; // Prempt
    }

    p->state = ProcessState::WAITING;
    waiting_queue.push_back(p);
    Logger::log("Process " + id + " blocked. Transitioned to WAITING.");
}

void Scheduler::unblock_process(const std::string& id) {
    auto it = std::find_if(waiting_queue.begin(), waiting_queue.end(),
                           [&id](const std::shared_ptr<Process>& p) { return p->id == id; });
                           
    if (it != waiting_queue.end()) {
        auto p = *it;
        waiting_queue.erase(it);
        add_to_ready_queue(p);
        Logger::log("Process " + id + " unblocked. Transitioned to READY.");
    }
}

const std::deque<std::shared_ptr<Process>>& Scheduler::get_ready_queue() const { return ready_queue; }
const std::vector<std::shared_ptr<Process>>& Scheduler::get_waiting_queue() const { return waiting_queue; }
std::shared_ptr<Process> Scheduler::get_running_process() const { return running_process; }
SchedAlgo Scheduler::get_current_algo() const { return current_algo; }

void Scheduler::schedule(SchedAlgo algo, int time_quantum) {
    current_algo = algo;
    Logger::log("Starting scheduler...");

    switch (algo) {
        case SchedAlgo::FCFS: run_fcfs(); break;
        case SchedAlgo::SJF: run_sjf(); break;
        case SchedAlgo::RR: run_rr(time_quantum); break;
        case SchedAlgo::PRIORITY: run_priority(); break;
    }
    
    calculate_metrics();
}

void Scheduler::print_gantt_chart(const std::vector<std::string>& execution_order) {
    std::cout << "\nGantt Chart:\n|";
    for (const auto& id : execution_order) {
        std::cout << " " << id << " |";
    }
    std::cout << "\n\n";
}

void Scheduler::run_fcfs() {
    std::vector<std::string> order;
    int current_time = 0;

    std::queue<std::shared_ptr<Process>> rq;
    for (auto& p : ready_queue) rq.push(p);
    ready_queue.clear();

    while (!rq.empty()) {
        auto p = rq.front();
        rq.pop();
        
        running_process = p;
        p->state = ProcessState::RUNNING;
        Logger::log("Process " + p->id + " is RUNNING.");
        
        if (current_time < p->arrival_time) current_time = p->arrival_time;
        
        current_time += p->burst_time;
        p->completion_time = current_time;
        p->state = ProcessState::TERMINATED;
        terminated_list.push_back(p);
        
        order.push_back(p->id);
    }
    running_process = nullptr;
    print_gantt_chart(order);
}

void Scheduler::run_rr(int time_quantum) {
    std::vector<std::string> order;
    int current_time = 0;

    std::queue<std::shared_ptr<Process>> rq;
    for (auto& p : ready_queue) rq.push(p);
    ready_queue.clear();

    while (!rq.empty()) {
        auto p = rq.front();
        rq.pop();
        
        running_process = p;
        p->state = ProcessState::RUNNING;
        Logger::log("Process " + p->id + " is RUNNING in RR.");
        
        order.push_back(p->id);

        if (p->remaining_time > time_quantum) {
            current_time += time_quantum;
            p->remaining_time -= time_quantum;
            p->state = ProcessState::READY;
            rq.push(p); // Put back to the queue
        } else {
            current_time += p->remaining_time;
            p->remaining_time = 0;
            p->completion_time = current_time;
            p->state = ProcessState::TERMINATED;
            terminated_list.push_back(p);
        }
    }
    running_process = nullptr;
    print_gantt_chart(order);
}

void Scheduler::run_sjf() {
    std::vector<std::string> order;
    int current_time = 0;

    std::vector<std::shared_ptr<Process>> sorted_rq(ready_queue.begin(), ready_queue.end());
    ready_queue.clear();
    
    // Sort logic simplified: by burst time only (non-preemptive)
    std::sort(sorted_rq.begin(), sorted_rq.end(), [](const std::shared_ptr<Process>& a, const std::shared_ptr<Process>& b) {
        return a->burst_time < b->burst_time;
    });

    for (auto& p : sorted_rq) {
        running_process = p;
        p->state = ProcessState::RUNNING;
        
        if (current_time < p->arrival_time) current_time = p->arrival_time;
        current_time += p->burst_time;
        p->completion_time = current_time;
        p->state = ProcessState::TERMINATED;
        terminated_list.push_back(p);
        order.push_back(p->id);
    }
    running_process = nullptr;
    print_gantt_chart(order);
}

void Scheduler::run_priority() {
    std::vector<std::string> order;
    int current_time = 0;

    std::vector<std::shared_ptr<Process>> sorted_rq(ready_queue.begin(), ready_queue.end());
    ready_queue.clear();
    
    // Sort logic: by priority (lower number == higher priority usually)
    std::sort(sorted_rq.begin(), sorted_rq.end(), [](const std::shared_ptr<Process>& a, const std::shared_ptr<Process>& b) {
        return a->priority < b->priority;
    });

    for (auto& p : sorted_rq) {
        running_process = p;
        p->state = ProcessState::RUNNING;
        
        if (current_time < p->arrival_time) current_time = p->arrival_time;
        current_time += p->burst_time;
        p->completion_time = current_time;
        p->state = ProcessState::TERMINATED;
        terminated_list.push_back(p);
        order.push_back(p->id);
    }
    running_process = nullptr;
    print_gantt_chart(order);
}

void Scheduler::calculate_metrics() {
    if (terminated_list.empty()) return;
    double total_wt = 0, total_tat = 0;
    
    for (auto& p : terminated_list) {
        p->turnaround_time = p->completion_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        total_wt += p->waiting_time;
        total_tat += p->turnaround_time;
    }
    
    std::cout << "Avg Waiting Time: " << (total_wt / terminated_list.size()) << "\n";
    std::cout << "Avg Turnaround Time: " << (total_tat / terminated_list.size()) << "\n\n";
}
