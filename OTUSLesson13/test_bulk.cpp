#define BOOST_TEST_MODULE allocator_test_module
#include <boost/test/unit_test.hpp>
#include <boost/signals2.hpp>

#include <memory>
#include <sstream>
#include <vector>

#include "bulk.h"
#include "command_reader.h"
#include "command_handler.h"
#include "command_processor.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(bulk_whole_test_suite)

	BOOST_AUTO_TEST_CASE(test_1) {
        constexpr int N = 3;
        stringstream in;
        in << "cmd1\n" << "cmd2\n" << "cmd3\n";
        in << "cmd4\n" << "cmd5\n";
        auto bulkMgr = make_unique<BulkCmdManager>(N);
        auto commandReader = make_unique<StreamCmdReader>(in);
        stringstream out;
        createObserverAndSubscribe<CmdStreamHandler>(bulkMgr.get(), out);
        process_all_commands(commandReader.get(), bulkMgr.get());

        string res = "bulk: cmd1, cmd2, cmd3\nbulk: cmd4, cmd5\n";
        BOOST_CHECK(out.str() == res);
	}

    BOOST_AUTO_TEST_CASE(test_2) {
        constexpr int N = 3;
        stringstream in;
        in << "cmd1\n" << "cmd2\n" << "{\n"
           << "cmd3\n" << "cmd4\n" << "cmd5\n" << "cmd6\n" << "cmd7\n" << "}\n";
        auto bulkMgr = make_unique<BulkCmdManager>(N);
        auto commandReader = make_unique<StreamCmdReader>(in);
        stringstream out;
        createObserverAndSubscribe<CmdStreamHandler>(bulkMgr.get(), out);

        // start commands cycle
        process_all_commands(commandReader.get(), bulkMgr.get());

        string res = "bulk: cmd1, cmd2\nbulk: cmd3, cmd4, cmd5, cmd6, cmd7\n";
        BOOST_CHECK(out.str() == res);
    }

    BOOST_AUTO_TEST_CASE(test_3) {
        constexpr int N = 3;
        stringstream in;
        in << "{\n" << "cmd1\n" << "cmd2\n"
           << "{\n" << "cmd3\n" << "cmd4\n" << "}\n"
           << "cmd5\n" << "cmd6\n" << "}\n";
        auto bulkMgr = make_unique<BulkCmdManager>(N);
        auto commandReader = make_unique<StreamCmdReader>(in);
        stringstream out;
        createObserverAndSubscribe<CmdStreamHandler>(bulkMgr.get(), out);

        // start commands cycle
        process_all_commands(commandReader.get(), bulkMgr.get());

        string res = "bulk: cmd1, cmd2, cmd3, cmd4, cmd5, cmd6\n";
        BOOST_CHECK(out.str() == res);
    }

    BOOST_AUTO_TEST_CASE(test_4) {
        constexpr int N = 3;
        stringstream in;
        in << "cmd1\n" << "cmd2\n" << "cmd3\n"
           << "{\n" << "cmd3\n" << "cmd4\n" << "cmd5\n" << "cmd6\n" << "cmd7\n";
        auto bulkMgr = make_unique<BulkCmdManager>(N);
        auto commandReader = make_unique<StreamCmdReader>(in);
        stringstream out;
        createObserverAndSubscribe<CmdStreamHandler>(bulkMgr.get(), out);

        // start commands cycle
        process_all_commands(commandReader.get(), bulkMgr.get());

        string res = "bulk: cmd1, cmd2, cmd3\n";
        BOOST_CHECK(out.str() == res);
    }


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(cmd_reader_test_suite)

    BOOST_AUTO_TEST_CASE(test_Base) {
        {
            stringstream in;
            in << "cmd1\n";
            StreamCmdReader commandReader(in);
            auto cmd = commandReader.read_next_cmd();
            BOOST_CHECK(cmd.cmd_type == CommandType::Base);
            BOOST_CHECK(cmd.data == "cmd1");
        }
        {
            stringstream in;
            in << "cmd1 cmd2\n";
            StreamCmdReader commandReader(in);
            auto cmd = commandReader.read_next_cmd();
            BOOST_CHECK(cmd.cmd_type == CommandType::Base);
            BOOST_CHECK(cmd.data == "cmd1 cmd2");
        }
        {
            stringstream in;
            in << "\n";
            StreamCmdReader commandReader(in);
            auto cmd = commandReader.read_next_cmd();
            BOOST_CHECK(cmd.cmd_type == CommandType::Base);
            BOOST_CHECK(cmd.data == "");
        }
        {
            stringstream in;
            in << "cmd{\n";
            StreamCmdReader commandReader(in);
            auto cmd = commandReader.read_next_cmd();
            BOOST_CHECK(cmd.cmd_type == CommandType::Base);
            BOOST_CHECK(cmd.data == "cmd{");
        }
        {
            stringstream in;
            in << "cmd}\n";
            StreamCmdReader commandReader(in);
            auto cmd = commandReader.read_next_cmd();
            BOOST_CHECK(cmd.cmd_type == CommandType::Base);
            BOOST_CHECK(cmd.data == "cmd}");
        }
    }

    BOOST_AUTO_TEST_CASE(test_Custom) {
        {
            stringstream in;
            in << "{\n";
            StreamCmdReader commandReader(in);
            auto cmd = commandReader.read_next_cmd();
            BOOST_CHECK(cmd.cmd_type == CommandType::StartCustomBulk);
        }
        {
            stringstream in;
            in << "}\n";
            StreamCmdReader commandReader(in);
            auto cmd = commandReader.read_next_cmd();
            BOOST_CHECK(cmd.cmd_type == CommandType::StopCustomBulk);
        }
    }

BOOST_AUTO_TEST_SUITE_END()