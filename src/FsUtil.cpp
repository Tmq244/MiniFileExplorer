#include "FsUtil.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>

using namespace std;

namespace fs = filesystem;

namespace fsutil {

// Helper function to convert file_time_type to time_t
static time_t toTimeT(const fs::file_time_type& ftime) {
    auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(
        ftime - fs::file_time_type::clock::now() + chrono::system_clock::now()
    );
    return chrono::system_clock::to_time_t(sctp);
}

// Helper function for case-insensitive string search
static bool containsIgnoreCase(const string& str, const string& keyword) {
    string strLower = str;
    string keyLower = keyword;
    transform(strLower.begin(), strLower.end(), strLower.begin(),
              [](unsigned char c) { return tolower(c); });
    transform(keyLower.begin(), keyLower.end(), keyLower.begin(),
              [](unsigned char c) { return tolower(c); });
    return strLower.find(keyLower) != string::npos;
}

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
    // Handle empty input - return base
    if (userInputPath.empty()) {
        return base;
    }

    // Handle ~ for home directory
    if (userInputPath[0] == '~') {
        const char* home = getenv("HOME");
        if (home) {
            string expanded = home;
            if (userInputPath.size() > 1) {
                expanded += userInputPath.substr(1);
            }
            return fs::weakly_canonical(fs::path(expanded));
        }
    }

    fs::path p(userInputPath);
    if (p.is_relative()) {
        return fs::weakly_canonical(base / p);
    }
    return fs::weakly_canonical(p);
}

vector<FileInfo> listDirectory(const fs::path& dir) {
    vector<FileInfo> result;

    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        return result;
    }

    for (const auto& entry : fs::directory_iterator(dir)) {
        FileInfo info;
        info.name = entry.path().filename().string();
        info.path = entry.path();
        info.isDirectory = entry.is_directory();

        // Get size (only for files, directories show as 0)
        if (entry.is_regular_file()) {
            info.size = entry.file_size();
        } else {
            info.size = 0;  // Directories show "-" in display
        }

        // Get timestamps
        try {
            info.mtime = toTimeT(entry.last_write_time());
            // Note: C++17 filesystem doesn't provide atime/ctime directly
            // We use mtime as approximation for all timestamps
            info.atime = info.mtime;
            info.ctime = info.mtime;
        } catch (...) {
            info.mtime = 0;
            info.atime = 0;
            info.ctime = 0;
        }

        result.push_back(info);
    }

    return result;
}

FileInfo getFileInfo(const fs::path& p, bool calcDirSize) {
    FileInfo info;
    info.name = p.filename().string();
    info.path = p;

    if (!fs::exists(p)) {
        return info;
    }

    info.isDirectory = fs::is_directory(p);

    // Get size
    if (info.isDirectory) {
        if (calcDirSize) {
            info.size = calcDirectorySize(p);
        } else {
            info.size = 0;  // Will display as "-"
        }
    } else if (fs::is_regular_file(p)) {
        info.size = fs::file_size(p);
    }

    // Get timestamps
    try {
        info.mtime = toTimeT(fs::last_write_time(p));
        // C++17 doesn't provide direct access to atime/ctime
        // Using mtime as approximation
        info.atime = info.mtime;
        info.ctime = info.mtime;
    } catch (...) {
        info.mtime = 0;
        info.atime = 0;
        info.ctime = 0;
    }

    return info;
}

void createFile(const fs::path& file) {
    // Create an empty file using ofstream
    ofstream ofs(file);
    ofs.close();
}

void createDir(const fs::path& dir) {
    fs::create_directory(dir);
}

void removeFile(const fs::path& file) {
    if (fs::exists(file) && fs::is_regular_file(file)) {
        fs::remove(file);
    }
}

bool removeEmptyDir(const fs::path& dir) {
    if (!fs::exists(dir)) {
        return false;
    }

    if (!fs::is_directory(dir)) {
        return false;
    }

    // Check if directory is empty
    if (!fs::is_empty(dir)) {
        return false;  // Directory not empty
    }

    fs::remove(dir);
    return true;
}

vector<FileInfo> searchRecursive(
    const fs::path& start,
    const string& keyword
) {
    vector<FileInfo> result;

    if (!fs::exists(start) || !fs::is_directory(start)) {
        return result;
    }

    // Use recursive_directory_iterator to traverse all subdirectories
    for (const auto& entry : fs::recursive_directory_iterator(start)) {
        string filename = entry.path().filename().string();

        // Case-insensitive search
        if (containsIgnoreCase(filename, keyword)) {
            FileInfo info;
            info.name = filename;
            info.path = entry.path();
            info.isDirectory = entry.is_directory();

            if (entry.is_regular_file()) {
                info.size = entry.file_size();
            } else {
                info.size = 0;
            }

            try {
                info.mtime = toTimeT(entry.last_write_time());
                info.atime = info.mtime;
                info.ctime = info.mtime;
            } catch (...) {
                info.mtime = 0;
                info.atime = 0;
                info.ctime = 0;
            }

            result.push_back(info);
        }
    }

    return result;
}

uintmax_t calcDirectorySize(const fs::path& dir) {
    uintmax_t totalSize = 0;

    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        return 0;
    }

    // Sum file sizes recursively
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (entry.is_regular_file()) {
            totalSize += entry.file_size();
        }
    }

    return totalSize;
}

void copyFile(const fs::path& src,
              const fs::path& dst,
              bool overwrite) {
    if (!fs::exists(src)) {
        throw runtime_error("Source not found");
    }

    fs::path targetPath = dst;

    // If destination is a directory, copy into it with same filename
    if (fs::is_directory(dst)) {
        targetPath = dst / src.filename();
    }

    // Check if target already exists
    if (fs::exists(targetPath) && !overwrite) {
        throw runtime_error("File exists in target");
    }

    // Copy with overwrite option
    fs::copy_options options = fs::copy_options::none;
    if (overwrite) {
        options = fs::copy_options::overwrite_existing;
    }

    fs::copy_file(src, targetPath, options);
}

void movePath(const fs::path& src,
              const fs::path& dst,
              bool overwrite) {
    if (!fs::exists(src)) {
        throw runtime_error("Source not found");
    }

    fs::path targetPath = dst;

    // If destination is a directory, move into it with same filename
    if (fs::is_directory(dst)) {
        targetPath = dst / src.filename();
    }

    // Check if target already exists
    if (fs::exists(targetPath) && !overwrite) {
        throw runtime_error("File exists in target");
    }

    // If overwrite is enabled and target exists, remove it first
    if (fs::exists(targetPath) && overwrite) {
        if (fs::is_directory(targetPath)) {
            fs::remove_all(targetPath);
        } else {
            fs::remove(targetPath);
        }
    }

    // Use rename for move operation
    fs::rename(src, targetPath);
}

} // namespace fsutil
