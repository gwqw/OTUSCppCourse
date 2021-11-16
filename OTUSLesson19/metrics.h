#pragma once

#include <cstddef>
#include <ostream>

struct MainMetric {
    std::size_t line_num = 0;
    std::size_t command_num = 0;
    std::size_t block_num = 0;
};

std::ostream& operator<<(std::ostream& out, const MainMetric& metric);

struct ThreadMetric {
    std::size_t command_num = 0;
    std::size_t block_num = 0;
};

std::ostream& operator<<(std::ostream& out, const ThreadMetric& metric);
