#include <iostream>
#include <memory>
#include <vector>

#include "bulk.h"
#include "command_handler.h"
#include "command_reader.h"
#include "command_processor.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Incorrect parameter number. Bulk must have one parameter: bulk size" << endl;
        return -1;
    }
    size_t bulk_size = 0;
    try {
        bulk_size = stol(argv[1]);
    } catch (const std::exception& e) {
        cerr << "'" << argv[1] << "'" << " is not a number. Cmd paramenter must be a number. ";
        cerr << e.what() << endl;
        return -2;
    }
    
    // init
    auto bulkMgr = make_unique<BulkCmdManager>(bulk_size);
    auto commandReader = make_unique<StreamCmdReader>(cin);
    createObserverAndSubscribe<CmdStreamHandler>(bulkMgr.get());
    createObserverAndSubscribe<CmdFileHandler>(bulkMgr.get());

    // start commands cycle
    process_all_commands(commandReader.get(), bulkMgr.get());

	return 0;
}
