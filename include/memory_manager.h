#pragma once

#include <vector>
#include <string>

struct MemoryBlock {
    int size;
    bool is_free;
    int start_address;
    MemoryBlock* next;

    MemoryBlock(int size, bool is_free, int start_address) 
        : size(size), is_free(is_free), start_address(start_address), next(nullptr) {}
};

class MemoryManager {
public:
    MemoryManager(int total_size);
    ~MemoryManager();

    int mem_alloc(int size); // returns start address or -1 if failed
    bool mem_free(int address);
    void memory_map() const;

    int get_total_size() const;
    int get_used_size() const;

private:
    int total_memory;
    MemoryBlock* head;

    void merge_free_blocks();
};
