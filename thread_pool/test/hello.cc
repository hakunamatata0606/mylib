#include <iostream>
#include <thread_pool.h>
#include <thread>

using namespace mylib;

int foo(int i, const std::string& s) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "FOOOOOOOO " << s << "\n";
    return i;
}

int main()
{
    thread_pool p;
    std::cout << "Main thread: " << std::this_thread::get_id() << "\n";
    p.start();
    // p.add_task([] {
    //     std::cout << "In task: " << std::this_thread::get_id() << "\n";
    // });
    auto a1 = p.add_task(foo, 5, "dummy1");
    auto a2 = p.add_task(foo, 10, "dummy2");
    auto a3 = p.add_task([]{
        foo(25, "dummy3");
    });
    auto a4 = p.add_task([]{
        return foo(12, "dummy4");
    });
    auto i1 = a1.get()->get();
    auto i2 = a2.get()->get();
    auto i3 = a3.get()->get();
    auto i4 = a4.get()->get();
    std::cout << "GOT: i1 " << i1 << "\n";
    std::cout << "GOT: i2 " << i2 << "\n";
    std::cout << "GOT: i3 " << i3 << "\n";
    std::cout << "GOT: i4 " << i4 << "\n";
    
    return 0;
}