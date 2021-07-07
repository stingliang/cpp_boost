# object_pool对象池

object_pool是用于类实例（对象）的内存池，它能够在析构时调用所有已经分配的内存块调用析构函数，从而正确释放资源，需要包含以下头文件：

```c++
#include <boost/pool/object_pool.hpp>
using namespace boost;
```

其中最关键的是`construct()`和`destroy()`函数，这两个函数是object_pool的真正价值所在。construct()实际上是一组函数，有多个参数的重载形式（目前最多支持3个参数，但可以扩展），它先调用`malloc()`分配内存，再在内存块上使用传入的参数调用类的构造函数，返回的是一个已经初始化的对象指针。`destory()`则先调用对象的析构函数，再用`free()`释放内存块。

## 用法

可以直接使用`construct()`直接在内存池中创建对象

```c++
struct demo_class
{
    int a,b,c;
    string str = "hello boost";

    demo_class(int x = 1, int y = 2, int z = 3):a(x), b(y), c(z)
    {
        cout << "created a demo_class object." << endl;
    }
    ~demo_class()
    {
        cout << "destroyed a demo_class object." << endl;
    }

};

int main()
{
    // 对象内存池
    object_pool<demo_class> op;

    // 传递参数构造一个对象
    auto p = op.construct(4, 5, 6);
    
    cout << p->a << "\n"
         << p->b << "\n"
         << p->c << "\n"
         << p->str << "\n"
         << endl;
    
    // 连续分配大量对象
    for (int i = 0; i < 10 ; i++)
    {
        auto p = op.construct(4, 5, 6);
    }
    cout << "at the end of the main function.\n" << endl;

}
```