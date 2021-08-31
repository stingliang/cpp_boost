//
// Created by Liang on 2021/8/31.
//

// common include
#include "common.h"

// boost test frame
#include <boost/test/unit_test.hpp>

// boost library
#include <boost/ref.hpp>
#include <boost/bind.hpp>

struct FunctionFixture {
    FunctionFixture() = default;
    ~FunctionFixture() = default;

    static int twoParamsFunc(int a, int b) {   // 二元函数
        return a + b;
    }

    static int tripParamsFunc(int a, int b, int c) {   // 三元函数
        return a + b * c;
    }

    struct FuncTest {
        int memberFunc(int a, int b) {
            return a + b;
        }

        int m_a = 0;
        int m_b = 1;
    };

    struct func {
        int operator()(int a, int b) {
            return a + b;
        }
    };
};

BOOST_FIXTURE_TEST_SUITE(s_functions, FunctionFixture)  /* NOLINT */

BOOST_AUTO_TEST_CASE(c_ref) {  /* NOLINT */

    std::string str("test reference wrapper");

    {   // reference_wrapper的基本用法
        int x = 10;
        // 包装int类型的引用
        boost::reference_wrapper<int> rw(x);
        // 隐式类型转换
        BOOST_CHECK_EQUAL(x, rw);
        // 显式类型转换
        (int &) rw = 100;
        // 隐式类型转换
        BOOST_CHECK_EQUAL(x, 100);
        // 拷贝构造
        boost::reference_wrapper<int> rw_copy(rw);
        BOOST_CHECK_EQUAL(rw_copy.get(), 100);
        // 指针用法
        boost::reference_wrapper<std::string> rws(str);
        BOOST_CHECK_EQUAL(*rws.get_pointer(), str);
        BOOST_CHECK_EQUAL(rws.get().size(), str.size());
    }

    {   // 工厂函数
        double x = 2.71828;
        auto rw = boost::cref(x);
        BOOST_CHECK_EQUAL(typeid(rw).name(), "class boost::reference_wrapper<double const >");

        auto rws = boost::ref(str);
        BOOST_CHECK_EQUAL(typeid(rws).name(),
                          "class boost::reference_wrapper"
                          "<class std::basic_string<char,struct std::char_traits<char>,"
                          "class std::allocator<char> > >");
    }
}

BOOST_AUTO_TEST_CASE(c_bind) {  /* NOLINT */

    // 绑定普通函数的用法
    BOOST_CHECK_EQUAL(boost::bind(twoParamsFunc, 1, 2)(), twoParamsFunc(1, 2));
    BOOST_CHECK_EQUAL(boost::bind(tripParamsFunc, 1, 2, 3)(), tripParamsFunc(1, 2, 3));

    // 占位符的用法
    int x(1), y(2), z(3);
    BOOST_CHECK_EQUAL(boost::bind(twoParamsFunc, _1, 9)(x), twoParamsFunc(x, 9));
    BOOST_CHECK_EQUAL(boost::bind(twoParamsFunc, _1, _2)(x, y), twoParamsFunc(x, y));
    BOOST_CHECK_EQUAL(boost::bind(twoParamsFunc, _2, _1)(x, y), twoParamsFunc(y, x));
    BOOST_CHECK_EQUAL(boost::bind(twoParamsFunc, _1, _1)(x, y), twoParamsFunc(x, x));   // y参数被忽略
    BOOST_CHECK_EQUAL(boost::bind(tripParamsFunc, _1, 8, _2)(x, y), tripParamsFunc(x, 8, y));
    BOOST_CHECK_EQUAL(boost::bind(tripParamsFunc, _3, _2, _2)(x, y, z), tripParamsFunc(z, y, y));   // x参数被忽略

    FuncTest ft, &rft = ft;
    FuncTest *p = &rft;

    // 操作成员函数
    BOOST_CHECK_EQUAL(boost::bind(&FuncTest::memberFunc, ft, _1, 20)(10), ft.memberFunc(10, 20));
    BOOST_CHECK_EQUAL(boost::bind(&FuncTest::memberFunc, rft, _2, _1)(10, 20), rft.memberFunc(20, 10));
    BOOST_CHECK_EQUAL(boost::bind(&FuncTest::memberFunc, p, _1, _2)(10, 20), p->memberFunc(10, 20));

    // 操作成员变量
    BOOST_CHECK_EQUAL(boost::bind(&FuncTest::m_a, _1)(ft), ft.m_a); // 使用占位符
    BOOST_CHECK_EQUAL(boost::bind(&FuncTest::m_a, rft)(), rft.m_a); // 直接绑定

    // 操作函数对象
    BOOST_CHECK(boost::bind(std::greater<int>(), _1, 10)(20));
    BOOST_CHECK_EQUAL(boost::bind(std::plus<int>(), _1, _2)(1, 2), 3);
    BOOST_CHECK_EQUAL(boost::bind(std::modulus<int>(), _1, 3)(3), 0);
    BOOST_CHECK_EQUAL(boost::bind<int>(func(),_1,_2)(10,20), 30);
}

BOOST_AUTO_TEST_CASE(c_function) {  /* NOLINT */

}

BOOST_AUTO_TEST_CASE(c_signals2) {  /* NOLINT */

}

BOOST_AUTO_TEST_SUITE_END()  /* NOLINT */