//Used for displaying the files and directories in the current directory

#include "fs.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

/**
 * @brief Displays the contents of a directory recursively
 * 
 * This function traverses through a directory and its subdirectories,
 * displaying all files and folders it finds. It handles errors gracefully
 * and skips system directories and files that should not be accessed.
 * 
 * @param directory The path to the directory to display
 */
void fsDisplay(const std::string& directory) {
    try {
        if (!fs::exists(directory)) {
            std::cerr << "Error: The path '" << directory << "' does not exist.\n";
            return;
        }

        std::cout << "Displaying contents of: " << directory << "\n\n";
        
        if (!fs::is_directory(directory)) {
            std::cout << "[FILE] " << fs::absolute(directory).string() << "\n";
            return;
        }

        std::stack<fs::path> directoryStack;
        directoryStack.push(fs::path(directory));
        int itemCount = 0;

        while (!directoryStack.empty()) {
            auto currentDir = directoryStack.top();
            directoryStack.pop();

            try {
                if (shouldSkipPath(currentDir.string())) {
                    continue;
                }

                std::cout << "\n[DIR] " << currentDir.string() << "\n";
                const auto dirOptions = fs::directory_options::skip_permission_denied;
                std::error_code errorCode;
                
                for (const auto& entry : fs::directory_iterator(currentDir, dirOptions, errorCode)) {
                    try {
                        const auto& entryPath = entry.path();
                        
                        if (shouldSkipPath(entryPath.string())) {
                            continue;
                        }

                        // Indent subdirectory contents for better readability
                        std::cout << "  " << (fs::is_directory(entryPath, errorCode) ? "[DIR] " : "[FILE] ")
                                 << entryPath.filename().string() << "\n";
                        itemCount++;

                        if (fs::is_directory(entryPath, errorCode)) {
                            directoryStack.push(entryPath);
                        }
                    } catch (...) {
                        continue;
                    }
                }

                if (errorCode) {
                    std::cerr << "Warning: Some entries in " << currentDir << " could not be accessed\n";
                }

            } catch (...) {
                continue;
            }
        }
        
        std::cout << "\nTotal items found: " << itemCount << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error during directory display: " << e.what() << "\n";
    }
}
