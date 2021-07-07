容器和数据结构
==============

容器是C++标准库中最引人注意的部分，C++98提供了vector、deque、list、set和map，实现了最常用的动态数组、双端队列、双向链表、集合和映射5种数据结构，而C++11又新增了array、forward_list和unordered容器。

它们均以泛型的形式提供，可以容纳任意类型的元素，极大地方便了C++程序员的工作。

Boost程序库基于与C++标准相同的设计理念，实现了数个新型容器（数据结构），如散列容器、循环队列、多维数组等，扩展了“容器”概念的内涵和外延，其中的array、tuple等已经被收入C++标准。

本文共介绍10个容器（数据结构），首先是array、dynamic_bitset、unordered、bimap和circular_buffer，它们都是对原有标准容器在概念上的扩展，与标准容器的接口非常相似，比较容易学习。

随后介绍tuple、any、variant，它们3个既是数据结构也是特殊的容器，展示了泛型编程的精妙用法。

最后介绍两个新型容器——multi_array和property_tree。

这些容器都对容纳的元素类型有一个基本要求：析构函数不能抛出异常，有的容器还要求元素具有默认构造函数或拷贝构造函数。

.. toctree::
    :maxdepth: 1

    array_and_dynamci_bitset
    unordered