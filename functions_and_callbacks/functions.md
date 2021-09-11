# functions

functions是一种函数对象的“容器“，是一种智能函数指针。它以对象方式封装了函数对象，能够容纳任意符合函数签名的可调用对象。因此它可以很方便的被用于回调机制，暂时保管函数或函数对象，在之后需要时再调用这些函数或函数对象，使回调机制拥有更多的弹性。

## 头文件和命名空间

```c++
#include <boost/function.hpp>
using namespace boost;
```

## 用法

这里需要说明的是，除了使用functions库以外，更好的解决方案可能是使用C++11以后支持的lambda匿名函数，所以这里只对function库做简单介绍。

```c++
// 函数举例
f(int a, int b) { return a + b; }
```

### 声明一个function对象

```c++
// 简单声明一个function
boost::function<int()> func;
// 此时func不持有任何对象c++
BOOST_CHECK(!func);
 // 使用decltype直接推到参数类型
boost::function<decltype(f)> func2;
```

### function基本操作

```c++
// 验证已存储函数f
func2 = f;
BOOST_CHECK(func2.contains(&f));

// 直接转换为bool判断是否包含一个函数对象
BOOST_CHECK(func2);

// 清空func2
func2.clear();
BOOST_CHECK(func2.empty());

// 配合bind存储成员函数c++
FuncTest ft;
boost::function<int(int, int)> func3;
func3 = boost::bind(&FuncTest::memberFunc, &ft, _1, _2);
BOOST_CHECK_EQUAL(func3(1, 2), 3);
```