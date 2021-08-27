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

///
/// \tparam T
/// \param std::vector
/// \return std::stringstream
/// \brief print stl vector
template<typename T>
std::stringstream printStl(std::vector<T>& v)
{
    std::stringstream ss;
    for(auto const& elem : v) {
        ss << "[" << elem << "]";
    }
    return ss;
}

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
/// \param std::list
/// \return std::stringstream
/// \brief print stl list
template<typename T>
std::stringstream printStl(std::list<T>& l)
{
    std::stringstream ss;
    for(auto const& elem : l) {
        ss << "[" << elem << "]";
    }
    return ss;
}

///
/// \tparam T
/// \param std::set
/// \return std::stringstream
/// \brief print stl set
template<typename T>
std::stringstream printStl(std::set<T>& s)
{
    std::stringstream ss;
    for(auto const& elem : s) {
        ss << "[" << elem << "]";
    }
    return ss;
}

#endif //CPP_BOOST_COMMON_H
