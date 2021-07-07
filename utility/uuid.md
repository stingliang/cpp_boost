## uuid

### 功能

生成UUID（Universally Unique Identifier），它是一个128位的数字（16字节），它无需中央认证就可以生成全球唯一的标识符，形如

```shell
E4A0D7CE-9E6D-4E74-9E6D-7E749E6D7E74
```

### 类摘要

```c++
class uuid
{
    public:
        static size_type static_size();    //长度，返回16
        size_type size() const;
        uint8_t data[static_size()];    //内部实现
        

        iterator begin();
        iterator end();
        bool is_nil() const;

}
```

### 数据属性

- 没有构造函数，可以像使用POD类型数据一样使用
- 支持流输入输出
- 支持字典序的比较
- 其存储数据的是一个普通数组，但是为其提供了begin()和end()迭代器

### 用法

#### 需要包含的头文件

```c++
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

//命名空间
using namespace boost::uuids;
```

#### 常用方法

```c++
boost::uuids::uuid u;

//随机输出一个uuid值，但是这种方式生成的uuid很容易重复
cout << u << endl;

assert(boost::uuids::uuid::static_size() == 16);  // 长度总为16
assert(u.size() == 16);

vector<unsigned char> v(16,7);	//一个vector对象
copy(v.begin(),v.end(),u.begin());	//标准复制方法
assert(u.data[0] == u.data[1] && u.data[15] == 7);	//使用数组方式访问

//流输出：07070707-0707-0707-0707-070707070707
cout << u << endl;
fill_n(u.data + 10,6,8);	//使用标准fill_n直接操纵数组

//流输出：07070707-0707-0707-0707-080808080808
cout << u << endl;
```

#### 使用生成器生成uuid

> 生成器本身是一个类

1）Nil生成器

生成一个全0的无效uuid

```c++
//第一个()构造生成器对象，第二个()调用对象的函数
uuid u = nil_generator()();
assert(u.is_nil);
//nil_uuid()是一个内联函数，相当于直接调用Nil生成器
u = nil_uuid();
assert(u.is_nil);
```

2）字符串生成器

从一个字符串创建uuid对象（可以是c_str、string、wstring），但是对字符串格式有严格要求

- 没有连字符的全hex数字
- 在5、7、9、11字节使用连字符的全hex数字

```c++
//声明字符串生成器对象
string_generator sgen;

//使用字符串构造
uuid u1 = sgen("0123456789abcdef0123456789abcdef");
cout << u1 << endl;
uuid u2 = sgen("01234567-89ab-cdef-0123-456789abcdef");
cout << u2 << endl;
```

3）名字生成器

name_generator使用基于名字的SHA1摘要算法，使用一个基准的UUID然后使用字符串名字派生出这个UUID的一系列UUID，<u>只要基准UUID不变，那么相同的名字总会创建相同的UUID</u>

```c++
uuid www_xxx_com = string_generator()("0123456789abcdef0123456789abcdef");

//使用uuid构造
name_generator ngen(www_xxx_com);

uuid u1 = ngen("mario");
//为名字生成UUID，version是基于SHA1算法
assert(!u1.is_nil() && u1.version() == uuid::version_name_based_sha1);

uuid u2 = ngen("link");
cout << "mario's id :" << u1 << endl;
cout << "link's id :" << u2 << endl;
```

4）随机生成器

保证生成随机的UUID不会重复

```c++
random_generator rgen;

uuid u = rgen();
assert(u.version() == uuid::version_random_number_based);
cout << u << endl;
```

### 转换字符串

窄字符串转换

`string to_string(uuid const&);`

宽字符串转换

`wstring to_wstring(uuid const&);`