/*
 * @Author: Jed
 * @Description: TcpEcho_Client.c
 * @Date: 2019-03-11
 * @LastEditTime: 2019-03-20
 */
#include <stdio.h>
#include <winsock2.h>
#define BUFLEN 2000				   // 缓冲区大小
#pragma comment(lib, "ws2_32.lib") // 使用Winsock 2.0 Library

int connectAndSend();

int main()
{
	printf("[TCP Echo Client]\n客户端开始运行。要退出，请输入q并回车。\n");
	printf("=======================================\n");

	while (connectAndSend());

	printf("客户端运行结束，按任意键退出。\n");
	getchar();
	return 0;
}

/* ***** connectAndSend()函数说明 ********
 * 该函数执行完整的建立连接和断开连接过程，
 * 写在一个函数中的好处是可以仅运行一次程序
 * 就可以发送多条信息。
 * 
 * 本函数的返回值：若用户输入了q或遇到错误
 * 则返回0；其他情况均返回1。
 */
int connectAndSend()
{
	char *host = "127.0.0.1";
	u_short serverport = 54321;
	struct sockaddr_in sin;
	char buf[BUFLEN + 1];
	SOCKET sock;

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata);

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(host);
	sin.sin_port = htons(serverport);
	int ret = connect(sock, (struct sockaddr *)&sin, sizeof(sin));
	if (ret == -1)
	{
		printf("[-] 连接到服务器失败！\n");
		closesocket(sock);
		WSACleanup();
		return 0;
	}
	printf("输入一行要发送的消息：");
	fgets(buf, BUFLEN, stdin);
	buf[strlen(buf) - 1] = '\0'; // 把换行符替换为空字符
	if (buf[1] == '\0' && (buf[0] == 'q' || buf[0] == 'Q'))
	{
		closesocket(sock);
		WSACleanup();
		return 0;
	} // 用户退出

	send(sock, buf, strlen(buf) + 1, 0);	  // 发送信息，信息末尾不带换行符
	int recvlen = recv(sock, buf, BUFLEN, 0); // 接收信息
	if (recvlen == SOCKET_ERROR)
	{
		printf("[-] Error: %ld.\n", GetLastError());
	}
	else if (recvlen == 0)
	{
		printf("[-] Server closed!");
	}
	else if (recvlen > 0)
	{
		printf("[+] 收到的消息：\n");
		buf[recvlen] = '\0';
		printf("%s\n", buf);
		printf("---------------------------------------\n");
	}
	closesocket(sock);
	WSACleanup();
	return 1;
}
