# thread

thread库为C++增加了线程处理的能力，它提供简明清晰的互斥量、线程、条件变量等概念，用它可以很容易地创建多线程应用程序

thread库是高度可移植的库，它兼容C++标准，支持使用最广泛的POSIX线程和Windows线程，用它编写的代码无须修改就可以在UNIX、Windows等操作系统上编译运行

## 头文件和命名空间

需要连接`boost_chrono`库使用

```c++
#include <boost/thread.hpp>
#include <boost/thread/with_lock_guard.hpp>
using namespace boost;
```

## mutex

介绍两种最基本的互斥量

### boost::mutex

`boost::mutex`提供最基本的`lock()`和`unclock()`操作

```c++
boost::mutex mu;
mu.lock();
//  critical resource
mu.unlock();
```

### boost::timed_mutex

`boost::timed_mutex`提供锁定超时功能，防止进程因为等待锁而长时间阻塞

```c++
boost::timed_mutex mu;
auto flag = mu.try_lock_for(boost::chrono::milliseconds(500));
if(flag) {
    //  critical resource
    mu.unlock();
}
```

### boost::lock_guard

thread库提供RAII型的`lock_guard`类用于辅助锁定互斥量

它在构造时锁定互斥量，在析构时自动解锁，从而保证了互斥量的正确操作，避免遗忘解锁，它就像一个智能指针

```c++
boost::mutex mu;
{
    boost::lock_guard<boost::mutex> g(mu);
    //  critical resource
}
```

thread库还提供一个便捷函数`with_lock_guard()`，它可以使用`lock_guard`锁定互斥量执行一个函数，简化`lock_guard`的调用

```c++
boost::mutex mu;
boost::with_lock_guard(mu, [](){
    //  critical resource
});
```

## 线程对象

thread类实现了操作系统里的线程表示，负责启动和管理线程对象，它在概念和操作上都和POSIX线程相似，其类摘要如下

```c++
class thread {    // 线程对象，不可拷贝
public:
    thread();
    explicit thread(F f);    // 传递可调用对象
    thread(F f, A1 a1, A2 a2,...)    // 传递可调用对象及参数
        
    thread(thread&&) noexcept;	// 转移构造函数
    thread& operator=(thread&&) noexcept;	// 转义赋值函数
    
    bool joinable() const;	// 是否可join
    void join();	// 等待线程
    
    void detach;	// 分离线程
    
    bool try_join_for(const duration& rel_time);	// 超时等待
    bool try_join_until(const time_point& t);	// 超时等待
    
    void interrupt();	// 中断线程
    bool interruption_requested() const;	// 是否被中断
    
    class id;	// 内部类，用于表示线程ID
    id get_id() const;	// 获得线程id对象
    native_handle_type native_handle();	// 获得系统相关的操作handle
    
    static unsigned hardware_concurrency();	// 静态成员，获取可并发的核心数
    static unsigned physical_concurrency();	// 静态成员，获取真实CPU的核心数
};

namespace this_thread {
    thread::id get_id();	// 获得当前线程的id对象
    void yield();	// 运行重新调度当前线程
    void sleep_until(const time_point& t);	// 睡眠等待
    void sleep_for(const duration& d);	// 睡眠等待
}
```

> 使用boost库的好处之一就是可以很方便的开发跨平台代码，比如在Linux和Windows下的线程概念是不一样的，包含的头文件也不一样，可以考虑使用boost的thread对象来代替传统标准库中的thread，这样编写的包含现场的代码可以很好的在不同平台上编译

thread对象是不可拷贝、不可比较的，但它支持转移（move）语义，有转移构造函数和转移赋值函数，所以它可以从工厂函数产生。

成员函数get_id()可以返回thread::id对象，它提供了完整的比较操作符和流输出操作符，唯一地表示了thread对象

```c++
boost::thread t1, t2;
BOOST_TEST_MESSAGE(t1.get_id());
BOOST_CHECK_EQUAL(t1.get_id(), t2.get_id());
```

另有4个自由函数get_id()、yield()、sleep_for()、sleep_until()，它们无需使用thread对象就可以操作当前线程，但它们位于子名字空间boost::this_thread,而不是thread的静态成员函数
> TIPS
> 你可以定义自己的字面值时间常量，例如：
> ```c++
> boost::chrono::seconds operator"" _s(unsigned long long n) { return boost::chrono::seconds(n); }
> ```
```c++
// 5s为C++14定义的字面值常量，5_s为自定义的字面值常量
using namespace std::chrono_literals;
std::this_thread::sleep_for(5s);    // C++ 标准库用法
boost::this_thread::sleep_for(5_s);     // Boost库用法
```

