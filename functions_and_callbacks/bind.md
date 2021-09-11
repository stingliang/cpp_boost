# bind

bind是对C++标准中函数适配器bind1st/bind2nd的泛化和增强，可以适配任意的可调用对象，包括函数指针、函数引用、成员函数指针、函数对象和lambda表达式。

bind远远地超越了STL中的函数绑定器bind1st/bind2nd，它最多可以绑定9个函数参数，而且bind对被绑定对象的要求很低，它可以在没有result_type内部类型定义的情况下完成对函数对象的绑定。

bind并不是一个单独的类或函数，而是非常庞大的家族，依据绑定的参数个数和要绑定的调用对象类型不同，bind总共有数十个不同的重载形式，但它们的名字都叫作bind，编译器会根据具体的绑定代码自动推导要使用的正确形式。

## 头文件和命名空间

```c++
#include <boost/bind.hpp>
using namespace boost;
```

## 基本用法

bind接收的第一个参数必须是一个可调用对象f，可以是函数、函数指针、函数对象或成员函数指针，之后bind最多接收9个参数，参数的数量必须与f的参数数量相等，这些参数将被传递给f作为入参。

绑定完成后，bind会返回一个函数对象，它内部保存了f的拷贝，具有operator（），返回值类型被自动推导为f的返回值类型。

在发生调用时，这个函数对象将把之前存储的参数转发给f完成调用。

例如，如果有一个函数func，它的形式如下：

`func(a1,a2)`

那么，它等价于一个具有无参operator（）的bind函数对象调用：`bind(func,a1,a2)()`这是bind最简单的形式。bind表达式存储了func和a1、a2的拷贝，产生了一个临时函数对象。因为func接收两个参数，而a1和a2都是实参，所以临时函数对象将具有一个无参的operator（）。当operator（）调用发生时函数对象把a1、a2的拷贝传递给func，以完成真正的函数调用。

bind的真正威力在于它的占位符，它们分别被定义为\_1、\_2、\_3一直到\_9，位于一个匿名名字空间。占位符可以取代bind中参数的位置，在发生函数调用时才接收真正的参数。占位符的名字都以下画线开始，然后接一个数字，这可能稍微有点“奇怪”，但它们是完全合法的C++标识符，起这样的名字完全是为了方便书写、识别和记忆。占位符的名字表示它在调用式中的顺序，而绑定表达式对顺序没有要求，\_1不一定要第一个出现，也不一定只出现一次。

例如：`bind(func,_2,_1)(a1,a2)`

上述代码返回一个具有两个参数的函数对象，第一个参数将放在函数func的第二个位置，而第二个参数则放在第一个位置，调用时上述代码等价于：`func(a2,a1)`

### 示例函数

假设有如下两个函数

```c++
int twoParamsFunc(int a, int b) { return a + b; }	// 二元函数
int tripParamsFunc(int a, int b, int c) { return a + b * c; } // 三元函数
```

#### 绑定普通函数的用法

`bind(twoParamsFunc,1,2)`将返回一个无参调用的函数对象，等价于`twoParamsFunc(1,2)`。`bind(tripParamsFunc,1,2,3)`同样返回一个无参调用的函数对象，等价于`tripParamsFunc(1,2,3)`。这两个绑定表达式没有使用占位符，而是给出了全部的具体参数，代码如下：

```c++
BOOST_CHECK_EQUAL(boost::bind(twoParamsFunc, 1, 2)(), twoParamsFunc(1, 2));
BOOST_CHECK_EQUAL(boost::bind(tripParamsFunc, 1, 2, 3)(), tripParamsFunc(1, 2, 3));
```

#### 占位符的用法

使用占位符bind可以有更多的变化，这才是它真正应该做的工作，下面列出了一些占位符的用法：

```c++
int x(1), y(2), z(3);
BOOST_CHECK_EQUAL(boost::bind(twoParamsFunc, _1, 9)(x), twoParamsFunc(x, 9));
BOOST_CHECK_EQUAL(boost::bind(twoParamsFunc, _1, _2)(x, y), twoParamsFunc(x, y));
BOOST_CHECK_EQUAL(boost::bind(twoParamsFunc, _2, _1)(x, y), twoParamsFunc(y, x));
BOOST_CHECK_EQUAL(boost::bind(twoParamsFunc, _1, _1)(x, y), twoParamsFunc(x, x));   // y参数被忽略
BOOST_CHECK_EQUAL(boost::bind(tripParamsFunc, _1, 8, _2)(x, y), tripParamsFunc(x, 8, y));
BOOST_CHECK_EQUAL(boost::bind(tripParamsFunc, _3, _2, _2)(x, y, z), tripParamsFunc(z, y, y));   // x参数被忽略
```

#### 操作成员函数

类的成员函数不同于普通函数，因为成员函数指针不能直接调用`operator()`，它必须先被绑定到一个对象或指针上，然后才能得到this指针进而调用成员函数。因此bind需要“牺牲”一个占位符的位置，要求用户提供一个类的实例、引用或指针，通过对象作为第一个参数来调用成员函数：

`bind(&X::func,x,_1,_2,...)`

这意味着使用成员函数时最多只能绑定8个参数。

假设有一个类`FuncTest`

```c++
struct FuncTest { int memberFunc(int a, int b) { return a + b; } };

FuncTest ft, &rft = ft;
FuncTest *p = &rft;
```

那么bind应该这样被使用

```c++
BOOST_CHECK_EQUAL(boost::bind(&FuncTest::memberFunc, ft, _1, 20)(10), ft.memberFunc(10, 20));
BOOST_CHECK_EQUAL(boost::bind(&FuncTest::memberFunc, rft, _2, _1)(10, 20), rft.memberFunc(20, 10));
BOOST_CHECK_EQUAL(boost::bind(&FuncTest::memberFunc, p, _1, _2)(10, 20), p->memberFunc(10, 20));
```

#### 操作成员变量

`bind`的另一个对类的操作是它可以绑定`public`成员变量，它就像是一个选择器，其用法与绑定成员函数类似，只需要像使用一个成员函数一样去使用成员变量名。

```c++
BOOST_CHECK_EQUAL(boost::bind(&FuncTest::m_a, _1)(ft), ft.m_a); // 使用占位符
BOOST_CHECK_EQUAL(boost::bind(&FuncTest::m_a, rft)(), rft.m_a); // 直接绑定
```

#### 操作函数对象

`bind`不仅能够绑定函数和函数指针，也能够绑定任意的函数对象，包括标准库中的所有预定义的函数对象。

如果函数对象有内部类型定义`result_type`，那么`bind`可以自动推导出返回值类型，其用法与绑定普通函数一样。但如果函数对象没有定义`result_type`，则需要在绑定形式上进行改动，用模板参数指明返回类型，像这样：

```c++
bind<result_type>(functor, ...);
```

标准库和Boost库中的大部分函数对象都具有`result_type`定义，因此不需要特别的形式就可以直接使用`bind`。例如：

```c++
BOOST_CHECK(boost::bind(std::greater<int>(), _1, 10)(20));
BOOST_CHECK_EQUAL(boost::bind(std::plus<int>(), _1, _2)(1, 2), 3);
BOOST_CHECK_EQUAL(boost::bind(std::modulus<int>(), _1, 3)(3), 0);
```

对于自定义函数对象，如果没有`result_type`类型定义。例如：

```c++
struct func { int operator()(int a, int b) { return a + b; } };
```

那么，我们必须在模板参数里指明bind的返回值类型，像这样：

`bind<int>(func(),_1,_2)(10,20)`

这种写法多少会有些不便，因此，在编写自己的函数对象时，最好遵循规范为它们增加内部的`typedef result_type`，这将使函数对象与许多其他标准库和Boost库组件良好地配合工作。

