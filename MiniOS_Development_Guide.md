# MiniOS Simulator - Development Diary & Architecture Guide

This document serves as a comprehensive guide to understanding, building, and presenting the **MiniOS Simulator**. It details how the project is structured, how each component works internally, how to use it, and the unique challenges faced during development.

---

## 1. Project Overview & Architecture

The MiniOS Simulator is designed to behave like a real operating system kernel. Instead of just running algorithms dynamically and terminating, it acts as a continuous running environment (a REPL shell) where memory, files, and processes have real simulated lifecycles.

### The Architecture

The system is split into modular components that represent actual OS kernel modules:

```text
                 +------------------+
                 |      Shell       |
                 |  Command Parser  |
                 +--------+---------+
                          |
        ----------------------------------------
        |            |            |            |
+--------------+ +------------+ +------------+ +-------------+
| Process      | | Scheduler  | | Memory     | | File System |
| Manager      | | (FCFS, RR, | | Manager    | | Simulator   |
| (State Mgt)  | |  SJF, Pri) | | (Allocator)| | (Disk I/O)  |
+--------------+ +------------+ +------------+ +-------------+
        |              |             |              |
        -----------------------------------------------------
                              |
+----------------+    +----------------+
| Kernel Config  |    | System Monitor |
| Loader         |    | (Dashboard)    |
+----------------+    +----------------+
```

---

## 2. How Each Component Works

### A. Kernel Configuration (`kernel_config.cpp`)
**What it does:** Real OS kernels don’t hardcode values; they load them at boot.
**How it works:** When MiniOS starts, it parses `config/kernel.conf` to set the default Scheduler algorithm, the Round Robin Time Quantum, and the Total Memory size for the allocator.

### B. Process Manager (`process_manager.cpp`)
**What it does:** Handles the lifecycle of processes.
**How it works:** Processes aren't just strings; they are objects with states: `NEW`, `READY`, `RUNNING`, `WAITING`, `TERMINATED`. When you type `create_process`, it adds a new Process object to the global Process Table.

### C. CPU Scheduler (`scheduler.cpp`)
**What it does:** Decides which process gets the CPU.
**How it works:** It maintains two critical queues:
- **Ready Queue:** Processes waiting for CPU time.
- **Waiting Queue:** Processes blocked (e.g., waiting for I/O).

When you run `schedule rr`, the scheduler loads processes from the `Ready Queue`, simulates execution by deducting their `burst_time`, and computes turnaround/waiting times.

### D. Memory Manager (`memory_manager.cpp`)
**What it does:** Simulates RAM allocation.
**How it works:** It uses a Linked List to represent continuous memory bytes. It uses the **First-Fit Algorithm**. When space is requested (`mem_alloc 128`), it finds the first free block large enough, splits the block into 'USED' and 'FREE' chunks, and updates the list. If blocks are freed (`mem_free`), it attempts to merge adjacent free nodes to avoid fragmentation.

### E. File System (`filesystem.cpp`)
**What it does:** Simulates a virtual disk.
**How it works:** Uses a C++ `std::map<std::string, File>` to store virtual files in memory. You can create, read, write, and delete files, perfectly simulating the interactions of a command-line OS environment.

### F. Shell (`shell.cpp`)
**What it does:** Gives the user an interactive command-line interface.
**How it works:** It loops infinitely (`while(true)`), reading strings from standard input (`std::cin`), parsing the first word as a command, and then delegating the task to the respective Manager classes. 

---

## 3. How to Test & Use The Simulator

You can test the robustness of the system using a specific flow.

### Boot the System
Run the compiled executable:
```bash
.\MiniOS.exe
```

### 1. Check Configuration
```text
MiniOS> kernel_config
```

### 2. Simulate Process Lifecycles
```text
MiniOS> create_process P1 5 1
MiniOS> create_process P2 3 2
MiniOS> create_process P3 10 3
```
*Note exactly how they start in NEW and change to READY.*

Check the process layout:
```text
MiniOS> list_processes
```

### 3. Run CPU Algorithms
Let's see Round Robin process scheduling.
```text
MiniOS> schedule rr
```
*This will output a visual Gantt Chart and metrics.*

### 4. Block a Process
Wait, P3 needs to pause for an I/O event:
```text
MiniOS> block_process P3
```
Let's check the system to ensure it moved to the `Waiting Queue`:
```text
MiniOS> system_status
```
Unblock it so it can run again later:
```text
MiniOS> unblock_process P3
```

### 5. Memory Allocation
```text
MiniOS> mem_alloc 128
MiniOS> mem_alloc 64
MiniOS> memory_map
```
*You will see the visual representation of fragmented bytes.*

---

## 4. Problems Faced During Development

### Challenge 1: Compiling with Older C++ Compilers (MinGW/GCC)
**The Problem:** Originally, the `filesystem.cpp` was written using C++17 structured bindings (e.g., `for (const auto& [name, file] : files)`). However, the Windows `g++` compiler available (MinGW version 6.3.0) only fully supports C++14/C++11. CMake also threw errors in PowerShell.
**The Fix:** We had to pivot. Instead of relying on CMake generation, we manually invoked `g++ --std=c++1y src\*.cpp -Iinclude -o MiniOS.exe`. We also had to refactor the C++17 structured bindings down to C++11 standard pair iterations (`pair.first`, `pair.second`).

### Challenge 2: Re-running Schedulers on Empty Queues
**The Problem:** When running a scheduler (`schedule rr`), it consumes processes from the `Ready Queue`. If you try to run `schedule sjf` right after, it prints an empty Gantt chart (`|`) because those processes have transitioned to `TERMINATED`!
**The Context:** This actually isn't a bug—it’s how a real OS works. A terminated process cannot be scheduled again unless it is recreated. This demonstrated the accuracy of the State Machine.

### Challenge 3: Windows PowerShell Execution Parsing
**The Problem:** Writing multi-line configurations dynamically to `kernel.conf` using UNIX-style HereDocs (`cat << EOF > file`) failed because PowerShell handles IO redirection differently than Bash.
**The Fix:** We pivoted our automation specifically to use system-native File writers to generate the configuration natively instead of through standard bash piping.

---

## 5. Why this project is impressive
Most student OS projects are single-run algorithms. They take input, print output, and exit. 

This project simulates **state**. It implements custom Continuous Memory Allocators (Linked Lists), Queue management (Deque structs), Configuration bootstrappers, Singleton Loggers, and functional REPL environments. It demonstrates an understanding of **Software Architecture**, **State Machines**, and **Low-Level Memory Concepts**.
