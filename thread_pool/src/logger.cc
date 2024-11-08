#include "logger.h"

#include <spdlog/sinks/stdout_sinks.h>

namespace mylib
{

std::once_flag logger::_flag;

std::shared_ptr<spdlog::logger> logger::getInstance()
{
    static auto instance = spdlog::stdout_logger_mt("thread_pool_logger");
    std::call_once(_flag, [&]{
        instance.get()->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l]  %v");
        instance.get()->set_level(spdlog::level::debug);
    });
    return instance;
}

}