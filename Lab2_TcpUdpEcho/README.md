# Echo实验











## 实验总结

### 服务器端编程的步骤
1. 加载套接字库，创建套接字`WSAStartup()`/`socket()`；
2. 绑定套接字到一个IP地址和一个端口上`bind()`；
3. 将套接字设置为监听模步等待连接请求`listen()`；
4. 请求到来后，接受连接请求，返回一个新的对应于此次连接的套接字`accept()`；
5. 用返回的套接字和客户端进行通信`send()`/`recv()`；
6. 返回，等待另一连接请求；
7. 关闭套接字，关闭加载的套接字库`closesocket()`/`WSACleanup()`。



### 客户端编程的步骤
1. 加载套接字库，创建套接字`WSAStartup()`/`socket()`；
2. 向服务器发出连接请求`connect()`；
3. 和服务器端进行通信`send()`/`recv()`；
4. 关闭套接字，关闭加载的套接字库`closesocket()`/`WSACleanup()`。



## 参考资料

https://www.cnblogs.com/hgwang/p/6074038.html