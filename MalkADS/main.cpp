#include "include/managers/LibrarySystem.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/stat.h>
#include <direct.h> // For _chdir on Windows

using namespace std;

bool directoryExists(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
        return false;
    return (info.st_mode & S_IFDIR) != 0;
}

void setupWorkingDirectory() {
    // Check if "data" exists in current directory
    if (directoryExists("data")) {
        return; // All good
    }
    
    // Check if "data" exists in parent directory (common when running from cmake-build-debug)
    if (directoryExists("../data")) {
        std::cout << "Data directory found in parent. Switching working directory..." << std::endl;
        _chdir("..");
    }
}

int main(int argc, char* argv[]) {
    // Ensure we are in the project root
    setupWorkingDirectory();

    bool launchGui = false;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--gui" || arg == "-g") {
            launchGui = true;
        }
    }

    if (launchGui) {
        std::cout << "Launching GUI..." << std::endl;
        // Use python to run the GUI script
        int result = std::system("python gui/main.py");
        return result;
    } else {
        // Launch CLI
        LibrarySystem library_system;
        library_system.run();
    }

    return 0;
}
