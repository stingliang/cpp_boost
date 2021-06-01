## Utility

### noncopyable

#### 功能

noncopyable允许轻松实现一个禁止拷贝的类

需要包含以下头文件：

```c++
#include <boost/noncopyable.hpp>
```

#### 原理

在C++中，定义一个类时，如果不明确定义拷贝构造函数和拷贝赋值操作符，编译器会自动生成这两个函数

```c++
// 举例
class empty_class{};
// 编译器自动添加拷贝构造函数和拷贝赋值操作符
class empty_class
{
public:
    empty_class (const empty_class&){...}    // 拷贝构造
    empty_class& operator=(const empty_class&){...}    // 拷贝赋值
};
```

但是有时候不需要类的拷贝语义，希望禁止拷贝实例，一般的手法是将拷贝构造函数和赋值操作符“私有化”

```c++
class do_not_copy
{
private:
    do_not_copy(const do_not_copy&);    // 声明即可不需要实现
    void operator=(const do_not_copy&);
}
```

但是这样写明显会重复很多代码，且不够规范

#### 用法

为实现不可拷贝类，使用如下解决方案，既从`boost::noncopyable`派生即可，上面的例子可简化为

```c++
class do_not_copy:boost::noncopyable
{...}
```

#### 实现原理

其实现原理很简单，代码很少

```c++
class noncopyable
{
protected:
    noncopyable();
    ~noncopyable();
private:
    noncopyable(const noncopyable);
    const noncopyable& operator=(const noncopyable&);
};
```

因此，当定义的类是noncopyable的派生类时，会自动私有化父类的拷贝构造函数，从而实现禁止拷贝

***

### ignore_unused

#### 功能

编写代码的过程中有时会出现一些暂时用不到但又必须保留的变量，GCC等编译器会对此发出警告，使用`-Wunused`可以关闭这些警告消息，不过这也有可能导致潜在的隐患。古老的办法是使用`(void)var`的形式来“使用”一下变量，但这种方法含义不明确，不利于维护。

Boost程序库的ignore_unused组件就这个问题给出了更好的解决方案。它原本是proto库里的一个小工具，因为在很多其他组件中被使用，所以在增强了功能后被“扶正”。

`ignore_unused`位于名字空间`boost`，需要包含的头文件如下：

```c++
#include <boost/core/ignore_unused.hpp> 
using namespace boost;
```

#### 用法

ignore_unused使用可变参数模板，支持任意数量、任意类型的变量，把它们作为函数的参数“使用”了一下，从而“骗”过编译器，达到与`(void)var`完全相同的效果。但它的命名更清晰，写法也更简单，而且由于使用的是inline函数，所以运行时完全没有效率损失。

这样做能够提高编码时的调试效率

```c++
int func(int x, int y)
{
    int i;
    return y;
    // 未使用变量i和x
}
```

改为

```c++
int func(int x, int y)
{
    int i;
    ignore_unused(i, x);
    return y;
    // 未使用变量i和x
}
```

***

### optional

#### 功能

在实际的软件开发过程中我们经常会遇到无效值的情况。例如，函数并不是总能返回有效值，很多时候即使函数正确执行，但其结果却不是合理的值。如果用数学语言来解释，这种情况就是返回值位于函数解空间之外。

求一个数的倒数、在实数域内开平方、在字符串中查找子串，它们都可能返回无效值。有些无效返回的情况可以用抛出异常的方式来通知用户，但在某些情况下，这样代价很高或不允许抛出异常，这时必须要以某种合理的、高效的方式通知用户。表示无效值最常用的做法是增加一个“哨兵”的角色，它位于解空间之外，如`NULL`、`-1`、`EOF`、`string::npos`、`vector::end()`等。但这些做法是不通用的。

optional使用容器语义，包装了“可能产生无效值”的对象，实现了未初始化的概念，为这种无效值的情况提供了更好的解决方案。

optional包含在以下文件中：

```c++
#include <boost/optional.hpp>
uisng namespace boost;
```

#### 初始化optional

1. 用无参的`optional()`或optional(boost::none)构造一个未初始化的对象
2. `optional(v)`构造一个已初始化的optional对象，内部拷贝v的值
3. `optional(condition, v)`根据条件来构造optional对象，若条件成立`(condition == true)`则初始化为v，否则为未初始化
4. optional支持拷贝构造和赋值构造，可以从另一个optional对象构造
5. `emplace()`可以就地创建对象，避免了构造后再拷贝的代价

#### 用法

可以将其看作是一个大小为1且行为类似于指针的容器（支持*/->操作符），optional还提供了对应的工厂函数`make_optional()`，可以根据参数类型自动推导optional的类型，用来辅助创建optional对象

```c++
// 计算倒数
optional<double> calc(int x)
{
    // 条件构造函数
    return optional<double>(x != 0, 1.0 / x);
}
// 计算实数的平方根
optional<double> sqrt_op(double x)
{
    // 条件构造函数
    return optional<double>(x >= 0, std::sqrt(x));
}

int main()
{
    // 一个未初始化的optional对象
    optional<int> op0;
    optional<int> op1(none);

    // bool测试
    assert(!op0);
    // 比较两个optional对象
    assert(op0 == op1);
    // 获取默认值
    assert(op1.value_or(253) == 253);
    // 使用函数对象
    cout << op1.value_or_eval(
                                [](){ return 857; }
                             ) << endl;
    // 初始化为字符串test，并获取其值
    optional<string> ops("test");
    cout << *ops << endl;

    // 就地创建一个字符串，没有拷贝代价
    ops.emplace("monado", 3);
    assert(*ops == "mon");

    // 容纳一个容器的引用
    vector<int> v(10);
    optional<vector<int>&> opv(v);
    assert(opv);

    // 使用箭头操作符
    opv->push_back(5);
    assert(opv->size() == 11);

    // 重置为初始化状态
    opv = none;
    assert(!opv);

    optional<double> d = calc(10);

    if(d)
    { cout << *d << endl; }
    d = sqrt_op(-10);
    if(!d)
    { cout << "no result" << endl; }
}
```

***

### assign

#### 功能

assign重载了操作符`+=`、`,`、`()`操作符，可以使用非常简洁的语法对容器进行赋值和初始化，在需要大量填入值得时候很有用。

其被定义在以下头文件：

```c++
#include <boost/assign.hpp>
using namespace boost::assign;
```

#### 容器赋值用法

##### operator+=

首先，必须使用`using`打开`boost::assign`命名空间，才能让重载的`+=`在作用域内生效，用法示例：

```c++
using namespace boost::assign;

vector<int> v;
set<string> s;
map<int, string> m;
// 用+=和,插入数据
v += 1, 2, 3, 4, 5, 6*6;
s += "c", "cpp", "lua", "swift";
m += std::make_pair(1, "one"), std::make_pair(2, "two");
```

上面的代码示范了assign库操作标准容器的能力。`+=`操作符后可以接若干个可被容器容纳的元素，元素之间使用“，”分隔。元素不一定是常量，也可以是表达式或函数调用，只要其结果能够转换成容器可容纳的类型即可。比较特别的是map容器，它必须使用辅助函数`make_pair()`来生成容器元素，单纯地用括号把pair的两个成员括起来是无效的。

##### operator()

同样，必须使用`using`打开`boost::assign`命名空间，才能让重载的`+=`在作用域内生效，用法示例：

```c++
using namespace boost::assign;
 
vector<int> _v;
list<string> l;
set<double> _s;
map<int, string> _m;
push_back(_v) (1) (2) (3) (4) (5);
push_front(l) ("c") ("cpp") ("lua") ("swift");
insert(_s) (3.14) (0.618) (1.732);
insert(_m) (1, "one") (2, "two");
```

#### 容器初始化用法

##### list_of

list_of()和前面给容器赋值得用法相似

```c++
vector<int> _v_ = list_of(1) (2) (3) (4) (5);
// v = [1, 2, 3, 4, 5]
```

##### map_list_of/pair_list_of

对于map的处理，有专用的初始化方法，`map_list_of/pair_list_of`可以接收两个参数，然后自动构造`std::pair`插入map中

<u>*【Tips】map_list_of和pair_list_of是完全的同义词*</u>

```c++
map<int, string> _m_ = map_list_of(1, "one") (2, "two") (3, "three");
```

#### 重复输入

##### repeat()

repeat有两个参数，第一个参数指定重复的次数，第二参数指定要插入的值

```c++
vector<int> v1 = list_of(1).repeat(3, 2)(3)(4)(5);
// v1 = [1, 2, 2, 2, 3, 4, 5]
```

##### repeat_fun()

repeat_fun第一个参数和repeat相同，第二个参数传入一个无参的函数对象，将其返回值插入容器

```c++
vector<int> v2 = list_of(1).repeat(3, &rand)(3)(4)(5);
// v2 = [1, x, x, x, 3, 4, 5], x为一个随机数
```

##### range()

range可以将一个序列的全部或部分元素插入另一个序列

```c++
deque<int> d;
push_front(d).range(v1.begin(), v1.begin() + 5);
// d = 3, 2, 2, 2, 1
```

***

### exception

#### 作用

配合C++标准库中的异常处理模式，对其进行了强化，包含在以下头文件

```c++
#include <boost/exception/all.hpp>
using namespace boost;
```

#### boost异常

`boost::exception`提供两个类：`exception`和`error_info`。他们是exception库的基础。

##### exception

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

##### erro_info

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

#### 向异常传递信息

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

#### 错误信息类

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

***

### uuid

#### 一、功能

生成UUID（Universally Unique Identifier），它是一个128位的数字（16字节），它无需中央认证就可以生成全球唯一的标识符，形如

```shell
E4A0D7CE-9E6D-4E74-9E6D-7E749E6D7E74
```



#### 二、类摘要

```c++
class uuid
{
    public:
        static size_type static_size();    //长度，返回16
        size_type size() const;
        uint8_t data[static_size()];    //内部实现
        

        iterator begin();
        iterator end();
        bool is_nil() const;

}
```



#### 三、数据属性

- 没有构造函数，可以像使用POD类型数据一样使用
- 支持流输入输出
- 支持字典序的比较
- 其存储数据的是一个普通数组，但是为其提供了begin()和end()迭代器

#### 四、用法

##### 1、需要包含的头文件

```c++
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

//命名空间
using namespace boost::uuids;
```

##### 2、常用方法

```c++
boost::uuids::uuid u;

//随机输出一个uuid值，但是这种方式生成的uuid很容易重复
cout << u << endl;

assert(boost::uuids::uuid::static_size() == 16);  // 长度总为16
assert(u.size() == 16);

vector<unsigned char> v(16,7);	//一个vector对象
copy(v.begin(),v.end(),u.begin());	//标准复制方法
assert(u.data[0] == u.data[1] && u.data[15] == 7);	//使用数组方式访问

//流输出：07070707-0707-0707-0707-070707070707
cout << u << endl;
fill_n(u.data + 10,6,8);	//使用标准fill_n直接操纵数组

//流输出：07070707-0707-0707-0707-080808080808
cout << u << endl;
```

##### 3、使用生成器生成uuid

> 生成器本身是一个类

1）Nil生成器

生成一个全0的无效uuid

```c++
//第一个()构造生成器对象，第二个()调用对象的函数
uuid u = nil_generator()();
assert(u.is_nil);
//nil_uuid()是一个内联函数，相当于直接调用Nil生成器
u = nil_uuid();
assert(u.is_nil);
```

2）字符串生成器

从一个字符串创建uuid对象（可以是c_str、string、wstring），但是对字符串格式有严格要求

- 没有连字符的全hex数字
- 在5、7、9、11字节使用连字符的全hex数字

```c++
//声明字符串生成器对象
string_generator sgen;

//使用字符串构造
uuid u1 = sgen("0123456789abcdef0123456789abcdef");
cout << u1 << endl;
uuid u2 = sgen("01234567-89ab-cdef-0123-456789abcdef");
cout << u2 << endl;
```

3）名字生成器

name_generator使用基于名字的SHA1摘要算法，使用一个基准的UUID然后使用字符串名字派生出这个UUID的一系列UUID，<u>只要基准UUID不变，那么相同的名字总会创建相同的UUID</u>

```c++
uuid www_xxx_com = string_generator()("0123456789abcdef0123456789abcdef");

//使用uuid构造
name_generator ngen(www_xxx_com);

uuid u1 = ngen("mario");
//为名字生成UUID，version是基于SHA1算法
assert(!u1.is_nil() && u1.version() == uuid::version_name_based_sha1);

uuid u2 = ngen("link");
cout << "mario's id :" << u1 << endl;
cout << "link's id :" << u2 << endl;
```

4）随机生成器
保证生成随机的UUID不会重复

```c++
random_generator rgen;

uuid u = rgen();
assert(u.version() == uuid::version_random_number_based);
cout << u << endl;
```

#### 转换字符串

窄字符串转换

`string to_string(uuid const&);`

宽字符串转换

`wstring to_wstring(uuid const&);`