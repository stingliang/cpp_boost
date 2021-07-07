# assert

## 功能

类似于标准库中的assert，但功能有所增强

## 头文件和命名空间

```c++
#include <boost/assert.hpp>
```

## 基本用法

定义了两个断言宏

```c++
//等同于assert
define BOOST_ASSERT(expr)    assert(expr)
//允许断言失败时输出描述字符，有助于排错
define BOOST_ASSERT_MSG(expr,msg)    assert((expr)&&(msg))
```

## 错误处理

在头文件前定义宏`BOOST_ENABLE_ASSERT_HANDLER`，那么在后面的`BOOST_ASSERT`将不在等同于`assert`，而是调用一个断言失败的函数

- `assertion_failed()`
- `assertion_failed_msg()`

相当于一个错误处理handler，且这两个函数被设定为没有具体实现，其声明如下：

```c++
namespace boost
{
    void assertion_failed(char const * expr,
                        char const * function,char const * file,long line)
    void assertion_failed_msg(char const * expr,char const * msg,
                        char const * function,char const * file,long line)
}
```

【示例】

```c++
#define BOOST_ENABLE_ASSERT_HANDLER
#include <sting.h>
#include <boost/assert.hpp>
#include <boost/format.hpp>

namespace boost
{
	void assertion_failed(char const * expr,char const * function,
                                         char const * file,long line){}
	void assertion_failed_msg(char const * expr,char const * msg,
                     char const * function,char const * file,long line)
	{
		boost::format fmt("Assertion failed!\nExpression:%s\n"
            		  "Function: %s\nFile: %s\nLine: %ld\n"
            		  "Msg: %s\n\n");
		fmt %expr %function %file %line %msg;
		std::cout << fmt;
            exit(0);
	}
}

using namespace std;

double func(int x)
{
	BOOST_ASSERT_MSG(x != 0,"divided by zero");
	cout << "after BOOST_ASSERT" << endl;
	return 1.0/x;
}

int main()
{
	cout << func(0) << endl;
}
```

运行结果：

```
Assertion failed!
Expression:x != 0
Function: double func(int)
File: boost_assert.cpp
Line: 26
Msg: divided by zero
```

