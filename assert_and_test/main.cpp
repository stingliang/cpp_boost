/*
 * @Name: liangrui
 * @Date: 2020-12-31 14:12:45
 * @LastEditTime: 2021-01-20 10:58:06
 */
#define COUT(str) (cout << "-------------\n" \
                        << str \
                        << "\n-------------" \
                        << endl)

// c++ stdlib
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <functional>
#include <cmath>
#include <list>
#include <memory>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::set;
using std::list;

// --------------------boost assert--------------------
#include <boost/assert.hpp>
#include <boost/core/lightweight_test.hpp>
double func(int x)
{
    BOOST_ASSERT_MSG(x != 0, "divided by zero");
    return 1.0/x; 
}
void assertTest()
{
    auto result = func(5);
    cout << "func(1) passed. Result: " << result << endl;
    // func(0);
}

int main()
{
    COUT("assert");
    assertTest();

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
}
