cmake_minimum_required(VERSION 3.15)
project(MiniOS VERSION 1.0 DESCRIPTION "Operating System Simulator" LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Include Directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Gather source files
file(GLOB SOURCES "src/*.cpp")

# Define executable
add_executable(MiniOS ${SOURCES})
