#include "FsUtil.h"

#include <filesystem>
#include <iostream>

using namespace std;

namespace fs = filesystem;

namespace fsutil {

bool existsDir(const fs::path& p) {
    return fs::exists(p) && fs::is_directory(p);
}

bool existsFile(const fs::path& p) {
    return fs::exists(p) && fs::is_regular_file(p);
}

bool isDirectory(const fs::path& p) {
    return fs::is_directory(p);
}

fs::path normalizePath(const fs::path& base, const string& userInputPath) {
    // TODO: handle ~, ., .. etc more carefully if you like
    fs::path p(userInputPath);
    if (p.is_relative()) {
        return fs::weakly_canonical(base / p);
    }
    return fs::weakly_canonical(p);
}

vector<FileInfo> listDirectory(const fs::path& dir) {
    vector<FileInfo> result;
    // TODO: fill using directory_iterator
    (void)dir;
    return result;
}

FileInfo getFileInfo(const fs::path& p, bool /*calcDirSize*/) {
    FileInfo info;
    info.name = p.filename().string();
    info.path = p;
    info.isDirectory = fs::is_directory(p);
    // TODO: size and timestamps
    return info;
}

void createFile(const fs::path& file) {
    // TODO: actually create file (e.g., ofstream)
    (void)file;
}

void createDir(const fs::path& dir) {
    // TODO: create directories
    (void)dir;
}

void removeFile(const fs::path& file) {
    // TODO: remove file
    (void)file;
}

bool removeEmptyDir(const fs::path& dir) {
    // TODO: remove dir if empty
    (void)dir;
    return false;
}

vector<FileInfo> searchRecursive(
    const fs::path& start,
    const string& keyword
) {
    vector<FileInfo> result;
    // TODO: implement recursive search
    (void)start;
    (void)keyword;
    return result;
}

uintmax_t calcDirectorySize(const fs::path& dir) {
    // TODO: sum file sizes recursively
    (void)dir;
    return 0;
}

void copyFile(const fs::path& src,
              const fs::path& dst,
              bool /*overwrite*/) {
    // TODO: use fs::copy_file
    (void)src;
    (void)dst;
}

void movePath(const fs::path& src,
              const fs::path& dst,
              bool /*overwrite*/) {
    // TODO: use fs::rename or copy+remove
    (void)src;
    (void)dst;
}

} // namespace fsutil
