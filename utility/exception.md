## exception

### 作用

配合C++标准库中的异常处理模式，对其进行了强化，包含在以下头文件

```c++
#include <boost/exception/all.hpp>
using namespace boost;
```

### boost异常

`boost::exception`提供两个类：`exception`和`error_info`。他们是exception库的基础。

#### exception

其类摘要如下：

```c++
class exception
{
protected:
    exception();
    exception(exception const & x);
    

    virtual ~exception();

private:
    template<class E, class Tag, class T>
    friend E const & operator<<(E const &, error_info<Tag, T> const &);
};
```

exception几乎没有公开的成员函数，它是一个抽象类。`exception`的重要能力在于其友元操作符`<<`，它可以存储`error_info`对象的信息，存入的信息可以由自由函数`get_error_info()`随时取出。

#### erro_info

其类摘要如下

```c++
template <class Tag, class T>
class error_info
{
public:
    typedef T value_type;
    

    error_info(value_type const & v);
    value_type & value();

};
```

`error_info`提供了向异常类型添加信息的通用解法。第一个模板类型参数Tag是一个标记，我们已经在前面见过了类似的用法，它通常（最好）是一个空类，仅用来标记`error_info`类，使它在模板实例化时生成不同的类型。第二个模板类型参数T是真正存储的信息数据，可以用成员函数`value()`访问。

### 向异常传递信息

`exception`和`error_info`被设计为配合`std：：exception`一起工作，自定义的异常类可以安全地从exception和`std：：exception`多重继承，从而获得两者的能力。
因为`exception`被定义为抽象类，所以我们的程序必须定义它的子类才能使用它，如前面所述，exception必须使用虚继承的方式。通常，继承完成后自定义异常类的实现也就结束了，不需要“画蛇添足”地向它增加成员变量或成员函数，这些工作都已经由`exception`完成了。例如：

```c++
struct my_exception : virtual std::exception, virtual boost::exception
{};
```

用一个struct作为第一个模板参数来标记信息类型，再用第二个模板参数指定信息的数据类型。由于error_info的类型定义较长，为了使用方便，通常需要使用typedef。
下面的代码使用`erro_info`定义了两个存储`<int>`和`<string>`的信息类：

```c++
using err_no = boost::erro_info<struct tag_err_no, int>;
using err_str = boost::erro_info<struct tag_err_str, string>;
```

当发生异常时，我们就可以创建一个自定义异常类，并用操作符“＜＜”向它存储任意信息，这些信息可以在任何时候使用`get_error_info（）`函数提取。示范`exception`用法的代码如下：

```c++
// 定义异常类
struct my_exception : virtual std::exception, virtual exception {};
// 异常信息类型
using err_no = error_info<struct tag_err_no, int>;
using err_str = error_info<struct tag_err_str, string>;

try
{
    try
    {
        // 抛出异常，存储错误码
        throw my_exception() << err_no(10);
    }
    catch(my_exception& e)
    {
        // 获得异常内存储的信息
        cout << *get_error_info<err_no>(e) << endl;
        cout << e.what() << endl;
        e << err_str("other info");  // 向异常追加信息
        throw;  // 再次抛出异常
    }
}
catch(my_exception& e)
{
    cout << *get_error_info<err_str>(e) << endl;
}
```

### 错误信息类

exception库特意提供若干预先定义好的错误信息类，如同标准库定义的`logic_err`等类型。

```c++
typedef erro_info<...> errinfo_api_function;
typedef erro_info<...> errinfo_at_line;
typedef erro_info<...> errinfo_errno;
typedef erro_info<...> errinfo_file_name;
typedef erro_info<...> errinfo_file_open_mode;

用法示例：
try
{
    throw my_exception() << errinfo_api_function("call api")
                         << errinfo_errno(101);
}
catch(boost::exception& e)
{
    cout << *get_error_info<errinfo_api_function>(e);
    cout << *get_error_info<errinfo_errno>(e);
}
```

此外，还提供3个预定义错误信息类型

```c++
typedef erro_info<...> throw_function;
typedef erro_info<...> throw_file;
typedef erro_info<...> throw_line;
```

这3个错误信息类型主要用于存储源代码的信息，配合宏`BOOST_CURRENT_FUNCTION`（参见boost::assert部分）、`__FILE__`和`__LINE__`使用，获得调用函数名、源文件名和源代码行号。