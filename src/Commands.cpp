#include "Commands.h"
#include "FsUtil.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <ctime>

using namespace std;
namespace fs = filesystem;

// Helper function to format time
static string formatTime(time_t t) {
    if (t == 0) return "-";
    char buf[64];
    struct tm* tm_info = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return string(buf);
}

// Helper function to format size with auto unit (KB/MB)
static string formatSizeAuto(uintmax_t bytes) {
    if (bytes >= 1024 * 1024) {
        return to_string(bytes / (1024 * 1024)) + " MB";
    } else if (bytes >= 1024) {
        return to_string(bytes / 1024) + " KB";
    } else {
        return to_string(bytes) + " B";
    }
}

void registerBuiltInCommands(CommandRegistry& registry) {

    // ==================== help ====================
    registry.registerCommand(
        "help",
        "Show all available commands.",
        [&registry](const vector<string>& /*args*/, FileSystemContext& /*ctx*/) {
            cout << "Available commands:\n";
            for (const auto& cmd : registry.all()) {
                cout << "  " << cmd.name << ": " << cmd.description << "\n";
            }
        }
    );

    // ==================== exit ====================
    registry.registerCommand(
        "exit",
        "Exit MiniFileExplorer.",
        [](const vector<string>& /*args*/, FileSystemContext& ctx) {
            cout << "MiniFileExplorer closed successfully\n";
            ctx.running = false;
        }
    );

    // ==================== cd ====================
    registry.registerCommand(
        "cd",
        "Switch to target directory. Usage: cd [path]",
        [](const vector<string>& args, FileSystemContext& ctx) {
            fs::path targetPath;

            if (args.empty()) {
                // cd with no args -> go to home directory
                targetPath = ctx.homeDir;
            } else {
                // Normalize the path (handles ~, relative paths, etc.)
                targetPath = fsutil::normalizePath(ctx.currentDir, args[0]);
            }

            // Check if path exists
            if (!fs::exists(targetPath)) {
                cout << "Invalid directory: " << targetPath << "\n";
                return;
            }

            // Check if it's a directory (not a file)
            if (!fs::is_directory(targetPath)) {
                cout << "Not a directory: " << targetPath << "\n";
                return;
            }

            ctx.currentDir = targetPath;
        }
    );

    // ==================== ls ====================
    registry.registerCommand(
        "ls",
        "List all files and directories. Usage: ls [-s|-t]",
        [](const vector<string>& args, FileSystemContext& ctx) {
            vector<FileInfo> entries = fsutil::listDirectory(ctx.currentDir);

            if (entries.empty()) {
                cout << "(empty directory)\n";
                return;
            }

            // Check for sorting options
            bool sortBySize = false;
            bool sortByTime = false;

            for (const auto& arg : args) {
                if (arg == "-s") sortBySize = true;
                if (arg == "-t") sortByTime = true;
            }

            // Sort if requested
            if (sortBySize) {
                // Sort by size descending (need to calculate dir sizes)
                for (auto& entry : entries) {
                    if (entry.isDirectory) {
                        entry.size = fsutil::calcDirectorySize(entry.path);
                    }
                }
                sort(entries.begin(), entries.end(),
                     [](const FileInfo& a, const FileInfo& b) {
                         // Empty folders at the end
                         if (a.size == 0 && b.size != 0) return false;
                         if (a.size != 0 && b.size == 0) return true;
                         return a.size > b.size;
                     });
            } else if (sortByTime) {
                // Sort by modification time descending
                sort(entries.begin(), entries.end(),
                     [](const FileInfo& a, const FileInfo& b) {
                         return a.mtime > b.mtime;
                     });
            }

            // Print header
            cout << left << setw(30) << "Name"
                 << setw(8) << "Type"
                 << setw(15) << "Size(B)"
                 << "Modify Time" << "\n";
            cout << string(75, '-') << "\n";

            // Print entries
            for (const auto& entry : entries) {
                string displayName = entry.name;
                if (entry.isDirectory) {
                    displayName += "/";
                }

                string typeStr = entry.isDirectory ? "Dir" : "File";
                string sizeStr = entry.isDirectory ? "-" : to_string(entry.size);

                cout << left << setw(30) << displayName
                     << setw(8) << typeStr
                     << setw(15) << sizeStr
                     << formatTime(entry.mtime) << "\n";
            }
        }
    );

    // ==================== touch ====================
    registry.registerCommand(
        "touch",
        "Create an empty file. Usage: touch [filename]",
        [](const vector<string>& args, FileSystemContext& ctx) {
            if (args.empty()) {
                cout << "Missing filename: Please enter 'touch [filename]'\n";
                return;
            }

            fs::path filePath = fsutil::normalizePath(ctx.currentDir, args[0]);

            // Check if file already exists
            if (fs::exists(filePath)) {
                cout << "File already exists: " << args[0] << "\n";
                return;
            }

            try {
                fsutil::createFile(filePath);
                cout << "Created file: " << args[0] << "\n";
            } catch (const exception& e) {
                cout << "Error creating file: " << e.what() << "\n";
            }
        }
    );

    // ==================== mkdir ====================
    registry.registerCommand(
        "mkdir",
        "Create a new directory. Usage: mkdir [foldername]",
        [](const vector<string>& args, FileSystemContext& ctx) {
            if (args.empty()) {
                cout << "Missing folder name: Please enter 'mkdir [foldername]'\n";
                return;
            }

            fs::path dirPath = fsutil::normalizePath(ctx.currentDir, args[0]);

            // Check if directory already exists
            if (fs::exists(dirPath)) {
                cout << "Directory already exists: " << args[0] << "\n";
                return;
            }

            try {
                fsutil::createDir(dirPath);
                cout << "Created directory: " << args[0] << "\n";
            } catch (const exception& e) {
                cout << "Error creating directory: " << e.what() << "\n";
            }
        }
    );

    // ==================== rm ====================
    registry.registerCommand(
        "rm",
        "Delete a file. Usage: rm [filename]",
        [](const vector<string>& args, FileSystemContext& ctx) {
            if (args.empty()) {
                cout << "Missing filename: Please enter 'rm [filename]'\n";
                return;
            }

            fs::path filePath = fsutil::normalizePath(ctx.currentDir, args[0]);

            // Check if file exists
            if (!fs::exists(filePath)) {
                cout << "File not found: " << args[0] << "\n";
                return;
            }

            // Check if it's a file (not a directory)
            if (fs::is_directory(filePath)) {
                cout << "Not a file (use rmdir for directories): " << args[0] << "\n";
                return;
            }

            // Confirmation prompt
            cout << "Are you sure to delete " << args[0] << "? (y/n) ";
            string response;
            getline(cin, response);

            if (response == "y" || response == "Y") {
                try {
                    fsutil::removeFile(filePath);
                    cout << "Deleted: " << args[0] << "\n";
                } catch (const exception& e) {
                    cout << "Error deleting file: " << e.what() << "\n";
                }
            } else {
                cout << "Deletion cancelled.\n";
            }
        }
    );

    // ==================== rmdir ====================
    registry.registerCommand(
        "rmdir",
        "Delete an empty directory. Usage: rmdir [foldername]",
        [](const vector<string>& args, FileSystemContext& ctx) {
            if (args.empty()) {
                cout << "Missing folder name: Please enter 'rmdir [foldername]'\n";
                return;
            }

            fs::path dirPath = fsutil::normalizePath(ctx.currentDir, args[0]);

            // Check if directory exists
            if (!fs::exists(dirPath)) {
                cout << "Directory not found: " << args[0] << "\n";
                return;
            }

            // Check if it's a directory
            if (!fs::is_directory(dirPath)) {
                cout << "Not a directory: " << args[0] << "\n";
                return;
            }

            // Try to remove (will fail if not empty)
            if (!fsutil::removeEmptyDir(dirPath)) {
                cout << "Directory not empty: " << args[0] << "\n";
                return;
            }

            cout << "Deleted directory: " << args[0] << "\n";
        }
    );

    // ==================== stat ====================
    registry.registerCommand(
        "stat",
        "Show file/directory info. Usage: stat [name]",
        [](const vector<string>& args, FileSystemContext& ctx) {
            if (args.empty()) {
                cout << "Missing target: Please enter 'stat [name]'\n";
                return;
            }

            fs::path targetPath = fsutil::normalizePath(ctx.currentDir, args[0]);

            // Check if target exists
            if (!fs::exists(targetPath)) {
                cout << "Target not found: " << args[0] << "\n";
                return;
            }

            FileInfo info = fsutil::getFileInfo(targetPath, true);

            cout << "Information for: " << args[0] << "\n";
            cout << string(40, '-') << "\n";
            cout << "Type:              " << (info.isDirectory ? "Directory" : "File") << "\n";
            cout << "Path:              " << info.path << "\n";
            cout << "Size:              " << (info.isDirectory ? "-" : to_string(info.size) + " bytes") << "\n";
            cout << "Creation Time:     " << formatTime(info.ctime) << "\n";
            cout << "Modification Time: " << formatTime(info.mtime) << "\n";
            cout << "Access Time:       " << formatTime(info.atime) << "\n";
        }
    );

    // ==================== search ====================
    registry.registerCommand(
        "search",
        "Search files/folders by keyword (recursive, case-insensitive). Usage: search [keyword]",
        [](const vector<string>& args, FileSystemContext& ctx) {
            if (args.empty()) {
                cout << "Missing keyword: Please enter 'search [keyword]'\n";
                return;
            }

            const string& keyword = args[0];
            vector<FileInfo> results = fsutil::searchRecursive(ctx.currentDir, keyword);

            if (results.empty()) {
                cout << "No results found for '" << keyword << "'\n";
                return;
            }

            cout << "Search results for '" << keyword << "' (" << results.size() << " items):\n";
            for (const auto& result : results) {
                string typeStr = result.isDirectory ? "(Dir)" : "(File)";
                cout << result.path.string() << " " << typeStr << "\n";
            }
        }
    );

    // ==================== cp ====================
    registry.registerCommand(
        "cp",
        "Copy file. Usage: cp [source] [target]",
        [](const vector<string>& args, FileSystemContext& ctx) {
            if (args.size() < 2) {
                cout << "Usage: cp [source file path] [target path]\n";
                return;
            }

            fs::path srcPath = fsutil::normalizePath(ctx.currentDir, args[0]);
            fs::path dstPath = fsutil::normalizePath(ctx.currentDir, args[1]);

            // Check if source exists
            if (!fs::exists(srcPath)) {
                cout << "Source not found: " << args[0] << "\n";
                return;
            }

            // Determine target file path
            fs::path targetFile = dstPath;
            if (fs::is_directory(dstPath)) {
                targetFile = dstPath / srcPath.filename();
            }

            // Check if target file exists
            bool overwrite = false;
            if (fs::exists(targetFile)) {
                cout << "File exists in target: Overwrite? (y/n) ";
                string response;
                getline(cin, response);
                if (response != "y" && response != "Y") {
                    cout << "Copy cancelled.\n";
                    return;
                }
                overwrite = true;
            }

            try {
                fsutil::copyFile(srcPath, dstPath, overwrite);
                cout << "Copied: " << args[0] << " -> " << targetFile << "\n";
            } catch (const exception& e) {
                cout << "Error copying: " << e.what() << "\n";
            }
        }
    );

    // ==================== mv ====================
    registry.registerCommand(
        "mv",
        "Move/rename file or folder. Usage: mv [source] [target]",
        [](const vector<string>& args, FileSystemContext& ctx) {
            if (args.size() < 2) {
                cout << "Usage: mv [source path] [target path]\n";
                return;
            }

            fs::path srcPath = fsutil::normalizePath(ctx.currentDir, args[0]);
            fs::path dstPath = fsutil::normalizePath(ctx.currentDir, args[1]);

            // Check if source exists
            if (!fs::exists(srcPath)) {
                cout << "Source not found: " << args[0] << "\n";
                return;
            }

            // Determine target path
            fs::path targetPath = dstPath;
            if (fs::is_directory(dstPath)) {
                targetPath = dstPath / srcPath.filename();
            }

            // Check if target exists
            bool overwrite = false;
            if (fs::exists(targetPath)) {
                cout << "Target exists: Overwrite? (y/n) ";
                string response;
                getline(cin, response);
                if (response != "y" && response != "Y") {
                    cout << "Move cancelled.\n";
                    return;
                }
                overwrite = true;
            }

            // Check if target directory exists
            if (!fs::exists(dstPath.parent_path()) && !fs::is_directory(dstPath)) {
                cout << "Invalid target path: " << args[1] << "\n";
                return;
            }

            try {
                fsutil::movePath(srcPath, dstPath, overwrite);
                cout << "Moved: " << args[0] << " -> " << targetPath << "\n";
            } catch (const exception& e) {
                cout << "Error moving: " << e.what() << "\n";
            }
        }
    );

    // ==================== du ====================
    registry.registerCommand(
        "du",
        "Calculate directory size. Usage: du [foldername]",
        [](const vector<string>& args, FileSystemContext& ctx) {
            if (args.empty()) {
                cout << "Missing folder name: Please enter 'du [foldername]'\n";
                return;
            }

            fs::path dirPath = fsutil::normalizePath(ctx.currentDir, args[0]);

            // Check if path exists
            if (!fs::exists(dirPath)) {
                cout << "Directory not found: " << args[0] << "\n";
                return;
            }

            // Check if it's a directory
            if (!fs::is_directory(dirPath)) {
                cout << "Not a directory: " << args[0] << "\n";
                return;
            }

            uintmax_t totalSize = fsutil::calcDirectorySize(dirPath);
            cout << "Total size of " << args[0] << ": " << formatSizeAuto(totalSize) << "\n";
        }
    );
}
