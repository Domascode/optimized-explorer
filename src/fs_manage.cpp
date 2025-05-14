/**
 * @file fs_manage.cpp
 * @brief Implementation of file system management operations
 * 
 * This file contains implementations for creating, deleting, and renaming
 * files and directories. It provides safe operations with proper error
 * handling and supports both absolute and relative paths.
 */

#include "fs.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

/**
 * @brief Creates a new file or directory at the specified path
 * 
 * This function handles both file and directory creation with the following features:
 * - Supports both absolute and relative paths
 * - Creates parent directories automatically if needed
 * - Performs existence checks to prevent overwriting
 * - Provides detailed error messages
 * 
 * @param path The path where to create the file or directory
 * @param isDirectory true to create a directory, false to create a file
 * @return bool true if creation was successful, false if any error occurred
 */
bool fsCreate(const std::string& path, bool isDirectory) {
    try {
        fs::path targetPath;
        
        // Handle relative and absolute paths
        fs::path inputPath(path);
        if (inputPath.is_absolute()) {
            targetPath = inputPath;
        } else {
            targetPath = fs::path(getCurrentDirectory()) / inputPath;
        }

        // Check if path already exists
        if (fs::exists(targetPath)) {
            std::cerr << "Error: '" << targetPath.string() << "' already exists\n";
            return false;
        }

        // Create the file or directory
        if (isDirectory) {
            if (!fs::create_directories(targetPath)) {
                std::cerr << "Error: Failed to create directory '" << targetPath.string() << "'\n";
                return false;
            }
            std::cout << "Created directory: " << targetPath.string() << "\n";
        } else {
            // Create parent directories if they don't exist
            if (!targetPath.parent_path().empty()) {
                fs::create_directories(targetPath.parent_path());
            }
            
            std::ofstream file(targetPath);
            if (!file) {
                std::cerr << "Error: Failed to create file '" << targetPath.string() << "'\n";
                return false;
            }
            file.close();
            std::cout << "Created file: " << targetPath.string() << "\n";
        }
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error creating " << (isDirectory ? "directory" : "file") 
                 << ": " << e.what() << "\n";
        return false;
    }
}

/**
 * @brief Deletes a file or directory at the specified path
 * 
 * This function handles both file and directory deletion with the following features:
 * - Supports both absolute and relative paths
 * - Recursively deletes directories and their contents
 * - Prevents deletion of the current working directory
 * - Reports number of items deleted for directories
 * - Provides detailed error messages
 * 
 * @param path The path to the file or directory to delete
 * @return bool true if deletion was successful, false if any error occurred
 */
bool fsDelete(const std::string& path) {
    try {
        fs::path targetPath;
        
        // Handle relative and absolute paths
        fs::path inputPath(path);
        if (inputPath.is_absolute()) {
            targetPath = inputPath;
        } else {
            targetPath = fs::path(getCurrentDirectory()) / inputPath;
        }

        // Check if path exists
        if (!fs::exists(targetPath)) {
            std::cerr << "Error: '" << targetPath.string() << "' does not exist\n";
            return false;
        }

        // Get canonical path to check if we're trying to delete current directory
        fs::path canonicalPath = fs::canonical(targetPath);
        if (canonicalPath == fs::canonical(getCurrentDirectory())) {
            std::cerr << "Error: Cannot delete the current working directory\n";
            return false;
        }

        // Delete the file or directory
        std::error_code ec;
        bool isDir = fs::is_directory(targetPath);
        
        if (isDir) {
            uintmax_t itemCount = fs::remove_all(targetPath, ec);
            if (ec) {
                std::cerr << "Error: Failed to delete directory '" << targetPath.string() 
                         << "': " << ec.message() << "\n";
                return false;
            }
            std::cout << "Deleted directory and " << (itemCount - 1) 
                     << " contained items: " << targetPath.string() << "\n";
        } else {
            if (!fs::remove(targetPath, ec)) {
                std::cerr << "Error: Failed to delete file '" << targetPath.string() 
                         << "': " << ec.message() << "\n";
                return false;
            }
            std::cout << "Deleted file: " << targetPath.string() << "\n";
        }
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error deleting item: " << e.what() << "\n";
        return false;
    }
}

/**
 * @brief Renames or moves a file or directory
 * 
 * This function handles both renaming and moving of files and directories with the following features:
 * - Supports both absolute and relative paths for both source and destination
 * - Creates parent directories of the destination automatically if needed
 * - Prevents renaming of the current working directory
 * - Performs existence checks to prevent overwriting
 * - Provides detailed error messages
 * 
 * The operation will fail if:
 * - The source path doesn't exist
 * - The destination path already exists
 * - The source is the current working directory
 * - There are insufficient permissions
 * 
 * @param oldPath The current path of the file or directory
 * @param newPath The new path for the file or directory
 * @return bool true if rename was successful, false if any error occurred
 */
bool fsRename(const std::string& oldPath, const std::string& newPath) {
    try {
        fs::path oldTargetPath;
        fs::path newTargetPath;
        
        // Handle relative and absolute paths for old path
        fs::path oldInputPath(oldPath);
        if (oldInputPath.is_absolute()) {
            oldTargetPath = oldInputPath;
        } else {
            oldTargetPath = fs::path(getCurrentDirectory()) / oldInputPath;
        }

        // Handle relative and absolute paths for new path
        fs::path newInputPath(newPath);
        if (newInputPath.is_absolute()) {
            newTargetPath = newInputPath;
        } else {
            newTargetPath = fs::path(getCurrentDirectory()) / newInputPath;
        }

        // Check if source exists
        if (!fs::exists(oldTargetPath)) {
            std::cerr << "Error: '" << oldTargetPath.string() << "' does not exist\n";
            return false;
        }

        // Check if destination already exists
        if (fs::exists(newTargetPath)) {
            std::cerr << "Error: '" << newTargetPath.string() << "' already exists\n";
            return false;
        }

        // Get canonical path to check if we're trying to rename current directory
        fs::path canonicalPath = fs::canonical(oldTargetPath);
        if (canonicalPath == fs::canonical(getCurrentDirectory())) {
            std::cerr << "Error: Cannot rename the current working directory\n";
            return false;
        }

        // Create parent directories of the new path if they don't exist
        if (!newTargetPath.parent_path().empty()) {
            fs::create_directories(newTargetPath.parent_path());
        }

        // Perform the rename
        fs::rename(oldTargetPath, newTargetPath);
        std::cout << "Renamed '" << oldTargetPath.string() << "' to '" 
                 << newTargetPath.string() << "'\n";
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error renaming item: " << e.what() << "\n";
        return false;
    }
}
