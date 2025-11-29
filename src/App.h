#pragma once

#include<string>
#include"Command.h"
#include"FileSystemContext.h"

class App{
public:
    App(FileSystemContext& ctx,const CommandRegistry& registry);
    void run();
private:
    FileSystemContext& ctx_;
    const CommandRegistry& registry_;
    void printPrompt() const;
    void executeLine(const std::string& line) const;
};
