## ignore_unused

### 功能

编写代码的过程中有时会出现一些暂时用不到但又必须保留的变量，GCC等编译器会对此发出警告，使用`-Wunused`可以关闭这些警告消息，不过这也有可能导致潜在的隐患。古老的办法是使用`(void)var`的形式来“使用”一下变量，但这种方法含义不明确，不利于维护。

Boost程序库的ignore_unused组件就这个问题给出了更好的解决方案。它原本是proto库里的一个小工具，因为在很多其他组件中被使用，所以在增强了功能后被“扶正”。

`ignore_unused`位于名字空间`boost`，需要包含的头文件如下：

```c++
#include <boost/core/ignore_unused.hpp> 
using namespace boost;
```

### 用法

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

