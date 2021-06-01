## lexical_cast

### 功能

对字符串进行“字面值”的转换，对字符串与整数/浮点数之间进行转换

### 需要包含的头文件

```c++
#inlude <boost/lexical_cast.hpp>
using namespace boost;
```

### 声明

```c++
//标准形式，转换数字和字符串
template <typename Target,typename Source>
inline Target lexical_cast(const Source &arg);

//转换C字符串
template <typename Target>
inline Target lexical_cast(const char * chars,std::size_t count)
```

### 使用

在模板参数里指定转换的目标类型即可

```c++
//string -> int
int x = lexical_cast<int> ("100");

// string -> float
float pai = lexical_cast<float> ("3.14159e5");

//int -> string
string str = lexical_cast<string> (456);

//float -> string
string str = lexical_cast<string> (0.618);

//hex -> string
string str = lexical_cast<string> (0x10);
```

**【注意事项】**
该模板智能转换字面值，如果出现不合理的转换，例如“hello”转int类型，则会报错（正常人应该不会这么干）

### 错误处理

当lexical_cast无法执行转换操作时会抛出异常bad_lexical_cast，它是std::bad_cast的派生类

#### 传统保护办法

在使用lexical_cast时应该使用try_catch来保护代码

```c++
try
{
    cout <<lexical_cast<int>("0x100");
}
catch(bad_lexical_cast& e)
{
    cout << "error: \n" << e.what() << endl;
}

//运行结果
error: 
bad lexical cast:source type value could not be interpreted as target
```

#### 已有库的保护办法

需要使用命名空间：boost::conversion

函数：

`boost::conversion::try_lexical_cast(typeRaw,typeTarget);`

返回值为bool表示是否转换成功

#### 【技巧：验证数字字符串的合法性（用于验证用户输入的有效性）】

实现一个模板类

```c++
template<typename T>
bool num_valid(const char* str)
{
    T tmp;
    return conversion::try_lexical_convert(str,tmp)  //尝试转换数字
}

//用法
assert(num_valid<double>("3.14"));
assert(!num_valid<int>("3.14"));
assert(num_valid<int>("65535"));
```

### 转换要求

lexical_cast对转换对象有一定要求

- 转换的起点对象是可流输出的（可以用“<<”）

  <u>【注意事项】对于重载了“<<”操作符的自定义类型也可以使用它</u>

- 转换的终点对象是可流输入的（可以用“>>”）

- 转换的终点对象是可默认构造的、可拷贝构造的

> 最常用的搭档：int,double,string等POD类型

### C++标准转换函数

```c++
//字符串转换为数字
int stoi(const string& str,size_t *idx = 0,int base = 10);
long stol(const string& str,size_t *idx = 0,int base = 10);
long long stoll(const string& str,size_t *idx = 0,int base = 10);
float stof(const string& str,size_t *idx = 0);
double stod(const string& str,size_t *idx = 0);

//数字转换为string
string to_string(Type val);
```

【注意事项】必须以空格或数字开头，否则报错

和lexical_cast的比较：

优点：

- 无需写模板参数
- 允许出现非数字字符（忽略起始空格，遇到无法转换的字符终止）

缺点：

- 不支持对自定义类型的转换