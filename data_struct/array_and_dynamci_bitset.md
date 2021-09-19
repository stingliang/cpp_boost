# array

## 功能

array包装了C++的内建数组，为其提供校准容器接口

## 头文件和命名空间

```c++
#include <boost/array.hpp> 
using namespace boost;
```

## 用法

array的模板参数指明了array的元素类型和大小，`array<int, 5>`，相当于声明了一个普通数组`int a[5]`。

但是没有`push_back()`和`push_front()`方法，因为其严格意义上不是一个标准容器，也不能动态增长

除此之外，还为其封装了类似于标准容器的迭代器、`empty()`、`=`、`swap()`等方法。

```c++
#include <boost/array.hpp>
void arrayTest()
{   
    using namespace boost;
    array<int, 10> ar;  // 一个大小为10的数组
    ar.assign(0);
    
    ar[0] = 1;  // 使用[]操作符访问元素
    ar.back() = 10; // 访问最后一个元素
    
    int i = 0;
    for(auto elem : ar)
        cout << "ar[" << i++ << "] " << elem << endl;
}
```

运行结果：

```
ar[0] 1
ar[1] 0
ar[2] 0
ar[3] 0
ar[4] 0
ar[5] 0
ar[6] 0
ar[7] 0
ar[8] 0
ar[9] 10
```

# dynamic_bitset

## 功能

虽然C++标准中可以使用`vector<bool>`和`bitset`来存储二进制数据，但是他们都有各自的限制，不满足标准容器的定义。

`vector<bool>`实际上是元素类型为bool的vector，其内部存储的并不是二进制数据，而bitset大小是固定的

`boost::dynamic_bitset`能够存储二进制数据，而且可以动态增长、进行一系列的位运算。

## 头文件和命名空间

```c++
#include <boost/dynamic_bitset.hpp> using namespace boost;
```

## 用法

### 创建和赋值

- 不带参数的构造函数创建一个大小为0的dynamic_bitset对象（可以在之后增长）
- 传入参数指定dynamic_bitset的大小并赋初值，像标准的容器一样
- 从另一个dynamic_bitset拷贝构造
- 从01字符串创建（很遗憾，`dynamic_bitset`与`std::bitset`有相同的缺陷，要求字符串必须是`std::string`，而不能是C字符串）

### 示例

```c++
#include <boost/dynamic_bitset.hpp>
#include <boost/utility.hpp>
void dynamicBitsetTest()
{
    using namespace boost;
    dynamic_bitset<> db1;   // 空的dyncmic_bitset
    dynamic_bitset<> db2(10);   // 大小为10的dyncmic_bitset
    // 大小为22的dyncmic_bitset
    dynamic_bitset<> db3(0x16, BOOST_BINARY(10101));    
    dynamic_bitset<> db4(string("0100101010011101001001"));   // 字符串构造
    dynamic_bitset<> db5(db3);  // 拷贝构造

    cout << std::hex << db4.to_ulong() << endl;  // 转换为整数
    for(int i = 0; i < db4.size(); i++)
        cout << db4[i];
    cout << endl;
}
```

