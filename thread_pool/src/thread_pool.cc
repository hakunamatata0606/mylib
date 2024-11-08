#include "thread_pool.h"

#include "thread_pool_impl.h"
#include "worker.h"
#include "task_queue.h"
#include "logger.h"
#include "impl/task.h"

namespace mylib
{

const unsigned thread_pool::MAX_WORKER = 3;


thread_pool_impl::thread_pool_impl(unsigned num_worker)
    : _num_worker(num_worker), _started(false), _workers(), _task_queue(std::make_shared<task_queue>())
{
    static auto logger = logger::getInstance();

    logger.get()->debug("thread_pool_impl::thread_pool_impl(): num_worker={}", num_worker);
}

std::shared_ptr<thread_pool_impl> thread_pool_impl::create(unsigned num_worker)
{
    return std::shared_ptr<thread_pool_impl>(new thread_pool_impl(num_worker));
}

thread_pool_impl::~thread_pool_impl()
{
    static auto logger = logger::getInstance();

    logger.get()->debug("thread_pool_impl::~thread_pool_impl(): ");
}

void thread_pool_impl::start()
{
    static auto logger = logger::getInstance();

    logger.get()->debug("thread_pool_impl::start():");
    _started = true;
    for (unsigned i = 0; i < _num_worker; i++) {
        worker_ptr_t w = std::make_unique<worker>(weak_from_this(), _task_queue, i);
        w.get()->start();
        _workers.push_back(std::move(w));
    }
}

void thread_pool_impl::add_task(handle_t handle)
{
    static auto logger = logger::getInstance();

    logger.get()->debug("thread_pool_impl::add_task():");
    class basic_task : public task
    {
    public:
        basic_task(handle_t h)
            : _h(h)
        {
        }
        
        void execute()
        {
            static auto logger = logger::getInstance();
            logger.get()->debug("basic_task::execute():");
            if (_h) {
                _h();
            }
        }
    private:
        handle_t _h;
    };
    task_ptr_t t = task_ptr_t(new basic_task(handle));
    add_task(std::move(t));
}

void thread_pool_impl::add_task(std::unique_ptr<task> t)
{
    _task_queue.get()->add_task(std::move(t));
}

thread_pool::thread_pool(unsigned num_worker)
    : _impl(thread_pool_impl::create(num_worker))
{
    static auto logger = logger::getInstance();

    logger.get()->debug("thread_pool::thread_pool():");
}

thread_pool::~thread_pool()
{
    static auto logger = logger::getInstance();

    logger.get()->debug("thread_pool::~thread_pool():");
}

void thread_pool::start()
{
    static auto logger = logger::getInstance();

    logger.get()->debug("thread_pool::start():");
    _impl.get()->start();
}

void thread_pool::add_task(std::unique_ptr<task> t)
{
    static auto logger = logger::getInstance();

    logger.get()->debug("thread_pool::add_task():");
    _impl.get()->add_task(std::move(t));
}

}//namespace mylib