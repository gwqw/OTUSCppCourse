#include "command_handler.h"

#include <fstream>
#include <atomic>

#include "simple_math.h"

using namespace std;

std::atomic<int> CmdFileHandler::counter_ = 0;

void CmdStreamHandler::update(BulkCmdHolder bulk) {
    out_ << "bulk: ";
    bool is_first = true;
    for (const auto& c : bulk->data_) {
        if (is_first) {
            is_first = false;
        } else {
            out_ << ", ";
        }
        try {
            int n = stol(c.data);
            out_ << factorial(n);
        } catch (const exception& e) {
            out_ << "Error while converting " << c.data << " to int and calc Factorial";
        }
    }
    out_ << std::endl;
}

void CmdFileHandler::update(BulkCmdHolder bulk) {
    string filename = getFileName(*bulk);
    ofstream out(filename);
    if (out) {
        for (const auto &c : bulk->data_) {
            try {
                int n = stol(c.data);
                out << fibonacci(n) << '\n';
            } catch (const exception& e) {
                out << "Error while converting " << c.data << " to int and calc Fibonacci" << endl;
            }
        }
    } else {
        cerr << "Cannot create file " << filename << endl;
    }
}

std::string CmdFileHandler::getFileName(const BulkCmd& bulk) {
    return "bulk" + to_string(bulk.time_)
        + "_" + to_string(counter_++)
        + ".log";
}
