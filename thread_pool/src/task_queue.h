#pragma once

#include "impl/task.h"
#include <deque>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace mylib
{

typedef std::unique_ptr<task> task_ptr_t;

class task_queue : std::enable_shared_from_this<task_queue>
{
public:
    task_queue();

    task_queue(const task_queue&) = delete;

    ~task_queue();

    void add_task(task_ptr_t t);

    task_ptr_t get_task();

    void stop();

private:
    std::deque<task_ptr_t> _tasks;

    std::mutex _mutex;

    std::condition_variable _cond;

    bool _stopped;
};

} // namespace mylib
