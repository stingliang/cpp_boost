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
#include <boost/function.hpp>
#include <boost/signals2.hpp>

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
        int memberFunc(int a, int b) const {
            boost::ignore_unused(m_a, m_b); // for IDEA warnning
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

    static int f(int a, int b) {
        return a + b;
    }

    static void slots1(int& i) {
        i++;
    }

    static void slots2(int& i) {
        i++;
    }

    // 定义一个合并器
    template<typename T>
    struct Combiner {
        // 返回类型
        typedef std::vector<T> result_type;
        // 重载()
        template<typename Iterator>
        result_type operator()(Iterator begin, Iterator end) {
            if (begin == end) {
                return result_type();
            }
            return result_type(begin, end);
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

    // 简单声明一个function
    boost::function<int()> func;
    // 此时func不持有任何对象
    BOOST_CHECK(!func);

    // 使用decltype直接推到参数类型
    boost::function<decltype(f)> func2;

    // 验证已存储函数f
    func2 = f;
    BOOST_CHECK(func2.contains(&f));

    // 直接转换为bool判断是否包含一个函数对象
    BOOST_CHECK(func2);

    // 清空func2
    func2.clear();
    BOOST_CHECK(func2.empty());

    // 配合bind存储成员函数
    FuncTest ft;
    boost::function<int(int, int)> func3;
    func3 = boost::bind(&FuncTest::memberFunc, &ft, _1, _2);
    BOOST_CHECK_EQUAL(func3(1, 2), 3);
}

BOOST_AUTO_TEST_CASE(c_signals2) {  /* NOLINT */

    using namespace boost::signals2;

    {   // 基本用法，指定插槽类型void()，其他模板参数使用默认值

        signal<void(int&)> sig;
        BOOST_CHECK(sig.empty());
        /*signal<void()> sig2;
        BOOST_CHECK(sig2.empty());*/

        // 使用connect()来连接插槽，使用operator()来产生信号
        int i(0);
        // 传统用法
        sig.connect(&slots1);
        sig.connect(&slots2);
        BOOST_CHECK_EQUAL(sig.num_slots(), 2);
        sig(i);
        // 也可以使用匿名函数包装
        /*sig2.connect([&](){ slots1(i); });
        sig2.connect([&](){ slots2(i); });
        BOOST_CHECK_EQUAL(sig2.num_slots(), 2);
         sig2();*/

        BOOST_CHECK_EQUAL(i, 2);
    }

    {    // 使用组号

        signal<void(int&)> sig;
        int i(0);

        // 最后一个被调用
        sig.connect(&slots1, at_back);
        // 第一个被调用
        sig.connect(&slots2, at_front);
        sig(i);

        BOOST_CHECK_EQUAL(i, 2);
    }

    {    // 获取返回值

        signal<int(int&)> sig;
        int i(0);

        sig.connect([&](int& i)->int{ return i; });
        sig.connect([&](int& i)->int{ return i + 1; });
        // 返回值是optional对象
        boost::optional result = sig(i);
        // 返回的是最后被调用函数的返回值
        BOOST_CHECK_EQUAL(*result, 1);
    }

    {    // 合并器

        /* 定义一个合并器
        template<typename T>
        struct Combiner {
            // 返回类型
            typedef std::vector<T> result_type;
            // 重载()
            template<typename Iterator>
            result_type operator()(Iterator begin, Iterator end) {
                if (begin == end) {
                    return result_type();
                }
                return result_type(begin, end);
            }
        };*/
        signal<int(int&), Combiner<int>> sig;
        int i(0);

        sig.connect([&](int& i)->int{ return i; });
        sig.connect([&](int& i)->int{ return i + 1; });
        // 返回值是合并器()的返回类型，这里为std::vector<int>，可以使用auto推导
        auto result = sig(i);
        BOOST_CHECK_EQUAL(printStl(result).str(), "[0][1]");
    }

    {   // 灵活管理插槽

        signal<int(int&), Combiner<int>> sig;
        int i(0);

        connection c1 = sig.connect([&](int i)->int{ return i; });
        connection c2 = sig.connect([&](int i)->int{ return i; });
        connection c3 = sig.connect([&](int i)->int{ return i; });
        BOOST_CHECK_EQUAL(sig.num_slots(), 3);

        // 断开c1的连接
        c1.disconnect();
        BOOST_CHECK_EQUAL(sig.num_slots(), 2);
        BOOST_CHECK(!c1.connected());
        BOOST_CHECK(c2.connected());

        // 使用RAII机制临时连接信号
        {
            // 进入局部作用域，建立连接
            scoped_connection sc = sig.connect([&](int i)->int{ return i; });
            BOOST_CHECK_EQUAL(sig.num_slots(), 3);
        }
        // 离开局部作用域，连接自动断开
        BOOST_CHECK_EQUAL(sig.num_slots(), 2);

        // 阻塞连接
        {
            // 阻塞c2
            shared_connection_block block(c2);
            // 仍有两个连接
            BOOST_CHECK_EQUAL(sig.num_slots(), 2);
            // c2被阻塞
            BOOST_CHECK(c2.blocked());
            // 只有c3会被调用
            auto result = sig(i);
            BOOST_CHECK_EQUAL(printStl(result).str(), "[0]");
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()  /* NOLINT */