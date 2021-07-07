## noncopyable

### 功能

noncopyable允许轻松实现一个禁止拷贝的类

需要包含以下头文件：

```c++
#include <boost/noncopyable.hpp>
```

### 原理

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

### 用法

为实现不可拷贝类，使用如下解决方案，既从`boost::noncopyable`派生即可，上面的例子可简化为

```c++
class do_not_copy:boost::noncopyable
{...}
```

### 实现原理

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
