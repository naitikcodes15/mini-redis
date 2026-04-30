#pragma once 
#include<string>
#include<vector>

using namespace std;


enum class CommandType { SET , GET , DEL , PING , UNKNOWN};

struct Command {
    CommandType type;
    vector<string> args;
};