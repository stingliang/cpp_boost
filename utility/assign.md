## assign

### 功能

assign重载了操作符`+=`、`,`、`()`操作符，可以使用非常简洁的语法对容器进行赋值和初始化，在需要大量填入值得时候很有用。

其被定义在以下头文件：

```c++
#include <boost/assign.hpp>
using namespace boost::assign;
```

### 容器赋值用法

#### operator+=

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

#### operator()

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

### 容器初始化用法

#### list_of

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

### 重复输入

#### repeat()

repeat有两个参数，第一个参数指定重复的次数，第二参数指定要插入的值

```c++
vector<int> v1 = list_of(1).repeat(3, 2)(3)(4)(5);
// v1 = [1, 2, 2, 2, 3, 4, 5]
```

#### repeat_fun()

repeat_fun第一个参数和repeat相同，第二个参数传入一个无参的函数对象，将其返回值插入容器

```c++
vector<int> v2 = list_of(1).repeat(3, &rand)(3)(4)(5);
// v2 = [1, x, x, x, 3, 4, 5], x为一个随机数
```

#### range()

range可以将一个序列的全部或部分元素插入另一个序列

```c++
deque<int> d;
push_front(d).range(v1.begin(), v1.begin() + 5);
// d = 3, 2, 2, 2, 1
```
