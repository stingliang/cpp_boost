# signals2

signals2（衍生自 Boost中另一个已被废弃的库signals）实现了线程安全的观察者模式。 在 signals2 库中，观察者模式被称为信号/插槽（ signals/ slots）机制，它是一种函数回调机制，一个信号关联了多个插槽，当信号发出时，所有关联它的插槽都会被调用。许多成熟的 软件系统都用到了这种信号/插槽机制（另一个 常用的名称是事件处理机制：event/ event handler），它可 以很好地解耦一组互相协作的类，有的语言甚至直接内建了对它的支持，signals2 以 库的形式 为 C++ 增加了 这个重要的功能。

## 头文件和命名空间
