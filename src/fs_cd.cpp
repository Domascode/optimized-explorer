#include "fs.h"
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

/**
 * @brief Current working directory for the file explorer
 * 
 * This variable maintains the state of the current working directory
 * across function calls. It's initialized to the program's starting directory.
 */
static fs::path currentWorkingDirectory = fs::current_path();

std::string getCurrentDirectory() {
    return currentWorkingDirectory.string();
}

bool fsCd(const std::string& directory) {
    try {
        fs::path newPath;
        
        // Handle special directory symbols
        if (directory == "..") {
            // Move up one directory
            newPath = currentWorkingDirectory.parent_path();
        } else if (directory == ".") {
            // Stay in current directory
            return true;
        } else if (directory == "~") {
            // Go to user's home directory
            const char* homeDir = std::getenv("USERPROFILE"); // Windows
            if (!homeDir) {
                homeDir = std::getenv("HOME"); // Unix-like systems
            }
            if (!homeDir) {
                std::cerr << "Error: Could not determine home directory\n";
                return false;
            }
            newPath = fs::path(homeDir);
        } else {
            // Handle relative and absolute paths
            fs::path inputPath(directory);
            if (inputPath.is_absolute()) {
                newPath = inputPath;
            } else {
                newPath = currentWorkingDirectory / inputPath;
            }
        }

        // Verify the new path exists and is a directory
        if (!fs::exists(newPath)) {
            std::cerr << "Error: Path '" << newPath.string() << "' does not exist\n";
            return false;
        }

        if (!fs::is_directory(newPath)) {
            std::cerr << "Error: Path '" << newPath.string() << "' is not a directory\n";
            return false;
        }

        // Try to get the canonical (normalized) path
        fs::path canonicalPath = fs::canonical(newPath);
        
        // Check if we have permission to access the directory
        std::error_code ec;
        fs::directory_iterator testAccess(canonicalPath, ec);
        if (ec) {
            std::cerr << "Error: Cannot access directory '" << canonicalPath.string() 
                     << "': Permission denied\n";
            return false;
        }

        // Update current working directory
        currentWorkingDirectory = canonicalPath;
        std::cout << "Changed directory to: " << currentWorkingDirectory.string() << "\n";
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error changing directory: " << e.what() << "\n";
        return false;
    }
}
