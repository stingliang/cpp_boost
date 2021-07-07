## optional

### 功能

在实际的软件开发过程中我们经常会遇到无效值的情况。例如，函数并不是总能返回有效值，很多时候即使函数正确执行，但其结果却不是合理的值。如果用数学语言来解释，这种情况就是返回值位于函数解空间之外。

求一个数的倒数、在实数域内开平方、在字符串中查找子串，它们都可能返回无效值。有些无效返回的情况可以用抛出异常的方式来通知用户，但在某些情况下，这样代价很高或不允许抛出异常，这时必须要以某种合理的、高效的方式通知用户。表示无效值最常用的做法是增加一个“哨兵”的角色，它位于解空间之外，如`NULL`、`-1`、`EOF`、`string::npos`、`vector::end()`等。但这些做法是不通用的。

optional使用容器语义，包装了“可能产生无效值”的对象，实现了未初始化的概念，为这种无效值的情况提供了更好的解决方案。

optional包含在以下文件中：

```c++
#include <boost/optional.hpp>
uisng namespace boost;
```

### 初始化optional

1. 用无参的`optional()`或optional(boost::none)构造一个未初始化的对象
2. `optional(v)`构造一个已初始化的optional对象，内部拷贝v的值
3. `optional(condition, v)`根据条件来构造optional对象，若条件成立`(condition == true)`则初始化为v，否则为未初始化
4. optional支持拷贝构造和赋值构造，可以从另一个optional对象构造
5. `emplace()`可以就地创建对象，避免了构造后再拷贝的代价

### 用法

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
