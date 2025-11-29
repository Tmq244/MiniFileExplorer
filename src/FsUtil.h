#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "FileInfo.h"

using namespace std;

namespace fsutil {

bool existsDir(const filesystem::path& p);
bool existsFile(const filesystem::path& p);
bool isDirectory(const filesystem::path& p);

filesystem::path normalizePath(
    const filesystem::path& base,
    const string& userInputPath
);

vector<FileInfo> listDirectory(const filesystem::path& dir);

FileInfo getFileInfo(const filesystem::path& p, bool calcDirSize = false);

void createFile(const filesystem::path& file);
void createDir(const filesystem::path& dir);
void removeFile(const filesystem::path& file);
bool removeEmptyDir(const filesystem::path& dir);

vector<FileInfo> searchRecursive(
    const filesystem::path& start,
    const string& keyword
);

uintmax_t calcDirectorySize(const filesystem::path& dir);

void copyFile(const filesystem::path& src,
              const filesystem::path& dst,
              bool overwrite);

void movePath(const filesystem::path& src,
              const filesystem::path& dst,
              bool overwrite);

} // namespace fsutil
