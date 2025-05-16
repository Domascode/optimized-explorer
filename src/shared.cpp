//Used for shared variables between files

#include "fs.h"
#include <vector>
#include <string>

/**
 * @brief List of system paths that should be skipped during traversal
 * 
 * These paths are either system directories or files that shouldn't be accessed
 * to prevent system issues or unnecessary processing.
 */
const std::vector<std::string> SYSTEM_SKIP_PATHS = {
    "$Recycle.Bin",
    "System Volume Information",
    "pagefile.sys",
    "hiberfil.sys",
    "swapfile.sys"
};

bool shouldSkipPath(const std::string& path) {
    for (const auto& skipPath : SYSTEM_SKIP_PATHS) {
        if (path.find(skipPath) != std::string::npos) {
            return true;
        }
    }
    return false;
}