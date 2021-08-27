/*
 * @Name: liangrui
 * @Date: 2020-12-28 11:09:20
 */

// common include
#include "common.h"

// boost test frame
#include <boost/test/unit_test.hpp>

// boost library
#include <boost/smart_ptr.hpp>
#include <boost/smart_ptr/make_unique.hpp>

BOOST_AUTO_TEST_SUITE(s_memory)  /* NOLINT */

BOOST_AUTO_TEST_CASE(c_shared_ptr) {  /* NOLINT */

    // 接管原始指针
    boost::shared_ptr<int> sp(new int(10));

    // 此时shared_ptr是指针唯一的额持有者
    BOOST_CHECK(sp.unique());

    // 拷贝构造
    boost::shared_ptr<int> sp1(sp);
    // shared_ptr<int> sp1 = sp;
    BOOST_CHECK(sp == sp1 && sp.use_count() == 2);

    // 使用解引用操作符操作被指对象
    *sp1 = 100;

    // 停止使用shared_ptr
    sp.reset();

    // 使用工厂函数构造
    auto sp2 = boost::make_shared<std::string>("make shared");
    auto sp3 = boost::make_shared<std::vector<int>>(10, 2);
    BOOST_CHECK(sp3 -> size() == 10);
}

BOOST_AUTO_TEST_CASE(c_weak_ptr) {  /* NOLINT */

    boost::shared_ptr<int> sp4(new int(10));
    BOOST_CHECK_EQUAL(sp4.use_count(), 1);

    // 从shared_ptr构造
    boost::weak_ptr<int> wp(sp4);
    BOOST_CHECK_EQUAL(wp.use_count(), 1);
    BOOST_CHECK(!wp.empty());

    // 从weak_ptr获取shared_ptr
    if(!wp.expired())
    {
        // 获得一个shared_ptr
        boost::shared_ptr<int> sp = wp.lock();
        *sp = 100;
        BOOST_CHECK_EQUAL(wp.use_count(), 2);
    }
    BOOST_CHECK_EQUAL(wp.use_count(), 1);

    // 智能指针失效
    sp4.reset();
    BOOST_CHECK(wp.expired());

    // weak_ptr将获得一个空指针
    BOOST_CHECK_EQUAL(wp.lock(), nullptr);
}

BOOST_AUTO_TEST_CASE(c_unique_ptr) {  /* NOLINT */

    // 空构造
    // std::unique_ptr<int> up1();
    std::unique_ptr<int> up2(nullptr);

    // 接管已有指针
    std::unique_ptr<int> up3(new int);

    // 移动构造
    std::unique_ptr<int> up4(new int);
    // std::unique_ptr<int> up5(up4);  //错误，堆内存不共享
    std::unique_ptr<int> up5(std::move(up4));  //正确，调用移动构造函数

    // 使用工厂函数
    auto up6 = std::make_unique<int>(10); //c++14
    BOOST_CHECK_EQUAL(*up6, 10);
    auto up7 = boost::make_unique<int>(10); //boost::make_unique
    BOOST_CHECK_EQUAL(*up7, 10);

    // 定制删除器
    struct myDel {
        void operator()(int const* p) {
            // std::cout << "delete." << std::endl;
            delete[] p;
        }
    };
    std::unique_ptr<int, myDel> up8(new int, myDel());
}

BOOST_AUTO_TEST_SUITE_END()  /* NOLINT */