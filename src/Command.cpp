#include "Command.h"

using namespace std;
void CommandRegistry::registerCommand(const string& name,
                                      const string& description,
                                      CommandHandler handler) {
    commands_.push_back(Command{name, description, move(handler)});
}

const Command* CommandRegistry::find(const string& name) const {
    for (const auto& cmd : commands_) { //command_: vector<Command> 
        if (cmd.name == name) return &cmd;
    }
    return nullptr;
}
