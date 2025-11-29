# MiniFileExplorer

A lightweight, terminal-based file manager built in C++17 that provides core file system operations similar to Linux command-line tools.

## Overview

MiniFileExplorer is a command-line file management tool that allows users to navigate, create, delete, search, and manipulate files and directories through an interactive terminal interface. It's designed to be simple, efficient, and user-friendly.

## Features

### Basic Operations
- ✅ **Directory Navigation**: Change directories with `cd`, support for relative/absolute paths and `~` for home directory
- ✅ **File Listing**: List directory contents with detailed information (name, type, size, modification time)
- ✅ **File Creation**: Create empty files with `touch`
- ✅ **Directory Creation**: Create directories with `mkdir`
- ✅ **File Deletion**: Delete files with confirmation prompt using `rm`
- ✅ **Directory Deletion**: Delete empty directories with `rmdir`
- ✅ **File Information**: Get detailed file/directory metadata with `stat`

### Advanced Features
- 🔍 **Recursive Search**: Case-insensitive file/folder search across subdirectories
- 📋 **File Operations**: Copy and move files/folders with overwrite protection
- 📊 **Directory Analysis**: Calculate total directory size with automatic unit conversion (B/KB/MB)
- 🔄 **Sorting Options**: Sort listings by size (`ls -s`) or modification time (`ls -t`)

### User Experience
- 🎯 **Interactive Command Prompt**: Continuous command input with helpful prompts
- 📖 **Help System**: Built-in help command listing all available commands
- ✨ **Error Handling**: Comprehensive error messages for invalid operations
- 🔒 **Safety Features**: Confirmation prompts for destructive operations

## Requirements

- **C++ Compiler** with C++17 support:
  - GCC 7.0 or higher
  - Clang 5.0 or higher
  - MSVC 2017 or higher
- **CMake** version 3.10 or higher
- **Standard C++ Filesystem Library** (included in C++17)

## Building the Project

### Quick Build

```bash
# Clone or navigate to the project directory
cd MiniFileExplorer

# Create build directory and compile
mkdir -p build
cd build
cmake ..
make

# Run the application
./MiniFileExplorer
```

### Alternative Build Method

```bash
cmake -B build -S .
cmake --build build
./build/MiniFileExplorer
```

### Troubleshooting

If you encounter filesystem library errors with older compilers, uncomment line 17 in `CMakeLists.txt`:

```cmake
target_link_libraries(MiniFileExplorer stdc++fs)
```

## Usage

### Starting the Application

```bash
# Start in current working directory
./MiniFileExplorer

# Start in a specific directory
./MiniFileExplorer /path/to/directory
```

### Interactive Session

Once started, you'll see a prompt:

```
Welcome to MiniFileExplorer!
Current Directory: /home/user
Enter command (type 'help' for all commands):
```

## Command Reference

### Navigation Commands

| Command | Description | Example |
|---------|-------------|---------|
| `cd [path]` | Change directory | `cd documents`, `cd ..`, `cd ~` |
| `cd` | Switch to home directory | `cd` |
| `ls` | List directory contents | `ls` |
| `ls -s` | List sorted by size (descending) | `ls -s` |
| `ls -t` | List sorted by modification time | `ls -t` |

### File Operations

| Command | Description | Example |
|---------|-------------|---------|
| `touch [filename]` | Create an empty file | `touch note.txt` |
| `mkdir [foldername]` | Create a directory | `mkdir archive` |
| `rm [filename]` | Delete a file (with confirmation) | `rm old.txt` |
| `rmdir [foldername]` | Delete an empty directory | `rmdir temp` |
| `stat [name]` | Show detailed file/directory info | `stat note.txt` |

### Advanced Operations

| Command | Description | Example |
|---------|-------------|---------|
| `search [keyword]` | Search files/folders recursively | `search .txt` |
| `cp [source] [target]` | Copy file | `cp file.txt backup/` |
| `mv [source] [target]` | Move/rename file or folder | `mv old.txt new.txt` |
| `du [foldername]` | Calculate directory size | `du documents` |

### Utility Commands

| Command | Description |
|---------|-------------|
| `help` | Show all available commands |
| `exit` | Exit MiniFileExplorer |

## Usage Examples

### Example 1: Basic Navigation and File Creation

```
Current Directory: /home/user
Enter command: cd documents
Current Directory: /home/user/documents
Enter command: touch notes.txt
Created file: notes.txt
Enter command: mkdir archive
Created directory: archive
Enter command: ls
Name                           Type    Size(B)         Modify Time
notes.txt                      File    0               2024-05-20 14:30:00
archive/                       Dir     -               2024-05-20 14:30:15
```

### Example 2: File Information

```
Enter command: stat notes.txt
Information for: notes.txt
----------------------------------------
Type:              File
Path:              /home/user/documents/notes.txt
Size:              1024 bytes
Creation Time:     2024-05-20 14:30:00
Modification Time: 2024-05-20 15:10:23
Access Time:       2024-05-20 15:10:23
```

### Example 3: Recursive Search

```
Enter command: search .txt
Search results for '.txt' (3 items):
/home/user/documents/notes.txt (File)
/home/user/documents/archive/old.txt (File)
/home/user/documents/backup/file.txt (File)
```

### Example 4: Directory Size Calculation

```
Enter command: du documents
Total size of documents: 5 MB
```

### Example 5: File Operations with Confirmation

```
Enter command: rm old.txt
Are you sure to delete old.txt? (y/n) y
Deleted: old.txt

Enter command: cp notes.txt backup/
Copied: notes.txt -> backup/notes.txt
```

## Project Structure

```
MiniFileExplorer/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── src/                    # Source code directory
│   ├── main.cpp           # Application entry point
│   ├── App.h/cpp          # Main application loop
│   ├── Command.h/cpp      # Command registry system
│   ├── Commands.h/cpp     # Built-in command implementations
│   ├── CommandParser.h/cpp # Input parsing
│   ├── FileSystemContext.h # Application state
│   ├── FileInfo.h         # File metadata structure
│   └── FsUtil.h/cpp       # File system utilities
└── build/                 # Build directory (generated)
```

## Architecture

### Component Overview

The project follows a modular architecture with clear separation of concerns:

1. **Application Layer** (`App.h/cpp`): Main event loop and user interaction
2. **Command System** (`Command.h/cpp`, `Commands.h/cpp`): Command registration and execution
3. **Parser Layer** (`CommandParser.h/cpp`): Input tokenization
4. **File System Layer** (`FsUtil.h/cpp`): Low-level file operations
5. **Data Structures** (`FileSystemContext.h`, `FileInfo.h`): State management

### Design Patterns

- **Command Pattern**: Commands are encapsulated as handler functions
- **Registry Pattern**: Centralized command lookup and management
- **Namespace Organization**: File utilities grouped in `fsutil` namespace

## Error Messages

The application provides clear error messages for common issues:

- `Invalid directory: [path]` - Directory doesn't exist
- `Not a directory: [path]` - Path points to a file
- `File already exists: [name]` - Cannot create duplicate file
- `Directory not empty: [name]` - Cannot delete non-empty directory
- `Target not found: [name]` - File/directory doesn't exist
- `Source not found` - Source path doesn't exist
- `File exists in target: Overwrite? (y/n)` - Overwrite confirmation

## Development

### Adding New Commands

To add a new command:

1. Add the command registration in `src/Commands.cpp`:
```cpp
registry.registerCommand(
    "mycommand",
    "Description of my command",
    [](const vector<string>& args, FileSystemContext& ctx) {
        // Implementation here
    }
);
```

2. Implement file system operations in `src/FsUtil.cpp` if needed.

### Building for Development

```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug -B build .
cmake --build build

# Release build
cmake -DCMAKE_BUILD_TYPE=Release -B build .
cmake --build build
```

## License

This project is developed as an educational file manager demonstration.

## Contributing

This is an educational project. Suggestions and improvements are welcome!

## Future Enhancements

Potential features for future versions:
- File permissions management
- File content viewing/editing
- Batch operations
- Customizable color themes
- Configuration file support
- Command history and aliases
- Use Rust to rewrite this project

## Acknowledgments

Built using:
- C++17 Standard Library
- `<filesystem>` library for file operations
- CMake for build configuration

---

**Note**: This is a terminal-based application. Ensure your terminal supports UTF-8 encoding for best display results.

