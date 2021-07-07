正确性与测试
==============

测试对于软件开发是非常重要的，程序员——尤其是C++程序员更应该认识到这一点。

但C/C++只提供了有限的正确性验证/测试支持——assert宏，这远远不够。

C++标准中的std：：exception能够处理运行时的异常，但并不能检查代码的逻辑，缺乏足够的、语言级别的工具来保证软件的正确性，这使程序员很容易陷入与bug搏斗的泥沼中。

Boost在这方面进行了改进：boost.assert库增强了原始的运行时的assert宏，static_assert库提供了静态断言（编译期诊断），而lightweight_test和test库则构建了完整的单元测试框架。

.. toctree::
    :maxdepth: 1

    assert
    lightweight_test