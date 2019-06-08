# 多人聊天编程实验（Chat 实验）

2019.3.23

报告文档：[Chat实验.pdf](Chat实验.pdf)



## 任务一

编写多人聊天程序，要求客户端和服务器都采用多线程方式进行编程。每个客户端都采用TCP协议连接服务器并保持连接。服务器同时与所有客户端建立和保持连接。每个客户端输入的消息都会通过服务器转发给所有客户。



## 任务二

服务器程序转发某个客户端发来的消息时都在消息前面加上该客户端的IP地址和端口号以及服务器的当前时间。要求服务器程序把转发的消息也显示出来。



## 任务三

新客户刚连接时服务器端把“enter”消息（包含客户端IP地址和端口号）发送给所有客户端。



## 任务四

客户端输入exit时退出客户端程序（正常退出），或者客户端直接关闭窗口退出（异常退出），服务器都会把该客户leave的消息广播给所有客户。



## 参考资料

[Getting Started with Winsock](https://docs.microsoft.com/en-us/windows/desktop/winsock/getting-started-with-winsock)

[windows socket函数详解](https://www.cnblogs.com/hgwang/p/6074038.html)

[对于CRITICAL_SECTION用法的介绍和理解](https://blog.csdn.net/shaoyiju/article/details/54882267)