#include "shell.h"
#include "kernel_config.h"
#include "logger.h"
#include <iostream>
#include <sstream>

Shell::Shell(ProcessManager& pm, Scheduler& sched, MemoryManager& mm, FileSystem& fs, SystemMonitor& sm)
    : pm(pm), sched(sched), mm(mm), fs(fs), sm(sm) {}

void Shell::run() {
    std::string input;
    std::cout << "\nMiniOS Kernel initialized. Type 'help' for commands.\n";
    
    while (true) {
        std::cout << "MiniOS> ";
        std::getline(std::cin, input);
        if (input.empty()) continue;
        
        std::istringstream iss(input);
        std::string cmd;
        iss >> cmd;

        if (cmd == "exit") {
            Logger::log("System shutting down...");
            break;
        }
        parse_command(input);
    }
}

void Shell::parse_command(const std::string& command_line) {
    std::istringstream iss(command_line);
    std::string cmd;
    iss >> cmd;

    Logger::log("Shell command: " + command_line);

    if (cmd == "help") {
        print_help();
    } 
    else if (cmd == "kernel_config") {
        // Needs a reference to KernelConfig? We can fetch it or just re-read it but better pass it 
        // if we change shell to hold KernelConfig. Let's assume we can fetch it, 
        // or actually, the system config is loaded statically or held in main. 
        // For now let's just create a quick KernelConfig object and load it to view 
        // (in a real system we'd pass the actual running config in the Shell constructor).
        KernelConfig kc;
        kc.load_from_file("config/kernel.conf");
        kc.print_config();
    }
    else if (cmd == "create_process") {
        std::string id;
        int burst, prio;
        if (iss >> id >> burst >> prio) {
            auto proc = pm.create_process(id, burst, prio);
            sched.add_to_ready_queue(proc);
            std::cout << "Process " << id << " created in NEW state. Transitions to READY.\n";
        } else {
            std::cout << "Usage: create_process <id> <burst_time> <priority>\n";
        }
    }
    else if (cmd == "kill_process") {
        std::string id;
        if (iss >> id) {
            if (pm.kill_process(id)) std::cout << "Process " << id << " terminated.\n";
            else std::cout << "Process not found.\n";
        } else {
            std::cout << "Usage: kill_process <id>\n";
        }
    }
    else if (cmd == "block_process") {
        std::string id;
        if (iss >> id) {
            sched.block_process(id);
            std::cout << "Process " << id << " blocked (WAITING queue).\n";
        } else {
            std::cout << "Usage: block_process <id>\n";
        }
    }
    else if (cmd == "unblock_process") {
        std::string id;
        if (iss >> id) {
            sched.unblock_process(id);
            std::cout << "Process " << id << " unblocked (READY queue).\n";
        } else {
            std::cout << "Usage: unblock_process <id>\n";
        }
    }
    else if (cmd == "list_processes") {
        auto& pt = pm.get_process_table();
        std::cout << "PID\tSTATE\n";
        for (auto& p : pt) {
            std::cout << p->id << "\t" << state_to_string(p->state) << "\n";
        }
    }
    else if (cmd == "schedule") {
        std::string algo_str;
        if (iss >> algo_str) {
            SchedAlgo algo = SchedAlgo::FCFS;
            if (algo_str == "fcfs") algo = SchedAlgo::FCFS;
            else if (algo_str == "sjf") algo = SchedAlgo::SJF;
            else if (algo_str == "rr") algo = SchedAlgo::RR;
            else if (algo_str == "priority") algo = SchedAlgo::PRIORITY;
            else { std::cout << "Unknown algorithm.\n"; return; }
            
            sched.schedule(algo, 2); // default quantum 2, ideally fetched from config!
        } else {
            std::cout << "Usage: schedule <fcfs|sjf|rr|priority>\n";
        }
    }
    else if (cmd == "mem_alloc") {
        int size;
        if (iss >> size) {
            int addr = mm.mem_alloc(size);
            if (addr != -1) std::cout << "Allocated " << size << " KB at address " << addr << "\n";
            else std::cout << "Memory allocation failed.\n";
        } else { std::cout << "Usage: mem_alloc <size>\n"; }
    }
    else if (cmd == "mem_free") {
        int addr;
        if (iss >> addr) {
            if (mm.mem_free(addr)) std::cout << "Freed memory at address " << addr << "\n";
            else std::cout << "Failed to free memory.\n";
        } else { std::cout << "Usage: mem_free <address>\n"; }
    }
    else if (cmd == "memory_map") {
        mm.memory_map();
    }
    else if (cmd == "create") {
        std::string fname;
        if (iss >> fname) {
            if (fs.create_file(fname)) std::cout << "File created.\n";
        } else std::cout << "Usage: create <filename>\n";
    }
    else if (cmd == "write") {
        std::string fname, content;
        if (iss >> fname) {
            std::getline(iss, content);
            if (!content.empty() && content[0] == ' ') content = content.substr(1);
            if (fs.write_file(fname, content)) std::cout << "Data written.\n";
        } else std::cout << "Usage: write <filename> <content...>\n";
    }
    else if (cmd == "read") {
        std::string fname;
        if (iss >> fname) {
            std::string data = fs.read_file(fname);
            std::cout << "Content: " << data << "\n";
        } else std::cout << "Usage: read <filename>\n";
    }
    else if (cmd == "delete") {
        std::string fname;
        if (iss >> fname) {
            if (fs.delete_file(fname)) std::cout << "File deleted.\n";
        } else std::cout << "Usage: delete <filename>\n";
    }
    else if (cmd == "list_files") {
        fs.list_files();
    }
    else if (cmd == "system_status") {
        sm.display_system_status();
    }
    else {
        std::cout << "Unknown command: " << cmd << ". Type 'help'.\n";
    }
}

void Shell::print_help() {
    std::cout << "\nSupported Commands:\n"
              << "  create_process <id> <burst> <prio>\n"
              << "  kill_process <id>\n"
              << "  block_process <id>\n"
              << "  unblock_process <id>\n"
              << "  list_processes\n"
              << "  schedule <fcfs|sjf|rr|priority>\n"
              << "  mem_alloc <size>\n"
              << "  mem_free <address>\n"
              << "  memory_map\n"
              << "  create <filename>\n"
              << "  write <filename> <content>\n"
              << "  read <filename>\n"
              << "  delete <filename>\n"
              << "  list_files\n"
              << "  system_status\n"
              << "  kernel_config\n"
              << "  help\n"
              << "  exit\n\n";
}
