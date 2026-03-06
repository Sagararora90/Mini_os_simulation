#pragma once

#include <string>
#include <map>
#include <vector>

struct File {
    std::string name;
    std::string content;
    size_t size;
};

class FileSystem {
public:
    FileSystem();

    bool create_file(const std::string& filename);
    bool write_file(const std::string& filename, const std::string& data);
    std::string read_file(const std::string& filename);
    bool delete_file(const std::string& filename);
    void list_files() const;
    
    int get_total_files() const;

private:
    std::map<std::string, File> files;
    // In a real simulator, we could map to a local disk.img file,
    // but building an in-memory map acts as our virtual disk simulation for now.
    void save_to_disk();
    void load_from_disk();
};
