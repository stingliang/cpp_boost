/*
 * @Name: liangrui
 * @Date: 2021-1-13 9:39:10
 * @LastEditTime: 2021-01-18 10:33:37
 */

#define COUT(str) (cout << "-------------\n" \
                        << str \
                        << "\n-------------" \
                        << endl)
 
 // c++ stdlib
 #include <iostream>
 #include <vector>
 #include <map>
 #include <set>
 #include <string>
 #include <functional>
 #include <cmath>
 #include <list>
 using std::cin;
 using std::cout;
 using std::endl;
 using std::string;
 using std::vector;
 using std::map;
 using std::set;
 using std::list;

// --------------------regex_match--------------------
#include <boost/xpressive/xpressive.hpp>
void regexMatch()
{
    using namespace boost::xpressive;

    // C风格正则表达式对象
    cregex reg = cregex::compile("a.c");

    assert(regex_match("abc", reg)); // match
    assert(regex_match("a+c", reg)); // match
    assert(!regex_match("ac", reg)); // mismatch
    assert(!regex_match("abd", reg)); // mismatch

    reg = cregex::compile(
        R"---(\d{6}(1|2)\d{3}(0|1)\d[0-3]\d\d{3}(X|\d))---",
        icase
    );  //使用C++11的字符串形式
    assert(regex_match("999555197001019999", reg));
    assert(regex_match("99955519700101999X", reg));
    assert(regex_match("99955520100101999x", reg));

    assert(!regex_match("99955520100101999z", reg));
    assert(!regex_match("99955530100101999X", reg));
    assert(!regex_match("999555201099019998", reg));
    assert(!regex_match("999555201012419991", reg));

    reg = cregex::compile(
        R"---(\d{6}((1|2)\d{3})((0|1)\d)([0-3]\d)(\d{3}(X|\d)))---",
        icase
    );  //使用C++11的字符串形式
    cmatch what;
    assert(regex_match("453001188007061233", what, reg));
    for(auto& x : what)
    {
        cout << "[" << x << "]";
    }
    cout << endl;

    cout << "date:" << what[1] << what[3] << what[5] << endl;
}

// --------------------regex_match--------------------
#include <boost/xpressive/xpressive.hpp>
void regexSearch()
{
    using namespace boost::xpressive;

    char str[] = "There is a POWER-suit item";
    cregex reg = cregex::compile("(power)-(.{4})", icase);

    assert(regex_search(str, reg));  // 可搜索到字符串

    cmatch what;
    regex_search(str, what, reg);  // 保存搜索结果
    assert(what.size() == 3);  // 共3个子表达式

    cout << what[0] << " " 
         << what[1] << " " 
         << what[2] << endl;
    assert(!regex_search("error message", reg));
}

// --------------------regex_replace--------------------
#include <boost/xpressive/xpressive.hpp>
void regexReplace()
{
    using namespace boost::xpressive;

    string str("readme.txt");

    sregex reg1 = sregex::compile("(.*)(me)");
    sregex reg2 = sregex::compile("(t)(.)(t)");

    cout << regex_replace(str, reg1, "manual") << endl;
    cout << regex_replace(str, reg1, "$1you") << endl;
    cout << regex_replace(str, reg1, "$&$&") << endl;
    cout << regex_replace(str, reg2, "$1N$3") << endl;

    str = regex_replace(str, reg2, "$1$3");
    cout << str << endl;
}

// --------------------regex_iterate--------------------
#include <boost/xpressive/xpressive.hpp>
void regexIterate()
{
    using namespace boost::xpressive;

    string str("Power-bomb, power-suit, pOWER-beam all times\n");

    cout << "str = " << str << endl;

    sregex reg = sregex::compile(R"(power-(\w{4}))", icase);
    sregex_iterator pos(str.begin(), str.end(), reg);
    sregex_iterator end;
    for(;pos != end; ++pos)
    {
        cout << "[" << (*pos)[0] << "]";
    }
    cout << endl;
}

// --------------------regex_token_iterate--------------------
#include <boost/xpressive/xpressive.hpp>
void regexSplit()
{
    using namespace boost::xpressive;

    char str[] = "Link*||+Mario+||Zelda!!!||Metroid";

    // 查找所有的单词，忽略标点
    cregex reg = cregex::compile(R"(\w+)", icase);

    cregex_token_iterator pos(str, str + strlen(str), reg);
    for(;pos != cregex_token_iterator();++pos)
    {
        cout << "[" << *pos << "]";
    }
    cout << endl;

    // 使用分隔符，分隔符是“||”
    cregex split_reg = cregex::compile(R"(\|\|)");
    pos = cregex_token_iterator(str,
                                str + strlen(str),
                                split_reg,
                                -1);
    for(;pos != cregex_token_iterator();++pos)
    {
        cout << "[" << *pos << "]";
    }
    cout << endl;
}

// --------------------regex_compiler--------------------
#include <boost/xpressive/xpressive.hpp>
#include <boost/algorithm/string.hpp>
void regexPlus()
{
    using namespace boost::xpressive;

    // regex_copmiler
    cregex_compiler rc;  // 一个正则表达式工厂
    rc["reg1"] = rc.compile("a|b|c");
    rc["reg2"] = rc.compile(R"(\d*)");
    assert(!regex_match("abc", rc["reg1"]));
    assert(regex_match("123", rc["reg2"]));

    // 格式化器
    struct  formater
    {
        string operator()(cmatch const& m) const
        {
            return boost::to_upper_copy(m[0].str());
        }
    };
    char str[] = "Link*||+Mario+||Zelda!!!||Metroid";
    cregex reg = cregex::compile(R"(\w+)", icase);
    cout << regex_replace(str, reg, formater()) << endl;
}

int main()
{
    COUT("regex_match");
    regexMatch();

    COUT("regex_search");
    regexSearch();

    COUT("regex_replace");
    regexReplace();

    COUT("regex_iterate");
    regexIterate();

    COUT("regex_split");
    regexSplit();

    COUT("regex_plus");
    regexPlus();
}