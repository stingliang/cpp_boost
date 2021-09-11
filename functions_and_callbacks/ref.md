# ref

C++标准库和Boost中的算法大量使用了函数对象作为判断式或谓词参数，而这些参数都是传值语义，算法或函数在内部保留函数对象的拷贝并使用。

例如：

```c++
struct square {
        typedef void result_type;
        void operator() (int& x) {
            x *= x;
        }
    };

   std::vector<int> v = {1, 2, 3, 4, 5};
    for (auto& elem: v) {
        square()(elem);
    }
```

一般情况下，传值语义都是可行的，但也有很多特殊情况：作为参数的函数对象拷贝代价过高（具有复杂的内部状态），不希望拷贝对象（内部状态不应该被改变），甚至拷贝是不可行的（noncopyable、singleton）。

## 头文件和命名空间

```c++
#include <boost/ref.hpp>
using namespace boost;
```

## 基本用法

ref库定义了一个很小、很简单的引用类型的包装器，名字叫reference_wrapper，其基本用法如下

```c++
// reference_wrapper的基本用法
int x = 10;
// 包装int类型的引用
boost::reference_wrapper<int> rw(x);
// 隐式类型转换
BOOST_CHECK_EQUAL(x, rw);
// 显式类型转换
(int&)rw = 100; 
// 隐式类型转换
BOOST_CHECK_EQUAL(x, 100);
// 拷贝构造
boost::reference_wrapper<int> rw_copy(rw);
BOOST_CHECK_EQUAL(rw_copy.get(), 100);
// 指针用法
std::string str("test reference wrapper");
boost::reference_wrapper<std::string> rws(str);
BOOST_CHECK_EQUAL(*rws.get_pointer(), str);
BOOST_CHECK_EQUAL(rws.get().size(), str.size());
```

reference_wrapper的用法有些类似C++中的引用类型（T&），它就像被包装对象的一个别名。但它只有在使用T的语境下才能够执行隐式转换，其他的情况下则需要调用类型转换函数或get（）函数才能真正地操作被包装对象。

此外，reference_wrapper支持拷贝构造和赋值，而引用类型是不可赋值的。

## 工厂函数

reference_wrapper的名字过长，用它声明包装对象很不方便，因此ref库提供了两个便捷的工厂函数ref（）和cref（），利用它们可以通过推导参数类型很容易地构造包装对象。这两个函数的声明如下：

```c++
reference_wrapper<T> ref(T& t);
reference_wrapper<T const> ref(T const& t);
```

ref()和cref()的区别仅仅在于产生的类型是否为const

```c++
// 工厂函数
double x = 2.71828;
auto rw = boost::cref(x);
BOOST_CHECK_EQUAL(typeid(rw).name(), "class boost::reference_wrapper<double const >");

auto rws = boost::ref(str);
BOOST_CHECK_EQUAL(typeid(rws).name(),
                 "class boost::reference_wrapper"
                 "<class std::basic_string<char,struct std::char_traits<char>,"
                 "class std::allocator<char> > >");
```