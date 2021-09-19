//
// Created by Liang on 2021/9/11.
//

// common include
#include "common.h"

// boost test frame
#include <boost/test/unit_test.hpp>

// boost library
#include <boost/atomic.hpp>

BOOST_AUTO_TEST_SUITE(s_concurrent_programming)  /* NOLINT */

BOOST_AUTO_TEST_CASE(c_atomic) {  /* NOLINT */

    boost::atomic<int> i(10);
    BOOST_CHECK_EQUAL(i, 10);
    // should not be used
    // boost::atomic<double> d;

    i += 1;
    BOOST_CHECK(i == 11);
    i.store(0);
    BOOST_CHECK_EQUAL(i.load(), 0);
}

BOOST_AUTO_TEST_SUITE_END()  /* NOLINT */