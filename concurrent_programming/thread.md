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

