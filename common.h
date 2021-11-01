//
// Created by Liang on 2021/8/27.
//

#ifndef CPP_BOOST_COMMON_H
#define CPP_BOOST_COMMON_H

// c++ stdlib
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <string>
#include <functional>
#include <cmath>
#include <list>
#include <deque>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>

///
/// \tparam T1
/// \tparam T2
/// \param std::map
/// \return std::stringstream
/// \brief print stl map
template<typename T1, typename T2>
std::stringstream printStl(std::map<T1,T2>& m)
{
    std::stringstream ss;
    for(auto const& elem : m) {
        ss << "[" << elem.first << ": " << elem.second << "]";
    }
    return ss;
}

///
/// \tparam T
/// \param stl/boost container
/// \return std::stringstream
/// \brief print stl set
template<typename T>
std::stringstream printStl(T& s)
{
    std::stringstream ss;
    for(auto const& elem : s) {
        ss << "[" << elem << "]";
    }
    return ss;
}

boost::chrono::seconds operator"" _s(unsigned long long n) { return boost::chrono::seconds(n); }

#endif //CPP_BOOST_COMMON_H
