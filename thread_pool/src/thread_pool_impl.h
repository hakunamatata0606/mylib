#pragma once

#include <list>
#include <memory>
#include "thread_pool.h"
#include "impl/task.h"

namespace mylib
{

typedef void(*handle_t)();

class worker;

class task_queue;


class thread_pool_impl : public std::enable_shared_from_this<thread_pool_impl>
{
    typedef std::unique_ptr<worker> worker_ptr_t;
    typedef std::list<worker_ptr_t> worker_list_t;

    explicit thread_pool_impl(unsigned num_worker);
public:

    ~thread_pool_impl();

    static std::shared_ptr<thread_pool_impl> create(unsigned num_worker);

    void start();

    void add_task(handle_t handle);

    void add_task(std::unique_ptr<task> t);

private:
    unsigned _num_worker;
    
    bool _started;

    worker_list_t _workers;

    std::shared_ptr<task_queue> _task_queue;
};

}// namespace mylib