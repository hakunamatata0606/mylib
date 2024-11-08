#pragma once

#include <spdlog/logger.h>
#include <mutex>

namespace mylib
{

class logger
{
public:
    static std::shared_ptr<spdlog::logger> getInstance();

private:
    static std::once_flag _flag;
};

}