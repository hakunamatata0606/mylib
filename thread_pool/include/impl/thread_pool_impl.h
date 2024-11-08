#pragma once

#include <mutex>
#include <condition_variable>
#include <exception>
#include "task.h"

namespace mylib
{
namespace detail {

template<typename Func, typename R>
class result_impl_helper
{
public:
    result_impl_helper(const Func& f)
        : _f(f), _executed(false), _e(nullptr)
    {
    }

    R get()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cond.wait(lock, [&] {
            return _executed;
        });
        if (_e) {
            std::rethrow_exception(_e);
        }
        return _res;
    }

    void execute()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        try {
            execute_internal();
        }catch(const std::exception& e) {
            _e = std::current_exception();
        }
        
        _executed = true;
        _cond.notify_one();
    }

protected:
    virtual void execute_internal() = 0;

    void execute_with_result()
    {
        _res = _f();
    }

    void execute_without_result()
    {
        _f();
    }

    R _res;
    Func _f;
private:
    
    bool _executed;
    std::mutex _mutex;
    std::condition_variable _cond;
    std::exception_ptr _e;

    friend class result<Func, R>;

    friend class thread_pool;
};

template<typename Func, typename R>
class result_impl<Func, R, false> : public result_impl_helper<Func, R>
{
public:
    result_impl(const Func& f)
        : result_impl_helper<Func, R>(f)
    {
    }
private:
    void execute_internal()
    {
        result_impl_helper<Func, R>::execute_with_result();
    }
};

template<typename Func, typename R>
class result_impl<Func, R, true> : public result_impl_helper<Func, R>
{
public:
    result_impl(const Func& f)
        : result_impl_helper<Func, R>(f)
    {
    }
private:
    void execute_internal()
    {
        result_impl_helper<Func, R>::execute_without_result();
    }
};

template<typename Result>
class task_wrapper : public task
{
public:
    task_wrapper(std::shared_ptr<Result> res)
        : _res(res)
    {

    }

    void execute()
    {
        _res.get()->execute();
    }

private:
    std::shared_ptr<Result> _res;
};

}//namespace detail

template<typename Func, typename R>
result<Func, R>::result(const Func& f)
    : _impl(new detail::result_impl<Func, R>(f))
{
}


template<typename Func, typename R>
result<Func, R>::~result()
{
}

template<typename Func, typename R>
R result<Func, R>::get()
{
    return _impl.get()->get();
}

template<typename Func, typename ... Args>
auto thread_pool::add_task(Func&& f, Args&&... args)
{
    auto wrap_func = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
    typedef decltype(f(args...)) RT;
    typedef std::conditional_t<std::is_same_v<RT, void>, void_t, RT> R;
    typedef result<decltype(wrap_func), R> result_t;
    typedef std::shared_ptr<result_t> result_ptr_t;
    typedef detail::task_wrapper<detail::result_impl<decltype(wrap_func), R>> wrapper_t;

    auto res = result_ptr_t(new result_t(wrap_func));
    auto res_task = std::unique_ptr<task>(new wrapper_t(res.get()->_impl));
    add_task(std::move(res_task));
    return res;
}

}//namespace mylib

inline std::ostream& operator<<(std::ostream& os, const mylib::void_t&)
{
    return os;
}