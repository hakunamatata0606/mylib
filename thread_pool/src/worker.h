#pragma once

#include <memory>
#include <thread>


namespace mylib
{

class thread_pool_impl;

class task_queue;

class worker
{
public:
    worker(std::weak_ptr<thread_pool_impl> pool, std::shared_ptr<task_queue> tasks, unsigned id);

    worker(const worker&) = delete;

    ~worker();

    void start();

private:
    static void thread_func(worker* wrk);

private:
    std::weak_ptr<thread_pool_impl> _pool;

    std::shared_ptr<task_queue> _tasks;

    unsigned _id;

    bool _stopped;

    std::thread _thread;
};

}// namespace mylib