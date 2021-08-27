/*
 * @Name: liangrui
 * @Date: 2021-8-27 11:09:20
 */

// common include
#include "common.h"

// boost test frame
#include <boost/test/unit_test.hpp>

// boost library
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/utility/string_ref.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>

BOOST_AUTO_TEST_SUITE(s_string_algorithm)  /* NOLINT */

BOOST_AUTO_TEST_CASE(c_lexical_cast) {  /* NOLINT */

    // string -> int/long
    auto x = boost::lexical_cast<int>("100");
    auto y = boost::lexical_cast<long>("2000");
    BOOST_CHECK_EQUAL(x, 100);
    BOOST_CHECK_EQUAL(y, 2000);

    // string -> float/double
    auto pai = boost::lexical_cast<float>("3.14159e5");
    auto e = boost::lexical_cast<double>("2.71828");
    BOOST_CHECK_EQUAL(pai, 3.14159e5);
    BOOST_CHECK_EQUAL(e, 2.71828);

    // int -> string
    auto str = boost::lexical_cast<std::string>(456);
    BOOST_CHECK_EQUAL(str, "456");
}

BOOST_AUTO_TEST_CASE(c_format) {  /* NOLINT */

    // 简单例子
    BOOST_CHECK_EQUAL((boost::format("%d + %d = %d") %1 %2 %(1+2)).str(), "1 + 2 = 3");
    boost::format fmt("( %1% + %2% ) * %2% = %3%");
    fmt %2 %5;
    fmt %((2+5)*5);
    BOOST_CHECK_EQUAL(fmt.str(), "( 2 + 5 ) * 5 = 35");

    // 格式化语法
    // %05d：输出宽度为5的整数，不足位用0填充
    // %-8.3f：输出左对齐，总宽度为8，小数位为3位的浮点数
    // %10s：输出10位的字符串，不足位用空格填充
    // %05X：输出宽度为5的大写十六进制整数，不足位用0填充
    boost::format fmt1("%05d\t%-8.3f\t% 10s\t%05x");
    std::stringstream ss;
    ss << fmt1 %62 %2.236 %"123456789" %48;
    BOOST_CHECK_EQUAL(ss.str(), R"(00062	2.236   	 123456789	00030)");

    // %|spec|：与printf格式选项的功能相同，但其两边增加了竖线分隔，可以更好地区分格式化选项与普通字符
    // %N%：标记第N个参数，相当于占位符，不带任何其他的格式化选项
    boost::format fmt2("%|05d|\t%|-8.3f|\t%| 10s|\t%|05x|");
    ss.str("");
    ss << fmt2 %62 %2.236 %"123456789" %48;
    BOOST_CHECK_EQUAL(ss.str(), R"(00062	2.236   	 123456789	00030)");
}

BOOST_AUTO_TEST_CASE(c_string_ref) {  /* NOLINT */
    const char * ch = "Hello,World!";

            // 标准字符串，有拷贝成本
            std::string str(ch);
            // 直接构造
            boost::string_ref s0("Hello,World!");
            BOOST_TEST_MESSAGE(s0);
            // 字符数组构造，0拷贝
            boost::string_ref s1(ch);
            // 标准字符串构造，0拷贝
            boost::string_ref s2(str);
            BOOST_CHECK_EQUAL(s0, s1);
            BOOST_CHECK_EQUAL(s1, s2);
            BOOST_CHECK_EQUAL(s0, ch);
            BOOST_CHECK_EQUAL(s2, str);

            // 截取部分字符串构造
            boost::string_ref s3(ch, 4);
            BOOST_CHECK_EQUAL(s3, str.substr(0,4));

            // 空构造
            boost::string_ref s4, s5;
            s4 = ch;
            s5 = str;
            BOOST_CHECK_EQUAL(s4, s5);
}

BOOST_AUTO_TEST_CASE(c_string_find_replace_algo) {  /* NOLINT */

    // 大小写转换
    std::string str("Hello,World!");
    // 返回其拷贝
    BOOST_CHECK_EQUAL(boost::to_upper_copy(str), "HELLO,WORLD!");
    // 改变其自身
    boost::to_upper(str);
    BOOST_CHECK_EQUAL(str, "HELLO,WORLD!");

    // 字符串查找算法
    boost::format fmt("|%s|.pos = %d\n");
    boost::string_ref str_1 = "Long long ago, there was a king.";
    // 找第一次出现
    auto rge = find_first(str_1, "long");
    BOOST_CHECK_EQUAL(rge.begin(), str_1.begin() + 5);
    // 大小写无关找第一次出现
    rge = ifind_first(str_1, "long");
    BOOST_CHECK_EQUAL(rge.begin(), str_1.begin());
    // 找第三次出现
    rge = find_nth(str_1, "ng", 2);
    BOOST_CHECK_EQUAL(rge.begin(), str_1.begin() + 29);
    // 取前四个字符
    rge = find_head(str_1, 4);
    BOOST_CHECK_EQUAL(std::string(rge.begin(), rge.end()), "Long");
    // 取尾5个字符
    rge = find_tail(str_1, 5);
    BOOST_CHECK_EQUAL(std::string(rge.begin(), rge.end()), "king.");
    // not found
    rge = find_first(str_1, "samus");
    BOOST_CHECK(rge.empty());
    BOOST_CHECK(!rge);

    // 替换和删除
    std::string str_2 = "Samus beat the monster.";
    // replace
    // 返回其拷贝
    BOOST_CHECK_EQUAL(boost::replace_first_copy(str_2, "Samus", "samus"), "samus beat the monster.");
    // 改变其自身
    boost::replace_last(str_2, "beat", "kill");
    BOOST_CHECK_EQUAL(str_2, "Samus kill the monster.");
    boost::replace_tail(str_2, 9, "ridley.");
    BOOST_CHECK_EQUAL(str_2, "Samus kill theridley.");

    // delete
    BOOST_CHECK_EQUAL(boost::ierase_all_copy(str_2, "samus "), "kill theridley.");
    BOOST_CHECK_EQUAL(boost::replace_nth_copy(str_2, "l", 1, "L"), "Samus kilL theridley.");
    BOOST_CHECK_EQUAL(boost::erase_tail_copy(str_2, 8), "Samus kill th");
}

BOOST_AUTO_TEST_CASE(c_string_split_join_algo) {  /* NOLINT */

    // 使用boost::assign填充容器, 相关链接(https://wowsting.club/cpp_boost/utility/assign.html)
    using namespace boost::assign;

    std::string str = "Samus, Link.Zelda::Mario-Luigi+zelda";
    std::deque<std::string> d;

    // 大小写无关查找， 将查找结果填充进标准容器
    boost::ifind_all(d, str, "zELDA");
    BOOST_CHECK_EQUAL(d.size(), 2);
    std::stringstream ss;
    for(auto const& x: d)
    {
        ss << "[" << x << "]";
    }
    BOOST_CHECK_EQUAL(ss.str(), "[Zelda][zelda]");

    // 分割字符串
    std::list<boost::iterator_range<std::string::iterator>> l;
    boost::split(l, str, boost::is_any_of(",.:-+"));
    ss.str("");
    for(auto const& x: l)
    {
        ss << "[" << x << "]";
    }
    BOOST_CHECK_EQUAL(ss.str(), "[Samus][ Link][Zelda][][Mario][Luigi][zelda]");
    // 忽略空串分割
    l.clear();
    boost::split(l, str, boost::is_any_of(",.:-+"), boost::token_compress_on);
    ss.str("");
    for(auto const& x: l)
    {
        ss << "[" << x << "]";
    }
    BOOST_CHECK_EQUAL(ss.str(), "[Samus][ Link][Zelda][Mario][Luigi][zelda]");

    // 拼接字符串
    std::vector<std::string> v = list_of("Samus")("Link")("Zelda")("Mario");
    boost::join(v, "+");
    BOOST_CHECK_EQUAL(boost::join_if(v, "**", [](boost::string_ref s){return contains(s, "a");}),
                      "Samus**Zelda**Mario");
}

BOOST_AUTO_TEST_SUITE_END()  /* NOLINT */