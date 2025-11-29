#pragma once

#include <filesystem>
#include <string>
#include <cstdint>
#include <ctime>

using namespace std;

struct FileInfo {
    string name;
    filesystem::path path;
    bool isDirectory{false};
    uintmax_t size{0};
    time_t mtime{};
    time_t atime{};
    time_t ctime{};
};
