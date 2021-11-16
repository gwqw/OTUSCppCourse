#include "command_reader.h"

#include <utility>
#include <string>

using namespace std;

Command StreamCmdReader::read_next_cmd() {
    string cmd_line;
    if (in_ && getline(in_, cmd_line)) {
        if (cmd_line == "{") {
            return Command{CommandType::StartCustomBulk};
        } else if (cmd_line == "}") {
            return Command{CommandType::StopCustomBulk};
        } else {
            return Command{CommandType::Base, std::move(cmd_line)};
        }
    } else {
        return Command{CommandType::Terminator};
    }
}