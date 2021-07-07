字符串与文本处理
================

字符串与文本处理曾经一直是C++的弱项，C++98标准仅提供了标准字符串类std：：string，但它缺乏很多文本处理的高级特性，使得不少C++程序员不得不转向其他语言。

C++11标准引入了＜regex＞＜codecvt＞，支持原始字符串，但依然不够完善。

Boost填补了C++这方面的空白，为整个C++社区做出了重要的贡献。

本章将讨论五个Boost中字符串处理领域的组件。

首先是两个与C标准库函数功能类似的lexical\_cast和format，它们关注于字符串的表示，可以将数值转化为字符串，对输出做精确的格式化。

接下来，string\_ref提供了一个“只读视图”，可以避免字符串的拷贝代价，即conststd：：string&。

后两个库关注字符串的处理：string\_algo库提供了大量常用的字符串处理函数，可以满足90%以上的应用需求。

剩下的10%的需求可以使用xpressive，xpressive是一个灵活且功能强大的正则表达式分析器，同时也是一个语法分析器。

.. toctree::
    :maxdepth: 1

    lexcial_cast
    format
    string_ref
    string_algo

.. toctree::
    :maxdepth: 2

    xpressive