## string_algo

### 功能

提供了强大的字符串处理能力，如查找、访问、基本的字符串处理

### 头文件和命名空间

```c++
#include <boost/algorithm/string.hpp>

using namespace boost;
```

### 用法

【注意事项】不仅可以用在`string`上（在这里`string`被看作是`vector<char>`），也可以用于部分其他容器,例如（`vector<T>`）

#### 大小写转换

```c++
string str("Hello");

//转向大写
cout << to_upper(str) << endl;    //这种方式会改变源数据
cout << to_upper_copy(str) << endl;    //这种方法返回一个转换后的拷贝对象

//转向小写
cout << to_lower(str) <<endl;
cout << to_lower_copy(str) << endl;
```

#### 判断式（算法）

- `lexicographical_compare`：根据字典顺序检测一个字符串是否小于另一个字符串
- `starts_with`：检测字符串是否以另一个字符串为前缀
- `ends_with`：检测字符串是否以另一个字符串为后缀
- `contains`：检测字符串是否包含另一个字符串
- `equals`：检测两个字符串是否相等
- `all`：检测字符串是否满足指定的判断式

【注意事项】

- 除了all以外都有一个i前缀的版本，表示大小写无关
- 这些函数都不变动字符串	

用法示例

```c++
string str("Power Bomb");

assert(iends_with(str,"bomb"));    //大小写无关检测后缀
assert(!ends_with(str,"bomb"));    //大小写敏感检测后缀

assert(starts_with(str,"Pow"));    //检测前缀

assert(contains(str,"er"));    //测试包含关系

string str2 = to_lower_copy(str);    //转换成小写
assert(iequals(str,str2));    //大小写无关判断相等

assert(ilexicographical_compare(str,str3));    //大小写无关字符串比较

assert(all(str2.substr(0,5),is_lower()));    //检测字符串均小写
```

#### 分类

提供一组分类函数，用于检测字符串是否符合某种特性，主要搭配其他算法使用，如上一节的all

- `is_space`：字符是否为空格或制表符（tab）
- `is_alnum`：字符是否为字母和数字字符
- `is_alpha`：字符是否为字母
- `is_cntrl`：字符是否为控制字符
- `is_digit`：字符是否为十进制数字
- `is_graph`：字符是否为图形字符
- `is_lower`：字符是否为小写字符
- `is_print`：字符是否为可打印字符
- `is_punct`：字符是否为标点符号字符
- `is_upper`：字符是否为大写字符
- `is_xdigit`：字符是否为十六进制数字
- `is_any_of`：字符是否是参数字符序列中的任意字符
- `if_from_range`：字符是否位于指定区间内，即from＜=ch＜=to

需要注意的是这些函数并不真正地检测字符，而是返回一个类型为detail：：is_classifiedF的函数对象，这个函数对象的operator（）才是真正的分类函数（因此，这些函数都属于工厂函数）。

#### 修剪

提供三个算法，删去字符串开头结尾的空格，提供_copy后缀和_if后缀

- `trim_left`：删除左边的空格
- `trim_right`：删除右边的空格
- `trim`：删除两边的空格

用法示例

```c++
format fmt("|%s|\n");

string str = "  samus aran  ";
cout << fmt % trim_copy(str);    //删除两端的空格
cout << fmt % trim_left_copy(str);    //删除左边的空格

trim_right(str);    //原地删除
cout << fmt % str;

string str2 = "2020 Happy new Year!!!";
cout << fmt % trim_left_copy_if(str2,is_digit());    //删除左端的数字
cout << fmt % trim_right_copy_if(str2,is_punct());    //删除右边的标点
//删除两端的标点、数字和空格
cout << fmt % trim_copy_if(str2,is_punct() || is_digit() || is_space()); 
```

执行结果

```
|samus aran|
|samus aran  |
|  samus aran|
| Happy new Year!!!|
|2020 Happy new Year|
|Happy new Year|
```

#### 查找

提供的查找算法如下：

- `find_first`：查找字符串在输入中第一次出现的位置
- `find_last`：查找字符串在输入中最后一次出现的位置
- `find_nth`：查找字符串在输入中的第N次（从0开始计数）出现的位置
- `find_head`：取一个字符串开头N个字符的子串，相当于substr（0，n）
- `find_tail`：取一个字符串末尾N个字符的子串

【注意事项】

这些算法的返回值是`iterator_range`，在概念上类似于`std::pair`，包装了两个迭代器，可以用`begin()`和`end()`访问。提供了i前缀的用法。
用法示例

```c++
format fmt("|%s|.pos = %d\n");
string str = "Long long ago , there was a king.";
iterator_range<string::iterator> rge;    //迭代器区间

rge = find_first(str,"long");    //找第一次出现
cout << fmt % rge % (rge.begin() - str.begin());

rge = ifind_first(str,"long");    //大小写无关第一次出现
cout << fmt % rge % (rge.begin() - str.begin());

rge = find_nth(str,"ng",2);    //找第三次出现
cout << fmt % rge % (rge.begin() - str.begin());

rge = find_head(str,4);    //取前4个字符
cout << fmt % rge % (rge.begin() - str.begin());

rge = find_tail(str,5);    //取末5个字符
cout << fmt % rge % (rge.begin() - str.begin());

rge = find_first(str,"samus");    //找不到
assert(rge.empty() && !rge);
```

执行结果

```
|long|.pos = 5
|Long|.pos = 0
|ng|.pos = 30
|Long|.pos = 0
|king.|.pos = 28
```

#### 替换与删除

替换、删除操作与查找算法非常接近，是在查找到结果后再对字符串进行处理，具体如下：

`replace`/`erase_first`：替换/删除字符串在输入中的第一次出现

`replace`/`erase_last`：替换/删除字符串在输入中的最后一次出现

`replace`/`erase_nth`：替换/删除字符串在输入中的第n次出现

`replace`/`erase_all`：替换/删除字符串在输入中的所有出现

`replace`/`erase_head`：替换/删除输入的开头

`replace`/`erase_tail`：替换/删除输入的末尾

前8个算法每个都有前缀i、后缀_copy的组合，有四个版本，后4个则只有后缀_copy的两个版本

示例代码如下：

```c++
// 替换和删除
string str_2 = "Samus beat the monster.\n";
// replace
cout << replace_first_copy(str_2, "Samus", "samus");
replace_last(str_2, "beat", "kill");
cout << str_2;
replace_tail(str_2, 9, "ridley.\n");
cout << str_2;
// delete
cout << ierase_all_copy(str_2, "samus ");
cout << replace_nth_copy(str_2, "l", 1, "L");
cout << erase_tail_copy(str_2, 8);
```

运行结果：

```c++
samus beat the monster.
Samus kill the monster.
Samus kill the ridley.
kill the ridley.
Samus kilL the ridley.
Samus kill the
```

#### 分割

string_algo提供了两个字符串分割算法：`find_all`（虽然它的名称含有find，但因为其功能而被归类为分割算法）和split，可以使用某种策略把字符串分割成若干部分，并将分割后的字符串拷贝存入指定的容器。

分割算法对容器类型的要求是必须能够持有查找到结果的拷贝或引用，因此容器的元素类型必须是`string`或`iterator_range＜string::iterator＞`，容器则可以是vector、`list`、deque等标准容器。

`find_all`算法类似于普通的查找算法，它搜索所有匹配的字符串，将其加入容器，有一个忽略大小写的前缀i版本。

`split`算法使用判断式Pred来确定分割的依据，如果字符ch满足判断式`Pred(Pred(ch)==true)`，那么它就是一个分割符，将字符串从这里分割。

还有第三个参数`eCompress`可以取值为`token_compress_on`或`token_compress_off`，如果值为前者，那么当两个分隔符连续出现时，它们将被视为一个分隔符，如果值为后者则两个连续的分隔符标记了一个空字符串。参数`eCompress`的默认取值为`token_compress_off`。

```c++
string str = "Samus, Link.Zelda::Mario-Luigi+zelda";

deque<string> d;
// 大小写无关查找
ifind_all(d, str, "zELDA");
assert(d.size() == 2);
for(auto x: d)
{
    cout << "[" << x << "]";
}
cout << endl;

// 存储range对象
list<iterator_range<string::iterator>> l;
split(l, str, is_any_of(",.:-+"));  // 使用标点分割
for(auto x: l)
{
    cout << "[" << x << "]";
}
cout << endl;

l.clear();
split(l, str, is_any_of(",.:-+"), token_compress_on);
for(auto x: l)
{
    cout << "[" << x << "]";
}
cout << endl;
```

程序运行结果如下：

```
[Zelda][zelda]
[Samus][ Link][Zelda][][Mario][Luigi][zelda]
[Samus][ Link][Zelda][Mario][Luigi][zelda]
```

#### 合并

合并算法join是分割算法的逆运算，它把存储在容器中的字符串连接成一个新的字符串，并且可以指定连接的分隔符。

join还有一个后缀_if的版本，它接收一个判断式，只有满足判断式的字符串才能合并。

```c++
vector<string> v = list_of("Samus")("Link")("Zelda")("Mario");
cout << join(v, "+") << endl;
cout << join_if(
    v, "**",
    [](string_ref s)
    {
        return contains(s, "a");
    }
) << endl;
```

程序首先使用assign库向`vector`添加了4个字符串，然后用`+`合并它们。随后的`join_if`算法使用lambda表达式定义了一个简单的谓词，它包装了算法`contains`，判断字符串是否包含字符a。
程序运行结果如下：

```
Samus+Link+Zelda+Mario
Samus**Zelda**Mario
```


