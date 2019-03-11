#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>

#define BUFLEN 2000				   // 缓冲区大小
#pragma comment(lib, "ws2_32.lib") // 加载winsock 2.2 Llibrary

int connectAndSendto();
int main()
{
	printf("[UDP Echo Client]\n客户端开始运行。要退出，请输入q并回车。\n");
	printf("=======================================\n");

	while (connectAndSendto());

	printf("客户端运行结束，按任意键退出。\n");
	getchar();
	return 0;
}

int connectAndSendto()
{
	char *host = "127.0.0.1";
	u_short serverport = 54322;
	struct sockaddr_in toAddr;
	SOCKET sock;

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	memset(&toAddr, 0, sizeof(toAddr));
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(serverport);
	toAddr.sin_addr.s_addr = inet_addr(host);

	char buf[BUFLEN + 1]; // 建立缓冲区

	printf("输入消息：");
	fgets(buf, BUFLEN, stdin);
	buf[strlen(buf) - 1] = '\0'; // 将换行符替换为空字符
	if (buf[1] == '\0' && (buf[0] == 'q' || buf[0] == 'Q'))
	{
		closesocket(sock);
		WSACleanup();
		return 0;
	}
	int sendlen = sendto(sock, buf, BUFLEN, 0, (SOCKADDR *)&toAddr, sizeof(toAddr));
	if (sendlen == SOCKET_ERROR)
	{
		printf("[-] 发送失败，错误号：%d", WSAGetLastError());
	}
	else
	{
		int tosize = sizeof(toAddr);
		int recvlen = recvfrom(sock, buf, BUFLEN, 0, (SOCKADDR *)&toAddr, &tosize);
		if (recvlen == SOCKET_ERROR)
		{
			printf("[-] recvfrom() failed; %d\n", WSAGetLastError());
			return 0;
		}
		else if (recvlen == 0)
		{
			return 0;
		}
		else
		{
			printf("\n%s", buf);
			printf("---------------------------------------\n");
		}
	}

	closesocket(sock);
	WSACleanup();
	return 1;
}
