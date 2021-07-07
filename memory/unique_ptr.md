# “独占型”指针unique_ptr

unique_ptr 指针指向的堆内存无法同其它 unique_ptr 共享，每个 unique_ptr 指针都独自拥有对其所指堆内存空间的所有权，被定义在头文件`<memory>`中，并且使用std命名空间

## 构造unique_ptr

##### 空构造

```c++
std::unique_ptr<int> p1();
std::unique_ptr<int> p2(nullptr);
```

##### 接管已有指针

```c++
std::unique_ptr<int> p3(new int);
```

由此就创建出了一个 p3 智能指针，其指向的是可容纳 1 个整数的堆存储空间

##### 移动构造

```c++
std::unique_ptr<int> p4(new int);
// std::unique_ptr<int> p5(p4);//错误，堆内存不共享
std::unique_ptr<int> p5(std::move(p4));//正确，调用移动构造函数
```

对于调用移动构造函数的 p4 和 p5 来说，p5 将获取 p4 所指堆空间的所有权，而 p4 将变成空指针`(nullptr)`

## 功能函数

##### release()

释放当前 unique_ptr 指针对所指堆内存的所有权，但该存储空间并不会被销毁

##### reset(p)

其中p表示一个普通指针，如果p为nullptr，则当前unique_ptr也变成空指针；反之，则该函数会释放当前unique_ptr指针指向的堆内存（如果有），然后获取p所指堆内存的所有权（p为`nullptr`）

## 工厂函数

在C++11标准库中，并没有提供unique_ptr的make_unique工厂函数（C++14提供了），但是在boost库中，对其的工厂函数也做了补充

##### C++14

```c++
auto p = std::make_unique<int>(10);
assert(p && *p == 10);
```

##### 使用boost库中的make_unique

首先需要包含头文件`＜boost/smart_ptr/make_unique.hpp＞`，使用命名空间`boost`

```c++
#include <boost/smart_ptr/make_unique.hpp>

auto p = boost::make_unique<int>(10);
assert(p && *p == 10);
```

## 定制删除器

 默认情况下，unique_ptr指针采用`std::default_delete<T>`方法释放堆内存,也可以自定义符合实际场景的释放规则,和 shared_ptr 指针不同，为 unique_ptr 自定义释放规则，只能采用函数对象的方式。例如：
//自定义的释放规则

```c++
struct myDel
{
    void operator()(int *p) {
        delete p;
    }
};
std::unique_ptr<int, myDel> p6(new int, myDel());
```

