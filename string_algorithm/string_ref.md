# string_ref

## 功能

一种轻量级的string，持有string类型的引用

## 头文件

```c++
#include <boost/utility/string_ref.hpp>
using namespace boost;
```

## 类摘要

```c++
template<typename charT,typename traits>
class basic_string_ref
{
    public:
        //和std::string有着几乎一样的接口
    private:
        const charT* ptr_;    //字符串指针
        std::size_t len_;     //字符串长度
};
```

不拷贝字符串，所以不分配内存，使用两个成员变量表示字符串

## 用法

【注意事项】只能像std::string&一样去获取其内容，<u>但不能修改其本身</u>

1、构造

```c++
//通过标准字符数组构造普通string，有拷贝成本
const char* ch = "hello";
string str(ch);

//字符数组构造，无成本
string_ref s1(ch);

//标准字符串构造，无成本
string_ref s2(str);
```

可以像使用普通string一样使用string_ref（除了修改）

2、用在哪

用于代替string&作为函数参数和返回值，可以完全避免字符串拷贝代价