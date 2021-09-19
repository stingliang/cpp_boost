# atomic

`atomic`实现了C++标准定义的原子操作库(C++11.29)，它封装了不同计算机硬件的底层操作原语，提供了跨平台的原子操作功能，可以把对普通的POD类型的操作原子化

## 头文件和命名空间

需要链接`boost_atomic`库使用

```c++
#include <boost/array.hpp> 
using namespace boost;
```

## 用法

可以用两种方式创建atomic对象：

- 有参数的构造函数创建有初值的`atomic`
- 无参数的默认构造函数创建一个初值不确定的`atomic`(这种方式很危险，我们应当尽量避免)

```c++
boost::atomic<int> i(10);
BOOST_CHECK_EQUAL(i, 10);
// should not be used
// boost::atomic<double> d;
```

atomic最重要的两个成员函数是`store()`和load()，它们以原子的方式存取atomic内部的值，不会因并发访问导致数据不一致

atomic还使用操作符重载简化了`store()`和load()的调用方式

- 赋值等价于`store()`
- 隐式类型转换则等价于`load()`

```c++
boost::atomic_int i(10);
i += 1;
BOOST_CHECK(i == 11);
i.store(0);
BOOST_CHECK_EQUAL(i.load(), 0);
```

