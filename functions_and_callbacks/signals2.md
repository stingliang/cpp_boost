# signals2

signals2（衍生自Boost中另一个已被废弃的库signals）实现了线程安全的观察者模式。在signals2库中，观察者模式被称为信号/插槽（signals/slots）机制，它是一种函数回调机制，一个信号关联了多个插槽，当信号发出时，所有关联它的插槽都会被调用

## 头文件和命名空间

```c++
#include <boost/signals2.hpp>
using namespace boostsignals2;
```

## 核心类signal

`signal`共有7个模板参数，这里仅说明比较重要的4个。

```
template<typename Signature, 
		 typename Combiner boost::signals::optional_last_value<R>,
         typename Group = int,
		 typename GroupCompare = std::less<Group>>
```

- `Signature`

  一个函数类型，可以被signal调用的函数，所有加入`signal`的函数都需要符合此处的函数定义

- `Combiner`

  合并器，用于处理回调函数的返回值

- `Group`

  插槽编组的类型（注意，这里是声明编组类型，可以为`std::string`，但一般使用默认的`int`）

- `GroupCompare`

  编组的排序标准，升序或降序，默认是`std::less<Group>`升序

### 操作函数

- `connect()`

  把插槽连接到信号上，相当于为信号（事件）增加了一个处理的handler。插槽可以是任意的可调用对象，包括函数指针、函数对象，以及它们的bind/lambda表达式和function对象，signal内部使用function作为容器来保存这些可调用对象。连接时可以指定组号也可以不指定组号，当信号发生时将依据组号的排序准则依次调用插槽函数。如果连接成功，`connect()`将返回一个connection对象，表示信号与插槽之间的连接关系，它是一个轻量级对象，可以处理两者间的连接，如断开、重连接或测试连接状态

- `disconnect()`

  断开插槽与信号的连接，它有两种形式：传递组号将断开该组的所有插槽、传递一个插槽对象将仅断开该插槽

- `disconnect_all_slots()`

  一次性断开信号的所有插槽连接

- `num_slots()`

  当前信号所连接的插槽数量

- `empty()`

  相当于num_slots（）==0，但它的执行效率较高，disconnect_all_slots（）的结果就是令empty（）返回true

- `operator()`

  可以接收最多9个参数。当operator（）被外界调用时意味着产生了一个信号（事件），从而导致信号所关联的所有插槽被调用。调用插槽的结果被合并器处理后返回，默认情况下是一个optional对象

- `combiner()`和`set_combiner()`

  分别用于获取和设置合并器对象，通过signal的构造函数也可以在创建的时候就传入一个合并器的实例。但除非想改用其他的合并方式，通常我们可以直接使用默认构造函数创建模板参数列表中指定的合并器对象

- `~signal()`

  析构时自动断开所有插槽连接

### 用法

假设有以下两个无参的函数，它们将被用作插槽

```c++
void slots1(int& i) { i++; }
void slots2(int& i) { i++; }
```

#### 基本用法

```c++
// 基本用法，指定插槽类型void()，其他模板参数使用默认值
signal<void(int&)> sig;
BOOST_CHECK(sig.empty());
int i(0);

// 使用connect()来连接插槽，使用operator()来产生信号
sig.connect(&slots1);
sig.connect(&slots2);
BOOST_CHECK_EQUAL(sig.num_slots(), 2);
sig(i);

BOOST_CHECK_EQUAL(i, 2);
```

#### 使用组号

在连接插槽时我们省略了`connect()`的第二个参数`connect_position`，它的默认值是`at_back`，表示将插槽插入信号插槽链表的尾部，因此slots2将在slots1之后被调用

```c++
signal<void(int&)> sig;
int i(0);
// 最后一个被调用
sig.connect(&slots1, at_back);
// 第一个被调用
sig.connect(&slots2, at_front);
sig(i);
BOOST_CHECK_EQUAL(i, 2);
```

`connect()`函数的另一个重载形式`connect(const group_type &group, const slot_type &slot, connect_position position = at_back)`可以在连接时指定插槽所在的组号，默认情况下组号是int类型。组号不一定要从0开始连续编号，它可以是任意的数值，离散值、负值均可。如果在连接的时候指定组号，那么每个编组的插槽又是一个插槽链表，从而形成一个略微有些复杂的二维链表，它们的顺序规则如下：

1. 各编组的调用顺序由组号从小到大决定（也可以在signal的第四个模板参数中改变排序函数对象）
2. 每个编组的插槽链表内部的插入顺序用at_back和at_front指定
3. 如果未被编组的插槽的位置标志是at_front，它将在所有的编组之前被调用
4. 如果未被编组的插槽的位置标志是at_back，它将在所有的编组之后被调用

#### 处理返回值

`signal`不仅可以把输入参数转发给所有插槽，也可以传回插槽的返回值。默认情况下，`signal`使用合并器`optional_last_value＜R＞`，它将使用`optional`对象返回最后被调用的插槽的返回值，它是一个`optional`对象，必须用解引用操作符`*`来获得值

```c++
// 获取返回值
signal<int(int&)> sig;
int i(0);
// 连接插槽
sig.connect([&](int& i)->int{ return i; });
sig.connect([&](int& i)->int{ return i + 1; });
// 返回值是optional对象
boost::optional result = sig(i);
// 返回的是最后被调用函数的返回值，使用*获取值
BOOST_CHECK_EQUAL(*result, 1);
```

#### 自定义合并器

默认的合并器`optional_last_value＜R＞`并没有太多的意义，一般用于不关心插槽返回值的情况下。但大多数时候，插槽的返回值都是有意义的，需要以某种方式处理多个插槽的返回值

`signal`允许用户自定义合并器来处理插槽的返回值，把多个插槽的返回值合并为一个结果返回给用户。合并器应该是一个函数对象**（不是函数或函数指针）**，合并器的形式如下：

```c++
template<typename T>
class combiner {
public:
    typedef T result_type;
    template<typename InputerIterator>
    result_type operator()(InputerIterator, InputerIterator) const;
};
```

`combiner`的调用操作符`operator()`的返回值类型可以是任意类型，其类型完全由用户指定，不一定是`optional`或是插槽的返回值类型

函数的模板参数`InputIterator`是插槽链表的返回值迭代器，可以使用它来遍历所有插槽的返回值，进行必要的处理

##### 示例合并器

此示例合并器将插槽的返回值转存在`std::vector<T>`中

```c++
// 定义一个合并器
template<typename T>
struct Combiner {
    // 返回类型
    typedef std::vector<T> result_type;
    // 重载()
    template<typename Iterator>
    // 迭代插槽返回值
    result_type operator()(Iterator begin, Iterator end) {
        if (begin == end) {
            return result_type();
        }
        return result_type(begin, end);
    }
};
```

使用自定义合并器的时候，我们需要改写signal的声明，在模板参数列表中增加第二个模板参数：合并器类型

`signal＜int(int),Combiner＜int＞＞sig;`

当信号被调用时，signal会自动把解引用操作转换为插槽调用，使用给定的合并器的`operator()`逐个处理插槽的返回值，并最终返回合并器`operator()`的结果。如果不使用`signal`的默认构造函数，而是在构造`signal`时传入一个合并器的实例，那么`signal`将使用这个合并器的**拷贝**处理返回值

```c++
signal<int(int&), Combiner<int>> sig;
int i(0);
// 连接插槽
sig.connect([&](int& i)->int{ return i; });
sig.connect([&](int& i)->int{ return i + 1; });
// 返回值是合并器()的返回类型，这里为std::vector<int>，可以使用auto推导
auto result = sig(i);
BOOST_CHECK_EQUAL(printStl(result).str(), "[0][1]");
```

#### 管理信号连接

信号与插槽的连接并不要求是永久性的，当信号调用完插槽后，有可能需要把插槽从信号中断开，再将插槽连接到其他信号上去，signals2库提供一种较为灵活的连接管理方式：使用`connection`对象

这里的示例使用以下`signal`对象

```c++
signal<int(int&), Combiner<int>> sig;
int i(0);
```

##### `connection`对象

`connection`是可拷贝、可赋值的，它也重载了比较操作符，因此它可以被安全地放入标准序列容器或关联容器中，成员函数`disconnect()`和`connected()`分别用来与信号断开连接和检测连接状态

```c++
connection c1 = sig.connect([&](int i)->int{ return i; });
connection c2 = sig.connect([&](int i)->int{ return i; });
connection c3 = sig.connect([&](int i)->int{ return i; });
BOOST_CHECK_EQUAL(sig.num_slots(), 3);

// 断开c1的连接
c1.disconnect();
BOOST_CHECK_EQUAL(sig.num_slots(), 2);
BOOST_CHECK(!c1.connected());
BOOST_CHECK(c2.connected());
```

##### `scoped_connection`对象

`scoped_connection`是`connection`的子类，提供RAII功能：插槽与信号的连接仅在作用域内生效，离开作用域会自动断开，当需要临时连接信号时，`scoped_connection`非常有用

```c++
// 使用RAII机制临时连接信号
{
    // 进入局部作用域，建立连接
    scoped_connection sc = sig.connect([&](int i)->int{ return i; });
    BOOST_CHECK_EQUAL(sig.num_slots(), 3);
}
// 离开局部作用域，连接自动断开
BOOST_CHECK_EQUAL(sig.num_slots(), 2);
```

##### 阻塞连接

插槽与信号的连接一旦断开就不能再连接起来，`connection`不提供`reconnet()`这样的函数。但`connection`可以暂时地“阻塞”插槽与信号的连接，当信号发生时被阻塞的插槽将不会被调用，`connection`对象的`blocked()`函数可以检测插槽是否被阻塞，但被阻塞的插槽并没有与信号断开连接，在需要的时候可以随时解除阻塞

`connection`自身没有阻塞的功能，我们需要用一个辅助类`shared_connection_block`来实现阻塞

`shared_connection_block`可以阻塞`connection`对象，直到它被析构或显式调用`unblock()`函数

```c++
// 阻塞c2
shared_connection_block block(c2);
// 仍有两个连接
BOOST_CHECK_EQUAL(sig.num_slots(), 2);
// c2被阻塞
BOOST_CHECK(c2.blocked());
// 只有c3会被调用
auto result = sig(i);
BOOST_CHECK_EQUAL(printStl(result).str(), "[0]");
```
