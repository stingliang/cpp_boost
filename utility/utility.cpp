/*
 * @Name: liangrui
 * @Date: 2021-8-27 11:09:20
 */

// common include
#include "common.h"

// boost test frame
#include <boost/test/unit_test.hpp>

// boost library
#include <boost/noncopyable.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/optional.hpp>
#include <boost/assign.hpp>
#include <boost/exception/all.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

BOOST_AUTO_TEST_SUITE(s_utility)  /* NOLINT */

BOOST_AUTO_TEST_CASE(c_noncopyable) {  /* NOLINT */

    class do_not_copy:boost::noncopyable{};

    do_not_copy _class1;
    // 以下语句不能通过编译
    // do_not_copy _class2 = _class1;
    // do_not_copy _class2(_class1);
}

BOOST_AUTO_TEST_CASE(c_ignore_unused) {  /* NOLINT */
    int i;
    boost::ignore_unused(i);
}

BOOST_AUTO_TEST_CASE(c_optional) {  /* NOLINT */

    // 一个未初始化的optional对象
    boost::optional<int> op0;
    boost::optional<int> op1(boost::none);
    // bool测试
    BOOST_CHECK(!op0);
    // 比较两个optional对象
    BOOST_CHECK(op0 == op1);
    // 获取默认值
    BOOST_CHECK_EQUAL(op1.value_or(253), 253);
    // 使用函数对象
    BOOST_CHECK_EQUAL(op1.value_or_eval([](){ return 857; }), 857);

    // 初始化为字符串test，并获取其值
    boost::optional<std::string> ops("test");
    BOOST_CHECK_EQUAL(*ops, "test");

    // 就地创建一个字符串，没有拷贝代价
    ops.emplace("monado", 3);
    BOOST_CHECK_EQUAL(*ops, "mon");

    // 容纳一个容器的引用
    std::vector<int> v(10);
    boost::optional<std::vector<int>&> opv(v);
    BOOST_CHECK(opv);
    // 使用箭头操作符
    opv->push_back(5);
    BOOST_CHECK_EQUAL(opv->size(), 11);

    // 重置为初始化状态
    opv = boost::none;
    BOOST_CHECK(!opv);

    // 应用于未定义的行为
    int x = 0;
    boost::optional<double> d1(x != 0, 1.0 / x);
    BOOST_CHECK(!d1);
    x = 1;
    boost::optional<double> d2(x != 0, 1.0 / x);
    BOOST_CHECK(d2);
    BOOST_CHECK_EQUAL(*d2, 1);
}

BOOST_AUTO_TEST_CASE(c_assign) {  /* NOLINT */
    // 要使用assign, 必须声明此命名空间
    using namespace boost::assign;

    // +=
    std::vector<int> v;
    std::set<std::string> s;
    std::map<int, std::string> m;
    // 用+=插入数据
    v += 1, 2, 3, 4, 5, 6*6;
    s += "c", "cpp", "lua", "swift";
    m += std::make_pair(1, "one"), std::make_pair(2, "two");
    BOOST_CHECK_EQUAL(printStl(v).str(), "[1][2][3][4][5][36]");
    BOOST_CHECK_EQUAL(printStl(s).str(), "[c][cpp][lua][swift]");
    BOOST_CHECK_EQUAL(printStl(m).str(), "[1: one][2: two]");

    // ()
    std::vector<int> _v;
    std::list<std::string> l;
    std::set<double> _s;
    std::map<int, std::string> _m;
    push_back(_v) (1) (2) (3) (4) (5);
    push_front(l) ("c") ("cpp") ("lua") ("swift");
    insert(_s) (3.14) (0.618) (1.732);
    insert(_m) (1, "one") (2, "two");
    BOOST_CHECK_EQUAL(printStl(_v).str(), "[1][2][3][4][5]");
    BOOST_CHECK_EQUAL(printStl(l).str(), "[swift][lua][cpp][c]");
    BOOST_CHECK_EQUAL(printStl(_m).str(), "[1: one][2: two]");

    // 初始化容器
    std::vector<int> _v_ = list_of(1) (2) (3) (4) (5);
    std::map<int, std::string> _m_ = map_list_of(1, "one") (2, "two") (3, "three");
    BOOST_CHECK_EQUAL(printStl(_v_).str(), "[1][2][3][4][5]");
    BOOST_CHECK_EQUAL(printStl(_m_).str(), "[1: one][2: two][3: three]");
}

BOOST_AUTO_TEST_CASE(c_exception) {  /* NOLINT */
    // 定义异常类
    struct my_exception : virtual std::exception, virtual boost::exception {
        // 重写what函数
        const char* what() const noexcept override
        {
            return "A custom error occurred.";
        }
    };
    // 异常信息类型
    using err_no = boost::error_info<struct tag_err_no, int>;
    using err_str = boost::error_info<struct tag_err_str, std::string>;

    try
    {
        try
        {
            // 抛出异常，存储错误码
            auto e = my_exception();
            e << err_no(10) << err_str("Something wrong!");
            throw std::move(e);
        }
        catch(my_exception& e)
        {
            // 获得异常内存储的信息
            BOOST_CHECK_EQUAL(*boost::get_error_info<err_no>(e), 10);
            BOOST_CHECK_EQUAL(*boost::get_error_info<err_str>(e), "Something wrong!");
            BOOST_CHECK_EQUAL(e.what(), "A custom error occurred.");
            e << err_str("But i don't know what happend...");  // 向异常追加信息
            throw;  // 再次抛出异常
        }
    }
    catch(my_exception& e)
    {
        BOOST_CHECK_EQUAL(*boost::get_error_info<err_no>(e), 10);
        BOOST_CHECK_EQUAL(*boost::get_error_info<err_str>(e), "But i don't know what happend...");
    }
}

BOOST_AUTO_TEST_CASE(c_uuid) {  /* NOLINT */

    //第一个()构造生成器对象，第二个()调用对象的函数
    boost::uuids::uuid u = boost::uuids::nil_generator()();
    BOOST_CHECK(u.is_nil());
    //nil_uuid()是一个内联函数，相当于直接调用Nil生成器
    u = boost::uuids::nil_uuid();
    BOOST_CHECK(u.is_nil());

    //声明字符串生成器对象
    boost::uuids::string_generator sgen;
    //使用字符串构造
    std::stringstream ss;
    boost::uuids::uuid u1 = sgen("0123456789abcdef0123456789abcdef");
    ss << u1;
    BOOST_CHECK_EQUAL(ss.str(), "01234567-89ab-cdef-0123-456789abcdef");
    boost::uuids::uuid u2 = sgen("01234567-89ab-cdef-0123-456789abcdef");
    ss.str("");
    ss << u2;
    BOOST_CHECK_EQUAL(ss.str(), "01234567-89ab-cdef-0123-456789abcdef");

    // 名字生成器
    boost::uuids::uuid www_xxx_com = boost::uuids::string_generator()("0123456789abcdef0123456789abcdef");
    //使用uuid构造
    boost::uuids::name_generator ngen(www_xxx_com);
    boost::uuids::uuid u3 = ngen("mario");
    //为名字生成UUID，version是基于SHA1算法
    BOOST_CHECK_EQUAL(u3.version(), boost::uuids::uuid::version_name_based_sha1);
    boost::uuids::uuid u4 = ngen("link");
    ss.str("");
    ss << u3;
    BOOST_CHECK_EQUAL(ss.str(), "c7c194f0-168b-5402-9cae-338f72c43671");
    ss.str("");
    ss << u4;
    BOOST_CHECK_EQUAL(ss.str(), "f1a16014-5e8c-5bf1-bc26-42a90d9d8941");
}

BOOST_AUTO_TEST_SUITE_END()  /* NOLINT */