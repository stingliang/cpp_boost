/*
 * @Name: liangrui
 * @Date: 2021-8-30 14:58:20
 */

// common include
#include "common.h"

// boost test frame
#include <boost/test/unit_test.hpp>

// boost library
#include <boost/assign.hpp>
#include <boost/array.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/utility.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/multi_array.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

BOOST_AUTO_TEST_SUITE(s_data_struct)  /* NOLINT */

BOOST_AUTO_TEST_CASE(c_array) {  /* NOLINT */

    // 一个大小为10的数组
    boost::array<int, 10> ar{0};
    // 使用[]操作符访问元素
    BOOST_CHECK_EQUAL(ar[0], 0);
    ar.back() = 10; // 访问最后一个元素
    BOOST_CHECK_EQUAL(ar[9], 10);
    BOOST_CHECK_EQUAL(printStl(ar).str(), "[0][0][0][0][0][0][0][0][0][10]");
}

BOOST_AUTO_TEST_CASE(c_dynamic_bitset) {  /* NOLINT */

    boost::dynamic_bitset<> db1;   // 空的dyncmic_bitset
    boost::dynamic_bitset<> db2(10);   // 大小为10的dyncmic_bitset
    boost::dynamic_bitset<> db3(0x16, BOOST_BINARY(10101));    // 大小为22的dyncmic_bitset
    boost::dynamic_bitset<> db4(std::string("100101010011101001001"));   // 字符串构造
    boost::dynamic_bitset<> db5(db3);  // 拷贝构造

    std::stringstream ss;
    ss << std::hex << db4.to_ulong();
    BOOST_CHECK_EQUAL(ss.str(), "12a749");
}

BOOST_AUTO_TEST_CASE(c_unordered) {  /* NOLINT */

    using namespace boost::assign;  // 必须使用此命名空间来使运算符重载生效

    // 散列集合用法示例
    boost::unordered_set<int> s = {1, 2, 3, 4, 5};
    BOOST_CHECK_EQUAL(printStl(s).str(), "[5][4][3][2][1]");
    // 获取容器大小
    BOOST_CHECK_EQUAL(s.size(), 5);
    s.clear();  // 清空集合
    BOOST_CHECK(s.empty());  // 集合判空
    // 插入操作
    s.insert(8);
    s.insert(45);
    BOOST_CHECK_EQUAL(s.size(), 2);
    // 查找元素
    BOOST_CHECK_EQUAL(*s.find(8), 8);
    // 删除元素
    s.erase(45);
    BOOST_CHECK_EQUAL(printStl(s).str(), "[8]");

    // 散列映射用法，使用assign初始化
    boost::unordered_map<int, std::string> um = map_list_of(1, "one")(2, "two")(3, "three");
    um.insert(std::make_pair(10, "ten"));
    BOOST_CHECK_EQUAL(um[10], "ten");
    um[11] = "eleven";  // 使用关联数组，注意这里的[x]和普通数组[x]的区别
    BOOST_CHECK_EQUAL(um[11], "eleven");

    // 遍历容器
    std::stringstream ss;
    for (auto const& elem: um) {
        ss << "[" << elem.first << ": " << elem.second << "]";
    }
    BOOST_CHECK_EQUAL(ss.str(), "[11: eleven][10: ten][3: three][2: two][1: one]");
}

BOOST_AUTO_TEST_CASE(c_multi_array) {  /* NOLINT */

    // 三维数组, 相当于int[2][3][4]
    boost::multi_array<int, 3> ma(boost::extents[2][3][4]);
    // 获取维数
    BOOST_CHECK_EQUAL(ma.num_dimensions(), 3);
    // 获取多维数组指针
    auto shape = ma.shape();
    BOOST_CHECK_EQUAL(shape[0], 2);
    BOOST_CHECK_EQUAL(shape[1], 3);
    BOOST_CHECK_EQUAL(shape[2], 4);

    // 给元素赋值, []运算符在DEBUG模式下提供越界检查
    int v(0);
    for (int i(0); i < shape[0]; i++) {
        for (int j(0); j < shape[1]; j++) {
            for (int k(0); k < shape[2]; k++) {
                ma[i][j][k] = v++;
            }
        }
    }

    // 统计元素个数
    BOOST_CHECK_EQUAL(ma.num_elements(), 24);

    // 遍历多维数组
    std::stringstream ss;
    for (auto i(0); i < shape[0]; i++) {
        for (auto j(0); j < shape[1]; j++) {
            for (auto k(0); k < shape[2]; k++) {
                ss << "[" << ma[i][j][k] << "]";
            }
        }
    }
    BOOST_CHECK_EQUAL(ss.str(),
                      "[0][1][2][3][4][5][6][7][8][9][10][11][12][13][14][15][16][17][18][19][20][21][22][23]");
}

BOOST_AUTO_TEST_CASE(c_property_tree) {  /* NOLINT */

    boost::property_tree::ptree pt;
    read_json("info.json", pt);
    BOOST_CHECK_EQUAL(pt.get<std::string>("name.firstname"), "sting");
    pt.add("address.conutry", "China");
    pt.add("address.province", "SiChuan");
    pt.add("address.city", "ChengDu");
    auto child = pt.get_child("address");
    std::stringstream ss;
    for(auto const& x: child) {
        ss << x.first << ": ";
        ss << x.second.get_value<std::string>();
    }
    BOOST_CHECK_EQUAL(ss.str(), "conutry: China" "province: SiChuan" "city: ChengDu");
    // 打印JSON串到标准输出
    // write_json(std::cout, pt);
}

BOOST_AUTO_TEST_SUITE_END()  /* NOLINT */