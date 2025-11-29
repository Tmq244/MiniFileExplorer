#pragma once

#include <filesystem>

using namespace std;

struct FileSystemContext {
    filesystem::path currentDir;
    filesystem::path homeDir;
    bool running{true};
};
