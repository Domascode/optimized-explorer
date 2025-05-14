cmake_minimum_required(VERSION 4.0.1)
project(optimized_explorer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/fs_search.cpp
    src/fs_display.cpp
    src/shared.cpp
    src/fs_cd.cpp
    src/fs_manage.cpp
)

# Create executable
add_executable(optimized_explorer ${SOURCES})

# Link std::filesystem (required on some compilers)
target_link_libraries(optimized_explorer PRIVATE stdc++fs)