cmake_minimum_required(VERSION 3.16)

# Project name
project(DSII_Project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Include directories
include_directories(include)

# Add executable
add_executable(DSII_Project ${SOURCES})

# Link libraries (if any)
# target_link_libraries(DSII_Project <library_name>)