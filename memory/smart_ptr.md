# “共享”指针shared_ptr

boost库中的shared_ptr，被收纳为C++新标准中的shared_ptr，可以自由地拷贝和赋值，当没有代码在使用它时，才会自行析构。

## 基本概念

引用计数：顾名思义，该指针被引用的次数

## 构造shared_ptr

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

## 功能函数

##### reset()

将引用计数减一。它也可以带参数，参数类型和构造函数相似，相当于先将引用计数减一，再去接管另一个指针

##### unique()和use_count()

`unique()`在shared_ptr是指针的唯一所有者时为true，`use_count()`返回当前指针的引用计数

## 工厂函数make_shared

`make_shared()`函数可以接收若干个参数，如何传递给类型T的构造函数（在构造对象时很有用），然后创建一个`shared_ptr<T>`的对象并返回，通常使用工厂函数创建对象比直接创建shared_ptr对象的方式更快且更高效

## 定制删除器

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

## 用法示例

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
