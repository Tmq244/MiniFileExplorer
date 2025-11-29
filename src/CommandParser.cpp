#include "CommandParser.h"

#include <sstream>

using namespace std;
ParsedCommand CommandParser::parse(const string& line) {
    ParsedCommand result;

    // split the line into tokens by whitespace
    istringstream iss(line);
    string token;
    bool first = true;

    while (iss >> token) {
        if (first) {
            result.name = token;
            first = false;
        } else {
            result.args.push_back(token);
        }
    }

    return result;
}
