# cpp风格的强制类型转换形式如下：

```cpp
cast-name<type>(expression);
```

## static_cast

任何具有`明确定义`的类型转换，只要不包括底层const，都可以使用static_cast。这里`明确定义`就是目标类型有对应的赋值构造函数。

`void*`转为原来类型的指针，使用static_cast

## const_cast

用于去掉或增加const性质。

常常用于函数重载，如下：

```cpp
// 比较两个string长度，返回较短string的引用
const string &shorterString(const string &s1, const string &s2)
{
    return s1.size() <= s2.size() ? s1 : s2;
}

// 上面函数，是两个`const string`作为入参，返回`const string`。如果我们想要一个函数，是两个`string`作为入参，返回`string`。见下：
string &shoreterString(string &s1, string &s2)
{
    auto &r = shorterString(const_cast<const string&>(s1), const_cast<const string>(s2));
    return const_cast<string&>(r);
}
```

## dynamic_cast

运行时类型识别(run-time type identification, RTTI)的转换，使用形式如下：
```cpp
dynamic_cast<type*>(e); // e必须是一个指针，如果转换失败，返回null
dynamic_cast<type&>(e); // e必须是一个左值，如果转换失败，返回bad_cast
dynamic_cast<type&&>(e); // e不能是左值
```

在上面，e必须符合以下其中一种条件：
* e的类型是目标type的公有派生类
* e的类型是目标type的公有基类
* e的类型是目标type本身

要求type

## reinterpret_cast

用于运算对象的位模式提供较低层次上的重新解释。使用reinterpret_cast是非常危险的，因为重新解释后如果不知道对象的实际结构，则在该对象转换后进行各种运算，其行为是未定义的。

[todo]
什么是左值？
