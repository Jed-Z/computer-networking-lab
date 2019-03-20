/*
 * @Author: Jed
 * @Description: UdpEcho_Server.cpp
 * @Date: 2019-03-11
 * @LastEditTime: 2019-03-20
 */
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib") // 加载Winsock 2.2 Library
#define BUFLEN 2000				   // 缓冲区大小

void makeNewMsg(char *msg, char *timestr, struct sockaddr_in from);
int main()
{
	printf("[UDP Echo Server]\n正在启动服务器...");

	u_short port = 54322;	// server port to connect
	struct sockaddr_in sin;  // an Internet endpoint address
	struct sockaddr_in from; // sender address
	int fromsize = sizeof(from);
	SOCKET sock; // socket descriptor

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata); // 加载winsock library

	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); // 创建UDP socket
	memset(&sin, 0, sizeof(sin));					 // 清零
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	bind(sock, (struct sockaddr *)&sin, sizeof(sin)); //绑定本地端口号（和本地IP地址）

	time_t now = time(NULL);
	printf("成功。\n服务器启动时间：%s", ctime(&now));
	printf("=======================================\n");

	char buf[BUFLEN + 1]; // 建立缓冲区

	while (!_kbhit())
	{
		int recvlen = recvfrom(sock, buf, BUFLEN, 0, (SOCKADDR *)&from, &fromsize);
		if (recvlen == SOCKET_ERROR)
		{
			printf("[-] recvfrom() failed; %d\n", WSAGetLastError());
			break;
		}
		else if (recvlen == 0)
		{
			break;
		}
		else
		{
			time_t now = time(NULL);
			char *timestr = ctime(&now);
			buf[recvlen] = '\0'; // 保证以空字符结尾
			printf("客户端的消息：%s\n", buf);
			printf("客户端IP地址：%u.%u.%u.%u\n", from.sin_addr.S_un.S_un_b.s_b1, from.sin_addr.S_un.S_un_b.s_b2, from.sin_addr.S_un.S_un_b.s_b3, from.sin_addr.S_un.S_un_b.s_b4);
			printf("客户端端口号：%hu\n", from.sin_port);
			printf("时间：%s", timestr);
			printf("---------------------------------------\n");

			makeNewMsg(buf, timestr, from);
			sendto(sock, buf, BUFLEN, 0, (SOCKADDR *)&from, sizeof(from));
		}
	}
	closesocket(sock);
	WSACleanup();

	printf("服务器已关闭，按任意键退出。\n");
	getchar();
	return 0;
}

/* 构造要发给客户端的字符串，用到了C++特性 */
void makeNewMsg(char *msg, char *timestr, struct sockaddr_in from)
{
	using std::endl;
	std::ostringstream os;
	os << "客户端的消息：" << msg << endl;
	os << "客户端IP地址：" << (int)from.sin_addr.S_un.S_un_b.s_b1 << '.' << (int)from.sin_addr.S_un.S_un_b.s_b2
	   << '.' << (int)from.sin_addr.S_un.S_un_b.s_b3 << '.' << (int)from.sin_addr.S_un.S_un_b.s_b4 << endl;
	os << "客户端端口号：" << from.sin_port << endl;
	os << "时间：" << timestr << std::flush;
	strncpy(msg, os.str().c_str(), BUFLEN);
}
