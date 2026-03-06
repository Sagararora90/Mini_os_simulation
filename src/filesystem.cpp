#include "filesystem.h"
#include "logger.h"
#include <iostream>

FileSystem::FileSystem() {
    // In a real simulation, we might load disk.img here
    Logger::log("FileSystem initialized.");
}

bool FileSystem::create_file(const std::string& filename) {
    if (files.find(filename) != files.end()) {
        Logger::log("Failed to create file: " + filename + " (Already exists)");
        return false;
    }
    files[filename] = {filename, "", 0};
    Logger::log("Created file: " + filename);
    return true;
}

bool FileSystem::write_file(const std::string& filename, const std::string& data) {
    auto it = files.find(filename);
    if (it == files.end()) {
        Logger::log("Failed to write to file: " + filename + " (Not found)");
        return false;
    }
    it->second.content = data;
    it->second.size = data.size();
    Logger::log("Written to file: " + filename + " (" + std::to_string(data.size()) + " bytes)");
    return true;
}

std::string FileSystem::read_file(const std::string& filename) {
    auto it = files.find(filename);
    if (it == files.end()) {
        Logger::log("Failed to read file: " + filename + " (Not found)");
        return "";
    }
    Logger::log("Read from file: " + filename);
    return it->second.content;
}

bool FileSystem::delete_file(const std::string& filename) {
    if (files.erase(filename) > 0) {
        Logger::log("Deleted file: " + filename);
        return true;
    }
    Logger::log("Failed to delete file: " + filename + " (Not found)");
    return false;
}

void FileSystem::list_files() const {
    std::cout << "\nFiles in FileSystem:\n";
    if (files.empty()) {
        std::cout << " (No files found)\n";
    }
    for (const auto& pair : files) {
        std::cout << " - " << pair.first << " (" << pair.second.size << " bytes)\n";
    }
    std::cout << "\n";
}

int FileSystem::get_total_files() const {
    return files.size();
}
