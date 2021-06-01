/*
 * @Name: liangrui
 * @Date: 2020-12-29 16:39:10
 * @LastEditTime: 2021-01-04 10:39:11
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
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::set;
using std::list;

// boost library
#include <boost/optional.hpp>
#include <boost/assign.hpp>
#include <boost/exception/all.hpp>
using namespace boost;

// 打印STL-vector
template<typename T>
void printStl(vector<T>& v)
{
    for(auto elem : v)
        cout << elem << endl;
}
// 打印STL-map
template<typename T1, typename T2>
void printStl(map<T1,T2>& m)
{
    for(auto elem : m)
        cout << elem.first << " | " << elem.second << endl;
}

// --------------------noncopyable--------------------
#include <boost/noncopyable.hpp>
void noncopyableTest()
{
    using namespace boost;

    class do_not_copy:noncopyable{};

    do_not_copy _class1;
    // 以下语句不能通过编译
    // do_not_copy _class2 = _class1;
    // do_not_copy _class2(_class1);
}

// --------------------unused--------------------
#include <boost/core/ignore_unused.hpp>
int ignore_unusedTest(int x, int y)
{
    using namespace boost;
    int i;
    ignore_unused(i, x);
    return y;
    // 未使用变量i和x
}

// --------------------optional--------------------
// 计算倒数
optional<double> calc(int x)
{
    // 条件构造函数
    return optional<double>(x != 0, 1.0 / x);
}
// 计算实数的平方根
optional<double> sqrt_op(double x)
{
    // 条件构造函数
    return optional<double>(x >= 0, std::sqrt(x));
}
void optionalTest()
{
    // 一个未初始化的optional对象
    optional<int> op0;
    optional<int> op1(none);

    // bool测试
    assert(!op0);
    // 比较两个optional对象
    assert(op0 == op1);
    // 获取默认值
    assert(op1.value_or(253) == 253);
    // 使用函数对象
    cout << op1.value_or_eval(
                                [](){ return 857; }
                             ) << endl;
    // 初始化为字符串test，并获取其值
    optional<string> ops("test");
    cout << *ops << endl;

    // 就地创建一个字符串，没有拷贝代价
    ops.emplace("monado", 3);
    assert(*ops == "mon");

    // 容纳一个容器的引用
    vector<int> v(10);
    optional<vector<int>&> opv(v);
    assert(opv);

    // 使用箭头操作符
    opv->push_back(5);
    assert(opv->size() == 11);

    // 重置为初始化状态
    opv = none;
    assert(!opv);

    optional<double> d = calc(10);

    if(d)
    { cout << *d << endl; }
    d = sqrt_op(-10);
    if(!d)
    { cout << "no result" << endl; }
}
// --------------------assign--------------------
void assignTest()
{
    using namespace boost::assign;

    // +=
    vector<int> v;
    set<string> s;
    map<int, string> m;
    // 用+=插入数据
    v += 1, 2, 3, 4, 5, 6*6;
    s += "c", "cpp", "lua", "swift";
    m += std::make_pair(1, "one"), std::make_pair(2, "two");
    printStl(v);
    printStl(m);

    // ()
    vector<int> _v;
    list<string> l;
    set<double> _s;
    map<int, string> _m;
    push_back(_v) (1) (2) (3) (4) (5);
    push_front(l) ("c") ("cpp") ("lua") ("swift");
    insert(_s) (3.14) (0.618) (1.732);
    insert(_m) (1, "one") (2, "two");
    printStl(_v);
    printStl(_m);

    // 初始化容器
    vector<int> _v_ = list_of(1) (2) (3) (4) (5);
    map<int, string> _m_ = map_list_of(1, "one") (2, "two") (3, "three");
    printStl(_v_);
    printStl(_m_);
}
// --------------------expection--------------------
void exceptionTest()
{
    // 定义异常类
    struct my_exception : virtual std::exception, virtual exception {};
    // 异常信息类型
    using err_no = error_info<struct tag_err_no, int>;
    using err_str = error_info<struct tag_err_str, string>;

    try
    {
        try
        {
            // 抛出异常，存储错误码
            throw my_exception() << err_no(10);
        }
        catch(my_exception& e)
        {
            // 获得异常内存储的信息
            cout << *get_error_info<err_no>(e) << endl;
            cout << e.what() << endl;
            e << err_str("other info");  // 向异常追加信息
            throw;  // 再次抛出异常
        }
    }
    catch(my_exception& e)
    {
        cout << *get_error_info<err_str>(e) << endl;
    }
}

int main()
{
    
    // noncopyable
    COUT("noncopyable");
    noncopyableTest();

    // ignore_unusedTest
    COUT("ignore_unusedTest");
    ignore_unusedTest(1,2);
    
    // optional
    COUT("optional");
    optionalTest();
    
    // assign
    COUT("assign");
    assignTest();

    // exception
    COUT("exception");
    exceptionTest();

}
