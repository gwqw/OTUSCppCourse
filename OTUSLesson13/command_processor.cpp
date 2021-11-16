#include "command_processor.h"

#include <utility>

using namespace std;

void process_all_commands(ICmdReader* cmdReader, BulkCmdManager* bulkMgr) {
    while (true) {
        auto cmd = cmdReader->read_next_cmd();
        bool to_break = cmd.cmd_type == CommandType::Terminator;
        bulkMgr->add_cmd(move(cmd));
        if (to_break) break;
    }
}