#pragma once
/**@file
    @brief command reader

    Command reader interface and implemetations
*/

#include <iostream>

#include "command.h"

/**
 *  @brief Command reader
 *
 *  Interface for command reader.
 */
class ICmdReader {
public:
    virtual ~ICmdReader() = default;
    virtual Command read_next_cmd() = 0;
};

/**
 *  @brief Command reader from input stream
 *
 *  Command reader implementation. Obtains stream in ctor and reads command, separated by eol
 */
class StreamCmdReader : public ICmdReader{
public:
    explicit StreamCmdReader(std::istream& in = std::cin) : in_(in) {}
    Command read_next_cmd() override;
private:
    std::istream& in_;
};