### “独占型”指针unique_ptr

unique_ptr 指针指向的堆内存无法同其它 unique_ptr 共享，每个 unique_ptr 指针都独自拥有对其所指堆内存空间的所有权，被定义在头文件`<memory>`中，并且使用std命名空间

#### 构造unique_ptr

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

#### 功能函数

##### release()

释放当前 unique_ptr 指针对所指堆内存的所有权，但该存储空间并不会被销毁

##### reset(p)

其中p表示一个普通指针，如果p为nullptr，则当前unique_ptr也变成空指针；反之，则该函数会释放当前unique_ptr指针指向的堆内存（如果有），然后获取p所指堆内存的所有权（p为`nullptr`）

#### 工厂函数

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

#### 定制删除器

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

***

### “共享”指针shared_ptr

boost库中的shared_ptr，被收纳为C++新标准中的shared_ptr，可以自由地拷贝和赋值，当没有代码在使用它时，才会自行析构。

#### 基本概念

引用计数：顾名思义，该指针被引用的次数

#### 构造shared_ptr

##### 空构造

```c++
shared_ptr() // 创建一个空智能指针，其原始指针就是nullptr
```

##### 接管原始指针

```c++
shared_ptr(T * p) // 从已有的指向T类型地指针p处接管内存，同时引用计数加一
```

##### 接管智能指针

```c++
shared_ptr(shared_ptr conster&r) operator= 
// 从另一个智能指针构造，引用计数加一，相当于和原来的shared_ptr共同管理一块内存
```

##### 指定析构函数的智能指针

```c++
shared_ptr(T * p, D d) 
// 其构造过程类似于（2），但是使用了D d作为指定的析构函数，在后面章节会讲到
```

#### 功能函数

##### reset()

将引用计数减一。它也可以带参数，参数类型和构造函数相似，相当于先将引用计数减一，再去接管另一个指针

##### unique()和use_count()

`unique()`在shared_ptr是指针的唯一所有者时为true，`use_count()`返回当前指针的引用计数

#### 工厂函数make_shared

`make_shared()`函数可以接收若干个参数，如何传递给类型T的构造函数（在构造对象时很有用），然后创建一个`shared_ptr<T>`的对象并返回，通常使用工厂函数创建对象比直接创建shared_ptr对象的方式更快且更高效

#### 定制删除器

`shared_ptr(Y*p，Dd)`的第一个参数是要被管理的指针，它的含义与其他构造函数的参数相同。而第二个参数则告诉shared_ptr在析构时不要使用delete来操作指针p，而要用d来操作，即把deletep换成d（p）

在这里删除器d可以是一个函数对象，也可以是一个函数指针，只要它能够像函数那样被调用，使得d（p）成立即可。对删除器的要求是它必须可拷贝，其行为也必须像delete那样，不能抛出异常。为了配合删除器的工作，shared_ptr提供一个自由函数`get_deleter()`，它能够返回内部的删除器指针。有了删除器的概念，我们就可以用shared_ptr实现管理任意资源。只要这种资源提供了它自己的释放操作，shared_ptr就能够保证它自动释放。

举例：假设有一组文件操作函数，使用file_t:

```c++
class file_t{...}; 
// open file 
file_t * open_file() 
{    
    cout << "open file" <<endl;    
    ...    
    return new file_t; 
} 
// close file 
void close_file(file_t * s) 
{    
    cout << "close file" <<endl;    
    ... 
}
```

那么对文件指针的释放操作就应该是close_file()，而不是delete，在这里，删除器`close_socket()`是一个自由函数，因此只需要把函数名传递给shared_ptr即可。也可以在函数名前加上取地址操作符“&”，其效果是等价的：

```c++
file_t * s = open_socket(); 
// 直接传入删除器 
shared_ptr<file_t> p(s, &close_file); 
// 等价操作 
// shared_ptr<file_t> p(s, &close_file);
```

#### 用法示例

shared_ptr被包含在

```c++
#include <boost/smart_ptr.hpp> 
using namespace boost;
```

`shared_ptr()`可以在任何情况下接手new的分配结果，其也提供基本的线程安全访问，可以被多个线程安全地读取

```c++
// 接管原始指针
shared_ptr<int> sp(new int(10));
// 此时shared_ptr是指针唯一的额持有者
assert(sp.unique());

// 拷贝构造
shared_ptr<int> sp1(sp);
// shared_ptr<int> sp1 = sp;
assert(sp == sp1 && sp.use_count() == 2);

// 使用解引用操作符操作被指对象
*sp1 = 100;

// 停止使用shared_ptr
sp.reset();

// 使用工厂函数构造
auto sp2 = make_shared<string>("make shared");
auto sp3 = make_shared<vector<int>>(10, 2);
assert(sp3 -> size() == 10);
```

***

### “弱”指针weak_ptr

因为它不具有普通指针的行为，没有重载operator`*`和`-＞`。它的最大作用在于协助shared_ptr工作，它只负责观测资源的使用情况。

#### 构造函数

weak_ptr被设计为与shared_ptr协同工作，它的构造和构造不会使shared_ptr的引用计数增加或减少，它可以从一个shared_ptr或者weak_ptr构造

```c++
shared_ptr<int> sp(new int(10));
assert(sp.use_count() == 1);

weak_ptr<int> wp(sp);
assert(wp.use_count() == 1);
assert(!wp.empty());
```

#### 功能函数

weak_ptr没有重载operator`*`和`-＞`，因为它不共享指针，不能操作资源，这正是它“弱”的原因。但它可以使用一个非常重要的成员函数`lock()`从被观测的shared_ptr获得一个可用的shared_ptr对象，把“弱”关系转换为“强”关系，从而操作资源。但当表示指针是否有效的`expired()==true`时，`lock()`函数将返回一个存储空指针的shared_ptr。

```c++
// 接上例
if(!wp.expired())
{
    // 获得一个shared_ptr
    shared_ptr<int> sp2 = wp.lock();
    *sp2 = 100;
    assert(wp.use_count() == 2);
}

assert(wp.use_count() == 1);
// 智能指针失效
sp.reset();
assert(wp.expired());
// weak_ptr将获得一个空指针
assert(!wp.lock());
```

#### 对象自我管理  

weak_ptr的一个重要用途是获得this指针的shared_ptr，使对象自己能够生产shared_ptr管理自己：对象使用weak_ptr观测this指针，这并不影响引用计数，在需要的时候就调用`lock()`函数，返回一个符合要求的shared_ptr供外界使用。
这个解决方案是一种惯用法，在头文件`＜boost/enable_shared_from_this.hpp＞`里定义一个助手类`enable_shared_from_this＜T＞`，它的声明摘要如下：

```c++
template<class T>
class enable_shared_from_this    // 辅助类，需要继承使用
{
public:
    // 工厂函数，产生this指针的shared_ptr
    shared_ptr<T> shared_from_this();
};
```

使用weak_ptr的时候只需要让想被shared_ptr管理的类继承它即可，成员函数`shared_from_this()`会返回this指针的shared_ptr。例如：

```c++
class self_shared: public enable_shared_from_this<self_shared>
{
public:
    self_shared(int n):num(n){}
    int num;
    void print()
    {
        cout << "self_shared:" << num << endl;
    }
};

int main()
{
    auto sp = make_shared<self_shared>(313);
    sp ->  print();
    
    auto p = sp -> shared_from_this();
    p -> num = 100;
    p -> print();
}
```

> 【注意事项】不能对一个未被shared_ptr管理的对象使用shared_from_this操作获取shared_ptr，否则会产生未定义的错误。
>

```c++
// 错误用法
self_shared ss;
auto p = ss.shared_from_this();
```

