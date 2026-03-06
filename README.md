# MiniOS – Operating System Simulator

An interactive, modular simulation of core Operating System components written in modern C++17. MiniOS provides a command-line interface (Shell) to interact with simulated OS subsystems, giving you hands-on experience with how kernels manage computing resources.

## Architecture

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
                      +----------------+
                      | System Monitor |
                      +----------------+
```

## Demo

*(Add your asciinema or screen2gif demo here!)*
![MiniOS Demo](demo.gif)

### Example Interactions

```text
MiniOS> create_process P1 5 1
Process P1 created in NEW state. Transitions to READY.

MiniOS> create_process P2 3 2
Process P2 created in NEW state. Transitions to READY.

MiniOS> schedule rr

Gantt Chart (Round Robin):
| P1 | P2 | P1 |

MiniOS> memory_map

[USED 128KB] [FREE 256KB] [USED 64KB]
```

## Features

- **Process Management**: Lifecycle simulation with realistic state transitions (`NEW` → `READY` → `RUNNING` → `WAITING` → `TERMINATED`).
- **CPU Scheduling Algorithms**: Supports First Come First Serve (FCFS), Shortest Job First (SJF), Round Robin (RR), and Priority Scheduling.
- **Queue-Based Execution**: Employs `ready_queue`, `waiting_queue`, and process tables just like a real kernel.
- **Custom Memory Allocator**: Simulates contiguous memory allocation with metadata, block splitting, merging, and fragmentation tracking.
- **Simulated File System**: Operations to create, read, write, and delete files on a simulated disk.
- **System Monitor**: Real-time snapshots of queues, process tables, memory usage, and file states.
- **Interactive Command Shell**: Parse real-time user commands to interact with the simulated OS.

## Supported Commands

| Command | Arguments | Description |
|-------|---------|--------|
| `create_process` | `<id> <burst_time> <priority>` | Create a new process. |
| `kill_process` | `<id>` | Terminate a running or ready process. |
| `list_processes` | *(none)* | Show all processes and their current states. |
| `block_process` | `<id>` | Move a process from RUNNING/READY to WAITING (simulate IO). |
| `unblock_process`| `<id>` | Move a process from WAITING to READY. |
| `schedule` | `<fcfs|sjf|rr|priority>` | Run the CPU scheduler and display Gantt chart. |
| `mem_alloc` | `<size>` | Allocate a block of memory (in KB). |
| `mem_free` | `<address>` | Free a memory block by its starting address. |
| `memory_map` | *(none)* | Display visualizing blocks of used and free memory. |
| `create` | `<filename>` | Create a new file in the simulated disk. |
| `write` | `<filename> <content>` | Write data to a file. |
| `read` | `<filename>` | Read data from a file. |
| `delete` | `<filename>` | Delete a file from the disk. |
| `list_files` | *(none)* | List all files in the file system. |
| `system_status`| *(none)* | Show system dashboard (Queues, Memory, Process Table). |
| `help` | *(none)* | Display available commands. |
| `exit` | *(none)* | Shut down MiniOS. |

## System Components

1. **Shell**: Parses CLI commands and invokes sub-system managers.
2. **Process Manager**: Handles process creation, maintains the process table, and triggers state transitions.
3. **Scheduler**: Manages `ready_queue` and `waiting_queue`; selects processes to dispatch to the CPU.
4. **Memory Manager**: Maintains a linked-list-based custom allocator tracking size, start addresses, and free/used status.
5. **File System**: Simulates basic file lifecycle and storage.
6. **System Monitor**: Aggregates state and presents dashboard outputs.
7. **Logger**: Dumps critical system events (timestamped) to OS logs.

## How to Build

```bash
# Provide a clean build directory
mkdir build
cd build

# Generate build configuration
cmake ..

# Compile project
cmake --build .
```

## Run

```bash
# From within the build directory
./MiniOS
```
