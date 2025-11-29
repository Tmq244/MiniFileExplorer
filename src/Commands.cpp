#include "Commands.h"

#include <iostream>

using namespace std;

void registerBuiltInCommands(CommandRegistry& registry) {

    registry.registerCommand(
        "help",
        "Show all available commands.",
        [&registry](const vector<string>& /*args*/, FileSystemContext& /*ctx*/) { // Commented args and ctx because they are not used in the command handler
            cout << "TODO: implement 'help' command.\n";
            cout << "Available commands:\n";
            for (const auto& cmd : registry.all()) {
                cout << "  " << cmd.name << " - " << cmd.description << "\n";
            }
        }
    );

    registry.registerCommand(
        "exit",
        "Exit MiniFileExplorer.",
        [](const vector<string>& /*args*/, FileSystemContext& ctx) {
            cout << "MiniFileExplorer closed successfully\n";
            ctx.running = false;
        }
    );

    registry.registerCommand(
        "cd",
        "Change current directory. Usage: cd [path]",
        [](const vector<string>& args, FileSystemContext& /*ctx*/) {
            cout << "TODO: implement 'cd' command. Args count = "
                      << args.size() << "\n";
        }
    );

    registry.registerCommand(
        "ls",
        "List directory contents. Usage: ls [-s|-t]",
        [](const vector<string>& args, FileSystemContext& /*ctx*/) {
            cout << "TODO: implement 'ls' command. Args count = "
                      << args.size() << "\n";
        }
    );

    registry.registerCommand(
        "touch",
        "Create an empty file. Usage: touch [filename]",
        [](const vector<string>& /*args*/, FileSystemContext& /*ctx*/) {
            cout << "TODO: implement 'touch' command.\n";
        }
    );

    registry.registerCommand(
        "mkdir",
        "Create a new directory. Usage: mkdir [foldername]",
        [](const vector<string>& /*args*/, FileSystemContext& /*ctx*/) {
            cout << "TODO: implement 'mkdir' command.\n";
        }
    );

    registry.registerCommand(
        "rm",
        "Delete a file. Usage: rm [filename]",
        [](const vector<string>& /*args*/, FileSystemContext& /*ctx*/) {
            cout << "TODO: implement 'rm' command.\n";
        }
    );

    registry.registerCommand(
        "rmdir",
        "Delete an empty directory. Usage: rmdir [foldername]",
        [](const vector<string>& /*args*/, FileSystemContext& /*ctx*/) {
            cout << "TODO: implement 'rmdir' command.\n";
        }
    );

    registry.registerCommand(
        "stat",
        "Show file/directory info. Usage: stat [name]",
        [](const vector<string>& /*args*/, FileSystemContext& /*ctx*/) {
            cout << "TODO: implement 'stat' command.\n";
        }
    );

    registry.registerCommand(
        "search",
        "Search files/folders by keyword in current directory (recursive).",
        [](const vector<string>& /*args*/, FileSystemContext& /*ctx*/) {
            cout << "TODO: implement 'search' command.\n";
        }
    );

    registry.registerCommand(
        "cp",
        "Copy file. Usage: cp [source] [target]",
        [](const vector<string>& /*args*/, FileSystemContext& /*ctx*/) {
            cout << "TODO: implement 'cp' command.\n";
        }
    );

    registry.registerCommand(
        "mv",
        "Move/rename file or folder. Usage: mv [source] [target]",
        [](const vector<string>& /*args*/, FileSystemContext& /*ctx*/) {
            cout << "TODO: implement 'mv' command.\n";
        }
    );

    registry.registerCommand(
        "du",
        "Calculate directory size. Usage: du [foldername]",
        [](const vector<string>& /*args*/, FileSystemContext& /*ctx*/) {
            cout << "TODO: implement 'du' command.\n";
        }
    );
}
