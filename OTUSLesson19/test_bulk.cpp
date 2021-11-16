#define BOOST_TEST_MODULE bulkmt_test_module
#include <boost/test/unit_test.hpp>

#include <memory>
#include <sstream>
#include <thread>
#include <chrono>

#include "bulk.h"
#include "command_reader.h"
#include "command_handler.h"
#include "command_processor.h"
#include "simple_math.h"
#include "thread_pool.h"

using namespace std;
using namespace std::chrono_literals;

BOOST_AUTO_TEST_SUITE(math_test_suite)

    BOOST_AUTO_TEST_CASE(test_factorial) {
        BOOST_CHECK(factorial(0) == 1);
        BOOST_CHECK(factorial(1) == 1);
        BOOST_CHECK(factorial(2) == 2);
        BOOST_CHECK(factorial(3) == 6);
        BOOST_CHECK(factorial(4) == 24);
        BOOST_CHECK(factorial(5) == 120);
        BOOST_CHECK(factorial(6) == 720);
        BOOST_CHECK(factorial(7) == 5040);
        BOOST_CHECK(factorial(8) == 40'320);
        BOOST_CHECK(factorial(9) == 362'880);
        BOOST_CHECK(factorial(10) == 3'628'800);
    }

    BOOST_AUTO_TEST_CASE(test_fibonacci) {
        BOOST_CHECK(fibonacci(0) == 0);
        BOOST_CHECK(fibonacci(1) == 1);
        BOOST_CHECK(fibonacci(2) == 1);
        BOOST_CHECK(fibonacci(3) == 2);
        BOOST_CHECK(fibonacci(4) == 3);
        BOOST_CHECK(fibonacci(5) == 5);
        BOOST_CHECK(fibonacci(6) == 8);
        BOOST_CHECK(fibonacci(7) == 13);
        BOOST_CHECK(fibonacci(8) == 21);
        BOOST_CHECK(fibonacci(9) == 34);
        BOOST_CHECK(fibonacci(10) == 55);
        BOOST_CHECK(fibonacci(11) == 89);
        BOOST_CHECK(fibonacci(12) == 144);
        BOOST_CHECK(fibonacci(13) == 233);
        BOOST_CHECK(fibonacci(14) == 377);
        BOOST_CHECK(fibonacci(40) == 102334155);
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(bulk_whole_test_suite)

	BOOST_AUTO_TEST_CASE(test_1) {
        constexpr int N = 3;
        stringstream in;
        in << "1\n" << "2\n" << "3\n";
        in << "4\n" << "5\n";
        stringstream out;
        {
            auto bulkMgr = make_unique<BulkCmdManager>(N);
            auto commandReader = make_unique<StreamCmdReader>(in);

            createObserverAndSubscribe<CmdStreamHandler>(bulkMgr.get(), out);
            process_all_commands(commandReader.get(), bulkMgr.get());
        }
        string res = "bulk: 1, 2, 6\nbulk: 24, 120\n";
        BOOST_CHECK(out.str() == res);
	}

    BOOST_AUTO_TEST_CASE(test_2) {
        constexpr int N = 3;
        stringstream in;
        in << "1\n" << "2\n" << "{\n"
           << "3\n" << "4\n" << "5\n" << "6\n" << "7\n" << "}\n";
        stringstream out;
        {
            auto bulkMgr = make_unique<BulkCmdManager>(N);
            auto commandReader = make_unique<StreamCmdReader>(in);
            createObserverAndSubscribe<CmdStreamHandler>(bulkMgr.get(), out);

            // start commands cycle
            process_all_commands(commandReader.get(), bulkMgr.get());
        }
        string res = "bulk: 1, 2\nbulk: 6, 24, 120, 720, 5040\n";
        BOOST_CHECK(out.str() == res);
    }

    BOOST_AUTO_TEST_CASE(test_3) {
        constexpr int N = 3;
        stringstream in;
        in << "{\n" << "1\n" << "2\n"
           << "{\n" << "3\n" << "4\n" << "}\n"
           << "5\n" << "6\n" << "}\n";
        stringstream out;
        {
            auto bulkMgr = make_unique<BulkCmdManager>(N);
            auto commandReader = make_unique<StreamCmdReader>(in);
            createObserverAndSubscribe<CmdStreamHandler>(bulkMgr.get(), out);

            // start commands cycle
            process_all_commands(commandReader.get(), bulkMgr.get());
        }
        string res = "bulk: 1, 2, 6, 24, 120, 720\n";
        BOOST_CHECK(out.str() == res);
    }

    BOOST_AUTO_TEST_CASE(test_4) {
        constexpr int N = 3;
        stringstream in;
        in << "1\n" << "2\n" << "3\n"
           << "{\n" << "3\n" << "4\n" << "5\n" << "6\n" << "7\n";
        stringstream out;
        {
            auto bulkMgr = make_unique<BulkCmdManager>(N);
            auto commandReader = make_unique<StreamCmdReader>(in);
            createObserverAndSubscribe<CmdStreamHandler>(bulkMgr.get(), out);

            // start commands cycle
            process_all_commands(commandReader.get(), bulkMgr.get());
        }

        string res = "bulk: 1, 2, 6\n";
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
            BOOST_CHECK(cmd.data.empty());
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



BOOST_AUTO_TEST_SUITE(thread_pool_test_suite)

    BOOST_AUTO_TEST_CASE(test_thread_pool) {
        {
            stringstream out;
            auto foo = [](stringstream& out, const string& a, const string& b){
                out << a << " " << b << endl;
                return size_t(1);
            };
            ThreadPool deadPool(2, "dead_pool");
            deadPool.addTask(foo, ref(out), string("a"), string("b"));
            deadPool.addTask(foo, ref(out), "c", "d");
            this_thread::sleep_for(400ms);
            BOOST_CHECK(out.str() == "a b\nc d\n");
        }
    }

BOOST_AUTO_TEST_SUITE_END()