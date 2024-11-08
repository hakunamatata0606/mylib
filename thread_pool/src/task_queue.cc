#include "task_queue.h"

namespace mylib
{

class DummyTask : public task
{
public:
    void execute() {}
};

task_queue::task_queue()
    : _stopped(false)
{

}

task_queue::~task_queue()
{

}

void task_queue::add_task(task_ptr_t t)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _tasks.push_back(std::move(t));
    _cond.notify_one();
}

task_ptr_t task_queue::get_task()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _cond.wait(lock, [&] {return (_tasks.size() > 0 || _stopped);});
    if (_stopped) {
        return task_ptr_t(new DummyTask());
    }
    task_ptr_t t = std::move(_tasks.front());
    _tasks.pop_front();
    return t;
}

void task_queue::stop()
{
    if (_stopped) {
        return;
    }
    _stopped = true;
    _cond.notify_all();
}

}// namespace mylib