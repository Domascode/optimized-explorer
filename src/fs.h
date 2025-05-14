#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

/**
 * @file fs.h
 * @brief File system exploration and search functionality
 * 
 * This header provides declarations for functions that allow exploring
 * and searching the file system. It includes functionality for recursive
 * directory traversal, file searching, content display, and file management.
 */

/**
 * @brief Checks if a path should be skipped during directory traversal
 * 
 * This function determines whether a given path should be excluded from processing.
 * It checks against a list of system directories and files that should not be accessed.
 * 
 * @param path The filesystem path to check
 * @return true if the path should be skipped, false otherwise
 */
bool shouldSkipPath(const std::string& path);

/**
 * @brief Searches for files and directories by name
 * 
 * This function performs a recursive search through the specified directory,
 * looking for files and directories whose names contain the search term.
 * The search is case-insensitive and handles errors gracefully.
 * 
 * @param directory The path to start the search from
 */
void fsSearch(const std::string& directory);

/**
 * @brief Displays the contents of a directory recursively
 * 
 * This function traverses through a directory and its subdirectories,
 * displaying all files and folders it finds. It handles errors gracefully
 * and skips system directories and files that should not be accessed.
 * 
 * @param directory The path to the directory to display
 */
void fsDisplay(const std::string& directory);

/**
 * @brief Changes the current working directory
 * 
 * This function changes the current working directory to the specified path.
 * It supports both absolute and relative paths, as well as special directory
 * symbols like "." (current directory), ".." (parent directory), and "~" (home directory).
 * 
 * @param directory The path to change to
 * @return true if the directory was changed successfully, false otherwise
 */
bool fsCd(const std::string& directory);

/**
 * @brief Gets the current working directory
 * 
 * Returns the full path of the current working directory as a string.
 * 
 * @return std::string The current working directory path
 */
std::string getCurrentDirectory();

/**
 * @brief Creates a new file or directory
 * 
 * Creates a new file or directory at the specified path. If creating a file,
 * any necessary parent directories will be created automatically. The path
 * can be either absolute or relative to the current working directory.
 * 
 * @param path The path where to create the file or directory
 * @param isDirectory true to create a directory, false to create a file
 * @return true if creation was successful, false otherwise
 */
bool fsCreate(const std::string& path, bool isDirectory);

/**
 * @brief Deletes a file or directory
 * 
 * Deletes the specified file or directory. If deleting a directory,
 * all its contents will be deleted recursively. The current working
 * directory cannot be deleted.
 * 
 * @param path The path to the file or directory to delete
 * @return true if deletion was successful, false otherwise
 */
bool fsDelete(const std::string& path);

/**
 * @brief Renames or moves a file or directory
 * 
 * Renames or moves a file or directory from oldPath to newPath.
 * Both paths can be either absolute or relative to the current working directory.
 * The current working directory cannot be renamed.
 * 
 * @param oldPath The current path of the file or directory
 * @param newPath The new path for the file or directory
 * @return true if rename was successful, false otherwise
 */
bool fsRename(const std::string& oldPath, const std::string& newPath); 