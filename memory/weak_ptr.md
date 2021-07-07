# “弱”指针weak_ptr

因为它不具有普通指针的行为，没有重载operator`*`和`-＞`。它的最大作用在于协助shared_ptr工作，它只负责观测资源的使用情况。

## 构造函数

weak_ptr被设计为与shared_ptr协同工作，它的构造和构造不会使shared_ptr的引用计数增加或减少，它可以从一个shared_ptr或者weak_ptr构造

```c++
shared_ptr<int> sp(new int(10));
assert(sp.use_count() == 1);

weak_ptr<int> wp(sp);
assert(wp.use_count() == 1);
assert(!wp.empty());
```

## 功能函数

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

## 对象自我管理  

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

```c++
// 错误用法
self_shared ss;
auto p = ss.shared_from_this();
```

