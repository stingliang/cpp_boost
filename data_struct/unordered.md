## unordered

### 功能

散列容器(hash container)是一种重要的容器，它通常比二叉树的存储方式可以提高更高的访问效率。

散列容器是无序的，散列容器包括<u>无序集合</u>和<u>无序映射</u>。

### 头文件和命名空间

```c++
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
using namespace boost;
```

### 用法

#### 散列集合(unordered_set)

unordered_set提供了两个散列集合类`unordered_set`和`unordered_multiset`，他们的接口、用法和C++标准容器的`set`/`multiset`相同，只是其内部用hash表代替了二叉树的实现，其查找复杂度由对数降为常数。

```c++
unordered_set<int> s = {1, 2, 3, 4, 5};

for(auto x : s)
{
    cout << x << ",";
}
cout << endl;
cout << s.size() << endl;   // 获取容器大小

s.clear();  // 清空集合
cout << s.empty() << endl;  // 集合判空

s.insert(8);
s.insert(45);
cout << s.size() << endl;
cout << *s.find(8) << endl; // 查找元素

s.erase(45);    // 删除元素
```

#### 散列映射(unordered_map)

unordered_map也提供了两个散列映射类`unordered_map`和`unordered_multimap`，他们的接口、用法和C++标准容器的`map`/`multimap`相同，只是其内部用hash表代替了二叉树的实现。

```c++
// 散列映射用法，使用assign初始化
unordered_map<int, string> um = map_list_of(1, "one")(2, "two")(3, "three");

um.insert(std::make_pair(10, "ten"));
cout << um[10] << endl; // 使用[]访问元素
um[11] = "eleven";  // 使用关联数组，注意这里的[x]和普通数组[x]的区别
um[15] = "fifteen";

auto p = um.begin();
for (;p != um.end(); ++p)
{ cout << p->first << "-" << p->second << ","; }

um.erase(11);
cout << um.size() << endl;
```

