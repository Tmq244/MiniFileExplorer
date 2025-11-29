#include"App.h"
#include<iostream>
#include"CommandParser.h"

using namespace std;

App::App(FileSystemContext& ctx, const CommandRegistry& registry)
    :ctx_(ctx),registry_(registry){}

void App::printPrompt(){
    cout<<"Current Directory: "<<ctx_.currentDir<<endl;
    cout<<"Enter command (type 'help' for all commands): ";
}

void App::executeLine(const string& line) const{
    ParsedCommand parsed=CommandParser::parse(line);

    if(parsed.name.empty()){
        return;
    }

    const Command* cmd= registry_.find(parsed.name);
    if(!cmd){
        cout<<"Unknown command: "<<parsed.name<<endl;
        return;
    }

    cmd->handler(parsed.args,ctx_);

}

void App::run(){
    string line;
    while(ctx_.running){
        printPrompt();
        if(!getline(cin,line)){
            break;
        }
        executeLine(line);
    }
}
