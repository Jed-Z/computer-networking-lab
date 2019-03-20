/*
 * @Author: Jed
 * @Description: TcpEcho_Server.c
 * @Date: 2019-03-11
 * @LastEditTime: 2019-03-20
 */
#include <stdio.h>
#include <time.h>
#include <winsock2.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib") // 使用winsock 2.2 library
#define BUFLEN 2000				   // 缓冲区大小

void makeNewMsg(char *msg, char *timestr);

int main()
{
	printf("[TCP Echo Server]\n正在启动服务器...");

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
		printf("---------------------------------------\n");

		makeNewMsg(buf, timestr);			  // 构建要发送给客户端的字符串
		send(ssock, buf, strlen(buf) + 1, 0); // 发送信息
		shutdown(ssock, SD_SEND);			  // 禁止收发数据
		closesocket(ssock);					  // 关闭从socket
	}

	closesocket(msock); // 关闭主socket
	WSACleanup();		// 卸载winsock library
	printf("服务器已关闭，按任意键退出。\n");
	getchar();
	return 0;
}

void makeNewMsg(char *msg, char *timestr)
{
	char tempbuf[BUFLEN + 1];
	strncpy(tempbuf, timestr, BUFLEN);
	strncat(tempbuf, msg, BUFLEN);
	strncpy(msg, tempbuf, BUFLEN);
}
