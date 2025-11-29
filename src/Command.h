#pragma once

#include <functional>
#include <string>
#include <vector>

#include "FileSystemContext.h"

using namespace std;

// Define a type alias for the command handler function
using CommandHandler = function<void(
    const vector<string>& args,
    FileSystemContext& ctx
)>;

struct Command {
    string name;
    string description;
    CommandHandler handler;
};

class CommandRegistry {
public:
    void registerCommand(const string& name,
                         const string& description,
                         CommandHandler handler);

    const Command* find(const string& name) const;

    const vector<Command>& all() const { return commands_; }

private:
    vector<Command> commands_;
};
