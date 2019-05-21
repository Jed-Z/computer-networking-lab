/*
 * @Author: Jed
 * @Description: client.cpp
 * @Date: 2019-03-23
 * @LastEditTime: 2019-03-23
 */
#include "client.h"

int main(int argc, char *argv[])
{
	printf("[Group Chat Client]\n群聊客户端正在连接服务器...");

	const char *host = "127.0.0.1"; // server IP to connect
	u_short port = 50500;			   // server port to connect
	struct sockaddr_in sin;			   // an Internet endpoint address
	char buf[BUFLEN + 1];			   // buffer for one line of text
	SOCKET sock;					   // socket descriptor
	int cc;							   // recv character count

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata); //加载winsock library

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	//创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
	memset(&sin, 0, sizeof(sin));						 // 从&sin开始的长度为sizeof(sin)的内存清0
	sin.sin_family = AF_INET;							 // 因特网地址簇
	sin.sin_addr.s_addr = inet_addr(host);				 // 服务器IP地址(32位)
	sin.sin_port = htons(port);							 // 服务器端口号
	if (connect(sock, (struct sockaddr *)&sin, sizeof(sin)) == 0) { // 连接到服务器
		printf("连接成功！\n\n");
		printf("***************************************\n");
	}
	else {
		printf("失败：%d\n", GetLastError());
		printf("[-] 请检查网络连接，并确认服务器已启动。按任意键关闭客户端。\n");
		getchar();
		exit(1);
	}

	HANDLE recv_thread = (HANDLE)_beginthreadex(NULL, 0, &recvThread, (void *)&sock, 0, NULL);
	while (1)
	{
		fgets(buf, BUFLEN, stdin);
		buf[strlen(buf) - 1] = '\0'; // 把换行符替换为空字符
		if (std::string(buf) == "exit") {// 用户输入了exit，正常退出
			break;
		}
		int sendlen = send(sock, buf, strlen(buf) + 1, 0);
	}

	CloseHandle(recv_thread);
	closesocket(sock); // 关闭监听套接字
	WSACleanup();	  // 卸载winsock library

	printf("按回车键继续...");
	getchar();

	return 0;
}

unsigned __stdcall recvThread(void *p)
{
	char buf[BUFLEN + 1]; // 接收缓冲区

	SOCKET sock = *(SOCKET *)p;
	while (1)
	{
		printf(">>> ");// 提示符
		int recvlen = recv(sock, buf, BUFLEN, 0);
		printf("\r");// 回到行首，覆盖掉之前显示的提示符
		if (recvlen == SOCKET_ERROR)
		{
			printf("[-] Error: %ld. 按任意键关闭客户端。\n", GetLastError());
			getchar();
			exit(1);
		}
		else if (recvlen == 0)
		{
			printf("[-] Server closed!");
			break;
		}
		else if (recvlen > 0)
		{
			printf("[+] 收到消息：\n");
			buf[recvlen] = '\0';
			printf("%s\n", buf);
		}
		printf("---------------------------------------\n");
	}

	return 0;
}