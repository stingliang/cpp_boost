#### 获取方式

官网下载合适版本：https://www.boost.org/

此处用的是`boost_1_75_0`版本

#### 开发环境

推荐使用`GCC 7.x.x`或以上编译器

#### 安装Boost库

此处采用简易安装，默认安装在`/usr/local/include`和`/usr/local/lib`下

```shell
# 解压
tar -zxvf boost_1_75_0.tar.gz
# 配置
./bootstrap.sh
# 安装
./b2 install
```

#### 安装验证

编写一个boost程序，验证安装是否成功

```c++
#include <iostream>
#include <boost/version.hpp>
#include <boost/config.hpp>

using std::cout;
using std::endl;

int main(int argc, char * argv[])
{
        cout << BOOST_VERSION << endl;
        cout << BOOST_LIB_VERSION << endl;
        cout << BOOST_PLATFORM << endl;
        cout << BOOST_COMPILER << endl;
        cout << BOOST_STDLIB << endl;
}
```

编译

```shell
g++ -o main main.cpp
```

运行结果

```shell
$ ./main 
107500
1_75
linux
GNU C++ version 7.3.1 20180303 (Red Hat 7.3.1-5)
GNU libstdc++ version 20180303
```

