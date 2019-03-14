# Echo 实验

2019.3.16

## 任务一：编写 TCP Echo 程序

服务器把客户端发送来的任何消息都返回给客户端，返回的消息前面要加上服务器的当前时间。客户端把返回的消息显示出来。客户端每输入一条消息就建立 TCP 连接，并把消息发送给服务器，在收到服务器回应后关闭连接。



## 任务二：编写 TCP Echo 增强程序

在任务一的基础上，服务器在收到客户端的消息时显示服务器的当前时间、客户端的IP地址、客户端的端口号和客户端发来的信息，并把它们一并返回给客户端。
客户端在发送消息后把服务器发回给它的消息显示出来。

提示：客户端程序与任务一同，不用修改

要求服务器直接从`accept()`的参数中得到客户端的IP地址和端口号。注：服务器获取IP地址后要求直接使用`s_un_b`的四个分量得到IP地址，不能使用函数`inet_ntoa()`转换IP地址。



## 任务三：编写 UDP Echo 增强程序

修改UDP例程，完成Echo功能，即当客户端发来消息时，服务器显示出服务器的当前时间、客户端的IP、客户端的端口号和客户发来的信息，并把它们一并发回给客户端，客户端然后把它们显示出来。
服务器可以直接从`recvfrom()`的参数`from`中得到客户端的IP地址和端口号，并且服务器用`sendto()`发回给客户端消息时可以直接用该参数`from`作为参数`toAddr`。可以使用`inet_ntoa()`转换客户端IP地址。
客户端程序的`recvfrom()`可以直接使用原来`sendto`使用的sock。该sock已经绑定了客户端的IP地址和端口号，客户端可以直接用来接收数据。



## 目录结构

```
Lab2_TcpUdpEcho
│   README.md
│
├───TCP
│   ├───可执行文件
│   │       TcpEcho_Client.exe
│   │       TcpEcho_Server.exe
│   │       TcpEcho_Server_Enhanced.exe
│   │
│   └───源码
│           TcpEcho_Client.c
│           TcpEcho_Server.c
│           TcpEcho_Server_Enhanced.cpp
│
└───UDP
     ├───可执行文件
     │       UdpEcho_Client.exe
     │       UdpEcho_Server.exe
     │
     └───源码
             UdpEcho_Client.c
             UdpEcho_Server.cpp
```



## 实验总结

### TCP 编程步骤

#### TCP 服务器端编程的步骤

1. 加载套接字库，创建套接字`WSAStartup()`/`socket()`；
2. 绑定套接字到一个IP地址和一个端口上`bind()`；
3. 将套接字设置为监听模步等待连接请求`listen()`；
4. 请求到来后，接受连接请求，返回一个新的对应于此次连接的套接字`accept()`；
5. 用返回的套接字和客户端进行通信`send()`/`recv()`；
6. 返回，等待另一连接请求；
7. 关闭套接字，关闭加载的套接字库`closesocket()`/`WSACleanup()`。



#### TCP 客户端编程的步骤

1. 加载套接字库，创建套接字`WSAStartup()`/`socket()`；
2. 向服务器发出连接请求`connect()`；
3. 和服务器端进行通信`send()`/`recv()`；
4. 关闭套接字，关闭加载的套接字库`closesocket()`/`WSACleanup()`。



### UDP 编程步骤

* 与 TCP 类似，不过 UDP 不需要建立连接（`accept()`/`connect()`）。

* 发送函数为`sendto()`，接收函数为`recvfrom()`。



## 参考资料

[windows socket函数详解](https://www.cnblogs.com/hgwang/p/6074038.html)

[Getting Started with Winsock](https://docs.microsoft.com/en-us/windows/desktop/winsock/getting-started-with-winsock)