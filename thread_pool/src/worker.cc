#include "worker.h"

#include "impl/task.h"
#include "task_queue.h"
#include "logger.h"

namespace mylib
{



worker::worker(std::weak_ptr<thread_pool_impl> pool, std::shared_ptr<task_queue> tasks, unsigned id)
    : _pool(pool), _tasks(tasks), _id(id), _stopped(false), _thread()
{
    static auto logger = logger::getInstance();

    logger.get()->debug("worker::worker(): id={}", _id);
}


worker::~worker()
{
    static auto logger = logger::getInstance();

    logger.get()->debug("worker::~worker(): id={}", _id);
    _stopped = true;
    _tasks.get()->stop();
    if (_thread.joinable()) {
        _thread.join();
    }
}

void worker::start()
{
    _thread = std::thread(worker::thread_func, this);
}

void worker::thread_func(worker* wrk)
{
    static auto logger = logger::getInstance();

    logger.get()->debug("worker_impl::thread_func(): started, id: {}", wrk->_id);
    while (!wrk->_stopped) {
        task_ptr_t t = wrk->_tasks.get()->get_task();
        t.get()->execute();
    }

    logger.get()->debug("worker_impl::thread_func(): stopped, id: {}", wrk->_id);
}

}// namespace mylib