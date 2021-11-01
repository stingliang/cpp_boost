//
// Created by Liang on 2021/9/11.
//

// common include
#include "common.h"

// boost test frame
#include <boost/test/unit_test.hpp>

// boost library
#include <boost/thread.hpp>
#include <boost/thread/with_lock_guard.hpp>

BOOST_AUTO_TEST_SUITE(s_concurrent_programming)  /* NOLINT */

BOOST_AUTO_TEST_CASE(c_mutex) {  /* NOLINT */
    {
        boost::mutex mu;
        mu.lock();
        //  critical resource
        mu.unlock();
    }

    {
        boost::timed_mutex mu;
        auto flag = mu.try_lock_for(boost::chrono::milliseconds(500));
        if(flag) {
            //  critical resource
            mu.unlock();
        }
    }
}

BOOST_AUTO_TEST_CASE(c_lock_guard) {  /* NOLINT */
    boost::mutex mu;
    {
        boost::lock_guard<boost::mutex> g(mu);
        //  critical resource
    }

    {
        boost::with_lock_guard(mu, [](){
            //  critical resource
        });
    }
}

BOOST_AUTO_TEST_CASE(c_thread_object) {  /* NOLINT */
    using namespace std::chrono_literals;

    boost::thread t1, t2;
    BOOST_TEST_MESSAGE(t1.get_id());
    BOOST_CHECK_EQUAL(t1.get_id(), t2.get_id());

    BOOST_TEST_MESSAGE("Waiting for 5 seconds ...");
    std::this_thread::sleep_for(5s);    // C++ 标准库用法
    BOOST_TEST_MESSAGE("Waiting for 5 seconds ...");
    boost::this_thread::sleep_for(5_s);     // Boost库用法
}

BOOST_AUTO_TEST_SUITE_END()  /* NOLINT */