/*
 * @Name: liangrui
 * @Date: 2020-12-28 11:09:20
 * @LastEditTime: 2020-12-29 11:26:38
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
using namespace boost;

int main()
{
    // ----------
    // shared_ptr
    // ----------

    // 接管原始指针
    shared_ptr<int> sp(new int(10));

    // 此时shared_ptr是指针唯一的额持有者
    assert(sp.unique());

    // 拷贝构造
    shared_ptr<int> sp1(sp);

    // shared_ptr<int> sp1 = sp;
    assert(sp == sp1 && sp.use_count() == 2);

    // 使用解引用操作符操作被指对象
    *sp1 = 100;

    // 停止使用shared_ptr
    sp.reset();

    // 使用工厂函数构造
    auto sp2 = make_shared<string>("make shared");
    auto sp3 = make_shared<vector<int>>(10, 2);
    assert(sp3 -> size() == 10);

    // ----------
    // weak_ptr
    // ----------

    shared_ptr<int> sp4(new int(10));
    assert(sp4.use_count() == 1);

    // 从shared_ptr构造
    weak_ptr<int> wp(sp4);
    assert(wp.use_count() == 1);
    assert(!wp.empty());

    // 从weak_ptr获取shared_ptr
    if(!wp.expired())
    {
        // 获得一个shared_ptr
        shared_ptr<int> sp = wp.lock();
        *sp = 100;
        assert(wp.use_count() == 2);
    }
    assert(wp.use_count() == 1);

    // 智能指针失效
    sp4.reset();
    assert(wp.expired());

    // weak_ptr将获得一个空指针
    assert(!wp.lock());

    // -------------
    // unique_ptr
    // -------------

    // 空构造
    std::unique_ptr<int> up1();
    std::unique_ptr<int> up2(nullptr);

    // 接管已有指针
    std::unique_ptr<int> up3(new int);

    // 移动构造
    std::unique_ptr<int> up4(new int);
    // std::unique_ptr<int> up5(up4);  //错误，堆内存不共享
    std::unique_ptr<int> up5(std::move(up4));  //正确，调用移动构造函数

    // 使用工厂函数
    auto up6 = std::make_unique<int>(10); //c++14
    assert(up6 && *up6 == 10);
    auto up7 = boost::make_unique<int>(10); //boost::make_unique
    assert(up7 && *up7 == 10);

    // 定制删除器
    struct myDel
    {
        void operator()(int *p) {
            cout << "delete." << endl;
        }
    };
    std::unique_ptr<int, myDel> up8(new int, myDel());


    // -------------
    // 对象自我管理
    // -------------

    class self_shared: public enable_shared_from_this<self_shared>
    {
    public:
        self_shared(int n):num(n){}
        int num;
        void print()
        {
            cout << "self_shared:" << num << endl;
        }
    };

    auto sp5 = make_shared<self_shared>(313);
    sp5 -> print();

    auto sp6 = sp5->shared_from_this();
    sp6 -> num = 100;
    sp6 -> print();

    // 错误用法
    // self_shared ss(100);
    // auto p = ss.shared_from_this();
}