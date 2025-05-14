#include "fs.h"
#include <iostream>
#include <string>
#include <limits>
#include <filesystem>

namespace fs = std::filesystem;

/**
 * @brief Displays the command prompt with current directory
 * 
 * Shows the current working directory in the prompt, abbreviated if too long.
 * The last directory name is always shown in full, while the path may be truncated.
 */
void displayPrompt() {
    std::string currentDir = getCurrentDirectory();
    fs::path dirPath(currentDir);
    
    // Get just the last directory name
    std::string lastDir = dirPath.filename().string();
    
    // If the path is too long, abbreviate it
    if (currentDir.length() > 40) {
        std::string abbreviated = "..." + currentDir.substr(currentDir.length() - 37);
        std::cout << abbreviated << "> ";
    } else {
        std::cout << currentDir << "> ";
    }
}

/**
 * @brief Displays help information about available commands
 */
void displayHelp() {
    std::cerr << "Available commands:\n"
              << "  search <directory>     - Search for files/directories by name\n"
              << "  display <directory>    - Show contents of directory\n"
              << "  cd [directory]         - Change directory (cd alone goes to home)\n"
              << "  mkdir <directory>      - Create a new directory\n"
              << "  touch <file>          - Create a new empty file\n"
              << "  rm <path>             - Delete a file or directory\n"
              << "  mv <old> <new>        - Rename or move a file or directory\n"
              << "  help                  - Show this help message\n"
              << "  exit/quit             - Exit the program\n\n"
              << "Notes:\n"
              << "  - Paths can be absolute or relative to current directory\n"
              << "  - Use quotes for paths containing spaces\n"
              << "  - Use ~ for home directory, .. for parent directory\n";
}

/**
 * @brief Main entry point for the file system explorer
 * 
 * This program provides a command-line interface for exploring and managing
 * the file system. It supports navigation, searching, and basic file operations.
 * 
 * @return int Exit code (0 for success, 1 for error)
 */
int main() {
    std::string command;
    std::string arg1, arg2;
    
    while (true) {  // Main program loop
        try {
            displayPrompt();
            std::cin >> command;
            
            // Check for exit commands first
            if (command == "exit" || command == "quit") {
                std::cout << "Goodbye!\n";
                break;
            }

            // Handle help command
            if (command == "help") {
                displayHelp();
                continue;
            }
            
            // Handle cd command differently as it might have empty directory
            if (command == "cd") {
                std::string cdPath;
                std::getline(std::cin >> std::ws, cdPath);
                
                // If no path specified, go to home directory
                if (cdPath.empty()) {
                    fsCd("~");
                } else {
                    fsCd(cdPath);
                }
                continue;
            }

            // Handle mv command which needs two arguments
            if (command == "mv") {
                std::getline(std::cin >> std::ws, arg1, ' ');
                std::getline(std::cin >> std::ws, arg2);
                
                if (arg1.empty() || arg2.empty()) {
                    std::cerr << "Error: mv command requires two arguments: <old_path> <new_path>\n";
                    continue;
                }
                
                fsRename(arg1, arg2);
                continue;
            }
            
            // Handle other commands that take one argument
            std::getline(std::cin >> std::ws, arg1);

            if (std::cin.fail()) {
                std::cerr << "Error: Failed to read input\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (command == "search") {
                fsSearch(arg1);
            } else if (command == "display") {
                fsDisplay(arg1);
            } else if (command == "mkdir") {
                fsCreate(arg1, true);
            } else if (command == "touch") {
                fsCreate(arg1, false);
            } else if (command == "rm") {
                fsDelete(arg1);
            } else {
                displayHelp();
            }

        } catch(const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
        
        // Add a visual separator between commands
        std::cout << "\n";
    }
    
    return 0;
}