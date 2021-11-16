#pragma once

#include "bulk.h"
#include "command_reader.h"

void process_all_commands(ICmdReader* cmdReader, BulkCmdManager* bulkMgr);