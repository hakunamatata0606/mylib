#pragma once

#include <memory>
#include <functional>

namespace mylib
{

class thread_pool_impl;
class thread_pool;
class task;

struct dummy {};
typedef dummy void_t;

namespace detail {

template<typename Func, typename R, bool B = std::is_same_v<R, void_t>>
class result_impl;

template<typename Result>
class task_wrapper;

}//namespace detail


typedef void(*handle_t)();


template<typename Func, typename R>
class result
{
public:
    result(const Func& f);

    ~result();

    R get();

private:
    std::shared_ptr<detail::result_impl<Func, R>> _impl;

    friend class thread_pool;
};

class thread_pool
{
    static const unsigned MAX_WORKER;
public:
    thread_pool(unsigned num_worker = MAX_WORKER);

    virtual ~thread_pool();

    void start();

    template<typename Func, typename ... Args>
    auto add_task(Func&& f, Args&&... args);
    

private:
    void add_task(std::unique_ptr<task> t);

    std::shared_ptr<thread_pool_impl> _impl;

    friend class thread_pool_impl;
};

}// namespace mylib

std::ostream& operator<<(std::ostream& os, const mylib::void_t&);

#include "impl/thread_pool_impl.h"