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

BOOST_AUTO_TEST_SUITE_END()  /* NOLINT */