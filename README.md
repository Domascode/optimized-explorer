# Optimized File System Explorer

A fast and efficient command-line file system explorer written in C++. This tool provides a user-friendly interface for navigating and managing files and directories while maintaining robust error handling and system safety features.

## Features

- Interactive command-line interface with current directory display
- Case-insensitive file and directory search
- Recursive directory traversal and display
- Safe file system operations with error handling
- Support for both absolute and relative paths
- Special directory symbols support (~, ., ..)
- System directory protection
- Permission checking
- Automatic parent directory creation
- Path normalization

## Building

### Prerequisites

- C++17 compatible compiler
- CMake 4.0.1 or higher

### Build Steps

```bash
# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .
```

## Usage

Available commands:

- `search <directory>` - Search for files/directories by name
- `display <directory>` - Show contents of directory
- `cd [directory]` - Change directory (cd alone goes to home)
- `mkdir <directory>` - Create a new directory
- `touch <file>` - Create a new empty file
- `rm <path>` - Delete a file or directory
- `mv <old> <new>` - Rename or move a file or directory
- `help` - Show help message
- `exit/quit` - Exit the program

### Notes

- Paths can be absolute or relative to current directory
- Use quotes for paths containing spaces
- Use ~ for home directory, .. for parent directory

## License

This project is open source and available under the MIT License. 