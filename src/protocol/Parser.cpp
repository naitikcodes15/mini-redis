#include "protocol/Parser.hpp"
#include <sstream>
#include <algorithm>

using namespace std;

Command Parser::parse(const string& raw_input){
    stringstream ss(raw_input);
    string token;
    vector<string> tokens;

    while(ss >> token){
        tokens.push_back(token);
    }
    if (tokens.empty()) return {CommandType::UNKNOWN, {}};
    // Normalize command to uppercase (e.g., "set" -> "SET")
    string cmd_name = tokens[0];
    transform(cmd_name.begin(), cmd_name.end(), cmd_name.begin(), ::toupper);

    Command cmd;
    if (cmd_name == "SET") cmd.type = CommandType::SET;
    else if (cmd_name == "GET") cmd.type = CommandType::GET;
    else if (cmd_name == "DEL") cmd.type = CommandType::DEL;
    else if (cmd_name == "PING") cmd.type = CommandType::PING;
    else cmd.type = CommandType::UNKNOWN;

    // Everything after the command name is an argument
    for (size_t i = 1; i < tokens.size(); ++i) {
        cmd.args.push_back(tokens[i]);
    }
    return cmd;
}
