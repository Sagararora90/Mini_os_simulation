#include "memory_manager.h"
#include "logger.h"
#include <iostream>

MemoryManager::MemoryManager(int total_size) : total_memory(total_size) {
    head = new MemoryBlock(total_size, true, 0);
    Logger::log("MemoryManager initialized with " + std::to_string(total_size) + " KB.");
}

MemoryManager::~MemoryManager() {
    MemoryBlock* current = head;
    while (current) {
        MemoryBlock* next = current->next;
        delete current;
        current = next;
    }
}

int MemoryManager::mem_alloc(int size) {
    MemoryBlock* current = head;
    
    // First fit algorithm
    while (current) {
        if (current->is_free && current->size >= size) {
            // Allocate 
            int alloc_address = current->start_address;
            
            // Split block if there is remaining space
            if (current->size > size) {
                MemoryBlock* new_block = new MemoryBlock(current->size - size, true, current->start_address + size);
                new_block->next = current->next;
                current->next = new_block;
            }
            
            current->size = size;
            current->is_free = false;
            Logger::log("Allocated " + std::to_string(size) + " KB at address " + std::to_string(alloc_address));
            return alloc_address;
        }
        current = current->next;
    }
    
    Logger::log("Failed to allocate " + std::to_string(size) + " KB. OOM / Fragmentation.");
    return -1;
}

bool MemoryManager::mem_free(int address) {
    MemoryBlock* current = head;
    while (current) {
        if (current->start_address == address && !current->is_free) {
            current->is_free = true;
            Logger::log("Freed memory at address " + std::to_string(address));
            merge_free_blocks(); // Attempt to merge adjacent free blocks
            return true;
        }
        current = current->next;
    }
    Logger::log("Failed to free memory at address " + std::to_string(address));
    return false;
}

void MemoryManager::merge_free_blocks() {
    MemoryBlock* current = head;
    while (current && current->next) {
        if (current->is_free && current->next->is_free) {
            current->size += current->next->size;
            MemoryBlock* temp = current->next;
            current->next = current->next->next;
            delete temp;
        } else {
            current = current->next;
        }
    }
}

void MemoryManager::memory_map() const {
    MemoryBlock* current = head;
    std::cout << "\nMemory Map (Total: " << total_memory << " KB):\n";
    while (current) {
        std::cout << "[" << (current->is_free ? "FREE " : "USED ") << current->size << "KB]";
        current = current->next;
    }
    std::cout << "\n\n";
}

int MemoryManager::get_total_size() const {
    return total_memory;
}

int MemoryManager::get_used_size() const {
    int used = 0;
    MemoryBlock* current = head;
    while(current){
        if(!current->is_free) used += current->size;
        current = current->next;
    }
    return used;
}
