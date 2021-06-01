## xpressive

### 功能

配合正则表达式使用，一定程度上弥补了C++对字符串处理的短板

### 命名空间

```c++
#include <boost/xpressive/xpressive.hpp>
using namespace xpressive;
```

---

### 基本数据结构

#### basic_regex

`basic_regex`是一个模板类，常用的是两种模板定义
`typedef:sregex`，用于操作标准字符串string
`typedef:cregex`,用于操作字符数组（C风格字符串）
`basic_regex`时xpressive库的核心，封装了对正则表达式的解析和编译，使用静态成员函数`compile()`创建一个`basic_regex`对象。

#### match_results

match_results保存了正则表达式的匹配结果。
match_results为正则表达式的匹配结果提供一个类似容器的“视图”，可以用size（）和empty（）判断匹配结果中子表达式的数量，operator[]返回第i个子表达式。如果i==0，则返回整个表达式的匹配对象。同样，为了支持不同的字符串类型，match_results有两个方便的typedef，分别是smatch和cmatch，用于支持std：：string和字符数组，它们的命名风格与sregex、cregex是相同的，它们使用相同的前缀。

#### sub_match

`sub_match`是一个类似迭代器对的对象，继承自`std::pair`，可以把它当作一个字符串的区间表示。

---

### 正则匹配

自由函数`regex_match()`用来检查一个字符串是否完全匹配一个正则表达式，返回bool结果，其常用的函数声明如下：

```c++
bool regex_match(string, basic_regex const & re);
bool regex_match(string, match_results% what, basic_regex const & re);
```

`regex_match()`最简单的用法接收两个参数：它的第一个参数是要被匹配检查的字符串，第二个参数是正则表达式对象（`sregex`或`cregex`）。
`regex_match()`的第二种形式多了一个`match_results`输出参数，可以返回查找到的字符串。

#### 简单正则匹配

```c++
using namespace boost::xpressive;

// C风格正则表达式对象
cregex reg = cregex::compile("a.c");

assert(regex_match("abc", reg)); // match
assert(regex_match("a+c", reg)); // match
assert(!regex_match("ac", reg)); // mismatch
assert(!regex_match("abd", reg)); // mismatch
```

#### 复杂正则匹配

下面使用正则表达式来匹配身份证号码。

身份证的基本编码是由18位数字组成的，前6位是地区编码，中间8位是年、月、日，最后4位数字中可能有一个是X。我们使用\d来表示数字，那么前6位数字的正则表达式：`\d{6}`，后4位的正则表达式：`\d{3}（X|\d）`。中间的年、月、日的检验要稍微复杂一些，不过为了演示可以进行适当的简化，不检查闰年、月份、天数的有效性，可以写成：`(1|2)\d{3}(0|1)\d[0-3]\d`。其中的`(1|2)\d{3}`检查4位的年份，第一个数字必须是1或2；`(0|1)\d`检查月份；`[0-3]\d`检查天数。

把这些组合起来我们得到：`\d{6}(1|2)\d{3}(0|1)\d[0-3]\d\d{3}(X|\d)`，再将其转换成字符串如下：

```c++
R"---(\d{6}(1|2)\d{3}(0|1)\d[0-3]\d\d{3}(X|\d))---"  // c++11
```

验证身份证的代码如下：

```c++
cregex reg = cregex::compile(
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
```

上述代码中`cregex`的创建略有不同，除正则表达式的字符串较长外，它还使用了一个`icase`标志，用于指示匹配时忽略大小写，此外整个程序与第一个没有什么不同。

#### 使用正则匹配结果

把上述程序稍微修改一下，增加子表达式，使用`match_results`来提取匹配结果中的年、月、日信息。

首先修改正则表达式，将其改为

```c++
R"---(\d{6}((1|2)\d{3})((0|1)\d)([0-3]\d)(\d{3}(X|\d)))---"  // c++11
```

这里我们增加了四对括号，分别定义了四个子表达式，它们是`((1|2)\d{3})`、`((0|1)\d)`、`([0-3]\d)`和`(\d{3}(X|\d))`。注意，子表达式里的括号仍然是一个子表达式，因此这个正则表达式里共有七个子表达式，按照出现的顺序将它们从1至7编号。

将代码修改为：

```c++
cregex reg = cregex::compile(
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
```

运行结果如下：

```
[453001188007061233][1880][1][07][0][06][1233][3]
date:18800706
```

---

### 正则查找

使用`regex_search()`函数进行正则查找，和regex_match()区别如下：

`regex_match()`要求输入的字符串必须要与正则表达式完全匹配，而`regex_search()`则检测输入的表达式中是否包含正则表达式，即存在一个匹配正则表达式的子串。

`regex_search()`声明如下：

```c++
bool regex_search(string, basic_regex const & re);
bool regex_search(string, match_results& what, basic_regex const & re);
```

`regex_search()`的调用形式与regex_match()完全相同，但有一点除外，即它不要求完全匹配，只要找到匹配的子串就返回`true`。

下面的代码示范了`regex_search()`用法，其中使用了正则表达式`(power)-(.{4})`来搜索模式`power+连字符+4个任意字符`：

示例代码：

```c++
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
```

运行结果：

```
POWER-suit POWER suit
```

---

### 正则替换

xpressive的替换功能使用的函数是`regex_replace()`，它先以正则表达式查找匹配的字符串，再用指定的格式将其替换，基本形式如下：

```c++
Stringregex_replace(String,basic_regexconst&re,Format);
```

`regex_replace()`的前两个参数与`regex_match()`、`regex_search()`相同，第三个参数`Format`可以是一个简单的字符串，也可以是一个符合ECMA-262定义的带格式的字符串，可以用`$N`引用正则表达式匹配的子表达式，用`$&`引用全匹配。在替换完成后，`regex_replace()`返回字符串的拷贝。

示例代码：

```c++
string str("readme.txt");

sregex reg1 = sregex::compile("(.*)(me)");
sregex reg2 = sregex::compile("(t)(.)(t)");

cout << regex_replace(str, reg1, "manual") << endl;
cout << regex_replace(str, reg1, "$1you") << endl;
cout << regex_replace(str, reg1, "$&$&") << endl;
cout << regex_replace(str, reg2, "$1N$3") << endl;

str = regex_replace(str, reg2, "$1$3");
cout << str << endl;
```

运行结果：

```
manual.txt
readyou.txt
readmereadme.txt
readme.tNt
readme.tt
```

---

### 正则迭代

xpressive库提供一个强大的迭代器模板`regex_iterator`，类似于`string_algo`的查找迭代器，可以用来遍历正则表达式的匹配结果。

`regex_iterator`的构造函数完成迭代初始化工作，要求传入进行分析的容器区间和正则表达式对象，之后就可以对它反复调用`operator++`，使用`*`或`-＞`获取匹配结果的`match_results`对象。

和`basic_regex`一样需要分`sregex_iterator`/cregex_iterator

示例用法：

```c++
string str("Power-bomb, power-suit, pOWER-beam all times\n");
cout << "str = " << str;
sregex reg = sregex::compile(R"(power-(\w{4}))", icase);
sregex_iterator pos(str.begin(), str.end(), reg);
sregex_iterator end;
for(;pos != end; ++pos)
{
    cout << "[" << (*pos)[0] << "]";
}
cout << endl;
```

运行结果：

```
str = Power-bomb, power-suit, pOWER-beam all times
[Power-bomb][power-suit][pOWER-beam]
```

---

### 正则分词

> 和`basic_regex`一样需要分
> `sregex_token_iterator`/`cregex_token_iterator`

`regex_token_iterator`的用法与regex_iterator大致相同，但它的变化在于匹配对象的使用方式。

在默认情况下，`regex_token_iterator`与regex_iterator相同，返回匹配的字符串。如果构造时传入-1作为最后一个args参数的值，它将把匹配的字符串视为分隔符。如果args是一个正数，则返回匹配结果的第args个字串。

`regex_token_iterator`还有一点与regex_iterator不同：它解引用返回的是一个`sub_match`对象，而不是`match_results`对象。

示例用法：

```c++
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
```

运行结果：

```c++
[Link][Mario][Zelda][Metroid]
[Link*][+Mario+][Zelda!!!][Metroid]
```

---

### 高级议题

#### 工厂类

除了可以使用`sregex::compile()`创建正则表达式对象，xpressive还提供了regex_compiler类作为工厂类。

> 和`basic_regex`一样需要分
> `sregex_compiler`/`cregex_compiler`

`regex_compiler`可以创建更多的正则表达式对象，重载了[]，可以将其当成一个正则表达式对象池。

示例代码：

```c++
using namespace boost::xpressive;

cregex_compiler rc;  // 一个正则表达式工厂

rc["reg1"] = rc.compile("a|b|c");
rc["reg2"] = rc.compile(R"(\d*)");

assert(!regex_match("abc", rc["reg1"]));
assert(regex_match("123", rc["reg2"]));
```

#### 异常

当`expressive`编译不正确的正则表达式或执行其他操作时会抛出regex_error异常。

使用`xpressive`的时候应当总使用`try-catch`块，以防止异常。

`regex_error`异常派生自`boost::expection`，可以任意追加错误信息。

#### 格式化器

在使用`regex_replace()`进行替换时，除了使用简单字符串和格式字符串，我们还可以使用格式化器。格式化器是一个具有`operator()`的可调用对象，函数指针、函数对象或lambda表达式都可以，但它必须能够处理查找到的`match_results`对象。下面的代码定义了一个函数对象`formater`，它将查找到的cmatch对象全部改为大写，使用了string_algo库的to_upper_copy算法：

```c++
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
```

运行结果：

```
LINK*||+MARIO+||ZELDA!!!||METROID
```