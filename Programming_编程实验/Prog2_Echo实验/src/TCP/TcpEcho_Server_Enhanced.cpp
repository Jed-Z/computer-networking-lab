/*
 * @Author: Jed
 * @Description: TcpEcho_Server_Enhanced.cpp
 * @Date: 2019-03-11
 * @LastEditTime: 2019-03-20
 */
#include <stdio.h>
#include <time.h>
#include <winsock2.h>
#include <conio.h>
#include <string>
#include <sstream>

#pragma comment(lib, "ws2_32.lib") // 使用winsock 2.2 library
#define BUFLEN 2000				   // 缓冲区大小

void makeNewMsg_enhanced(char *msg, char *timestr, struct sockaddr_in clsin);

int main()
{
	printf("[TCP Echo Server Enhanced]\n正在启动服务器...");

	struct sockaddr_in clsin; // the from address of a client
	struct sockaddr_in sin;   // an Internet endpoint address
	SOCKET msock, ssock;	  // master & slave sockets
	u_short port = 54321;	  // server port

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata); // 加载Winsock library，使用2.0版本

	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 创建主socket
	memset(&sin, 0, sizeof(sin));					   // 清零
	sin.sin_family = AF_INET;						   // 因特网地址簇(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;				   // 监听所有(接口的)IP地址
	sin.sin_port = htons(port);						   // 监听的端口号（网络序）
	bind(msock, (struct sockaddr *)&sin, sizeof(sin)); // 绑定监听的IP地址和端口号
	listen(msock, 5);								   // 建立长度为5的连接请求队列，并把到来的连接请求加入队列等待处理

	time_t now = time(NULL);
	printf("成功。\n服务器启动时间：%s", ctime(&now));
	printf("=======================================\n");

	char buf[BUFLEN + 1]; // 建立缓冲区

	while (!_kbhit())
	{															 // 检测是否有按键,如果没有则进入循环体执行
		int alen = sizeof(struct sockaddr);						 // 取到地址结构的长度
		ssock = accept(msock, (struct sockaddr *)&clsin, &alen); // 从socket，阻塞地从连接请求队列中创建连接

		int recvlen = recv(ssock, buf, BUFLEN, 0); // 接收信息
		buf[recvlen] = '\0';					   // 保证以空字符结尾
		now = time(NULL);
		char *timestr = ctime(&now);
		printf("收到消息：%s\n", buf);
		printf("收到时间：%s", timestr);
		printf("客户端IP地址：%u.%u.%u.%u\n", clsin.sin_addr.S_un.S_un_b.s_b1, clsin.sin_addr.S_un.S_un_b.s_b2, clsin.sin_addr.S_un.S_un_b.s_b3, clsin.sin_addr.S_un.S_un_b.s_b4);
		printf("客户端端口号：%hu\n", clsin.sin_port);
		printf("---------------------------------------\n");

		makeNewMsg_enhanced(buf, timestr, clsin); // 构建要发送给客户端的字符串
		send(ssock, buf, strlen(buf) + 1, 0);	 // 发送信息
		shutdown(ssock, SD_SEND);				  // 禁止收发数据
		closesocket(ssock);						  // 关闭从socket
	}

	closesocket(msock); // 关闭主socket
	WSACleanup();		// 卸载winsock library
	printf("服务器已关闭，按任意键退出。\n");
	getchar();
	return 0;
}

/* 构造要发给客户端的字符串，用到了C++特性 */
void makeNewMsg_enhanced(char *msg, char *timestr, struct sockaddr_in clsin)
{
	using std::endl;
	std::ostringstream os;
	os << "内容：" << msg << endl;
	os << "时间：" << timestr << std::flush;
	os << "客户端IP地址：" << (int)clsin.sin_addr.S_un.S_un_b.s_b1 << '.' << (int)clsin.sin_addr.S_un.S_un_b.s_b2
	   << '.' << (int)clsin.sin_addr.S_un.S_un_b.s_b3 << '.' << (int)clsin.sin_addr.S_un.S_un_b.s_b4 << endl;
	os << "客户端端口号：" << clsin.sin_port << std::flush;
	strncpy(msg, os.str().c_str(), BUFLEN);
}
