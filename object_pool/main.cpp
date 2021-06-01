/*
 * @Name: liangrui
 * @Date: 2020-12-29 13:52:48
 * @LastEditTime: 2020-12-29 14:17:31
 */

// c++ stdlib
#include <iostream>
#include <vector>
#include <string>
#include <functional>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

// boost library
#include <boost/smart_ptr.hpp>
#include <boost/smart_ptr/make_unique.hpp>
#include <boost/pool/object_pool.hpp>
using namespace boost;

struct demo_class
{
    int a,b,c;
    string str = "hello boost";

    demo_class(int x = 1, int y = 2, int z = 3):a(x), b(y), c(z)
    {
        cout << "created a demo_class object." << endl;
    }
    ~demo_class()
    {
        cout << "destroyed a demo_class object." << endl;
    }
};

int main()
{
    // 对象内存池
    object_pool<demo_class> op;

    // 传递参数构造一个对象
    auto p = op.construct(4, 5, 6);

    cout << p->a << "\n"
         << p->b << "\n"
         << p->c << "\n"
         << p->str << "\n"
         << endl;

    // 连续分配大量对象
    for (int i = 0; i < 10 ; i++)
    {
        auto p = op.construct(4, 5, 6);
    }
    cout << "at the end of the main function.\n" << endl;
}