/*
 * @Name: liangrui
 * @Date: 2021-8-30 17:11:20
 */

#define TRUE true

#include <boost/test/unit_test.hpp>

struct sample_fixture {
    sample_fixture() {
        member_data = 0;
    }
    ~sample_fixture() = default;

    int member_data;
    int member_function() const {
        return member_data;
    }
};

BOOST_FIXTURE_TEST_SUITE(s_test_fixture, sample_fixture)  /* NOLINT */

BOOST_AUTO_TEST_CASE(c_fixture_case) {  /* NOLINT */
    /*这里的测试用例在运行前会执行sample_fixture的构造函数并在结束时执
     * 行析构函数测试用例中可以使用sample_fixture的成员函数和数据成员*/
    BOOST_CHECK_EQUAL(member_data, 0);
    BOOST_CHECK_EQUAL(member_function(), 0);
}

BOOST_AUTO_TEST_SUITE_END()  /* NOLINT */