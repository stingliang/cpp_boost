# lightweight_test

## 测试断言

lightweight_test只提供一个轻量级的单元测试框架，提供基本的单元测试，不支持测试用例、测试套件的概念。

## 头文件和命名空间

```c++
#include <boost/core/lightweight_test.hpp>
```

## 测试断言

lightweight_test定义了数个测试断言，以下列出较常用的几个。

- BOOST_TEST（e）：断言表达式成立
- BOOST_TEST_NOT（e）：断言表达式不成立
- BOOST_ERROR（s）：直接断言失败，输出错误消息s
- BOOST_TEST_EQ（e1，e2）：断言两个表达式相等
- BOOST_TEST_NE（e1，e2）：断言两个表达式不等
- BOOST_TEST_CSTR_EQ（e1，e2）：断言两个C字符串相等
- BOOST_TEST_CSTR_NE（e1，e2）：断言两个C字符串不相等
- BOOST_TEST_THROWS（e，ex）：断言表达式e抛出异常ex

如果以上断言失败，就会增加内部的错误计数，lightweight_test提供函数`boost::report_errors()`来输出测试结果，在测试结束时，我们必须调用`report_errors()`，否则会发生BOOST_ASSERT断言错误。

## 用法示例

```c++
auto p = std::make_shared<int>(10);

BOOST_TEST(*p == 10);  // 测试解引用
BOOST_TEST(p.unique());  // 测试唯一性
BOOST_TEST_NOT(!p);  // 测试指针是否为空

BOOST_TEST_EQ(p.use_count(), 1);  // 相等测试
BOOST_TEST_NE(*p, 20);  // 不等测试

p.reset();
BOOST_TEST(!p);

BOOST_TEST_THROWS(*p, std::runtime_error);
BOOST_ERROR("error accured!");

return boost::report_errors();  // 输出测试报告
```

运行结果：

```
main.cpp(62): expression '*p' did not throw exception 'std::runtime_error' 
in function 'int main()'
main.cpp(63): error accured! in function 'int main()'
2 errors detected.
```

