#pragma once

#include <string>
#include <vector>

using namespace std;

struct ParsedCommand {
    string name;
    vector<string> args;
};

class CommandParser {
public:
    static ParsedCommand parse(const string& line);
};
