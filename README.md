# 多线程Logger

## 设计思路

目标：
- 线程安全
- 高性能
- 方便的调用
- 方便的格式化

采用了producer-consumer的模式，在Logger类中开启一个线程从缓冲区读取log向文件中写，其他应用程序线程调用写入接口时只向缓冲区写入log。

logger类和logentry分离，又叫做前后端分离，前者负责将log（string）写入文件中，后者负责格式化制造log和接收用户调用。

## condition_variable的作用

不使用cv的时候，logger直到析构之前其writer线程都会占满CPU（100%），cv的作用是在用户设定条件得不到满足时阻塞一个线程，在本实例中不使用cv阻塞虽然不会产生安全问题（消耗空缓冲区），但仍然会产生额外性能损耗，加入cv后解决了这个问题。

## 单例模式的实现



## 格式化


### std::format的使用

1. 需要c++20以上，g++在这里使用了版本13
2. format pattern必须是编译时确定的，因此必须是字面量或constexpr函数返回的std::string，后者麻烦且可读性差。



## 宏helpers


