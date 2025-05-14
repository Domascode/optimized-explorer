#include "fs.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

/**
 * @brief Checks if a filename matches the search term
 * 
 * Performs a case-insensitive substring search to determine if the filename
 * contains the search term.
 * 
 * @param fileName The name of the file to check
 * @param searchTerm The term to search for
 * @return true if the filename contains the search term, false otherwise
 */
bool matchesSearch(const std::string& fileName, const std::string& searchTerm) {
    std::string lowerFileName = fileName;
    std::string lowerSearchTerm = searchTerm;
    
    // Convert both strings to lowercase for case-insensitive comparison
    std::transform(lowerFileName.begin(), lowerFileName.end(), lowerFileName.begin(), ::tolower);
    std::transform(lowerSearchTerm.begin(), lowerSearchTerm.end(), lowerSearchTerm.begin(), ::tolower);
    
    return lowerFileName.find(lowerSearchTerm) != std::string::npos;
}

void fsSearch(const std::string& directory) {
    try {
        // Get search term from user
        std::cout << "Enter search term: ";
        std::string searchTerm;
        std::getline(std::cin, searchTerm);
        
        // Validate search term
        if (searchTerm.empty()) {
            std::cerr << "Error: Search term cannot be empty\n";
            return;
        }

        // Validate directory
        if (!fs::exists(directory)) {
            std::cerr << "Error: The path '" << directory << "' does not exist.\n";
            return;
        }

        std::cout << "Searching for '" << searchTerm << "' in: " << directory << "\n";
        
        // Handle single file case
        if (!fs::is_directory(directory)) {
            if (matchesSearch(fs::path(directory).filename().string(), searchTerm)) {
                std::cout << "[FILE] " << fs::absolute(directory).string() << "\n";
            }
            return;
        }

        // Initialize directory traversal
        std::stack<fs::path> directoryStack;
        directoryStack.push(fs::path(directory));
        int matchCount = 0;

        // Process directories in a depth-first manner
        while (!directoryStack.empty()) {
            auto currentDir = directoryStack.top();
            directoryStack.pop();

            try {
                // Skip system directories
                if (shouldSkipPath(currentDir.string())) {
                    continue;
                }

                // Configure directory iterator options
                const auto dirOptions = fs::directory_options::skip_permission_denied;
                std::error_code errorCode;
                
                // Iterate through directory contents
                for (const auto& entry : fs::directory_iterator(currentDir, dirOptions, errorCode)) {
                    try {
                        const auto& entryPath = entry.path();
                        
                        // Skip system files and directories
                        if (shouldSkipPath(entryPath.string())) {
                            continue;
                        }

                        // Check if current entry matches search criteria
                        if (matchesSearch(entryPath.filename().string(), searchTerm)) {
                            std::cout << (fs::is_directory(entryPath, errorCode) ? "[DIR] " : "[FILE] ")
                                     << entryPath.string() << "\n";
                            matchCount++;
                        }

                        // Add subdirectories to the stack for processing
                        if (fs::is_directory(entryPath, errorCode)) {
                            directoryStack.push(entryPath);
                        }
                    } catch (...) {
                        // Skip entries that cause errors
                        continue;
                    }
                }
            } catch (...) {
                // Skip directories that cause errors
                continue;
            }
        }
        
        // Display search results summary
        std::cout << "\nFound " << matchCount << " matches for '" << searchTerm << "'\n";
    } catch (const std::exception& e) {
        std::cerr << "Error during search: " << e.what() << "\n";
    }
}
