#include "TcpClient.h"

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("[-] Invalid arguments\n");
		printf("Usage: TcpClient.exe IPAddress PortNumber\n");
		exit(-1);
	}

	const char* host = argv[1]; // server IP to connect
	u_short port = atoi(argv[2]);			   // server port to connect
	struct sockaddr_in sin;			   // an Internet endpoint address
	char buf[BUFLEN + 1];			   // buffer for one line of text
	SOCKET sock;					   // socket descriptor

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata); //加载winsock library

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	//创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
	memset(&sin, 0, sizeof(sin));						 // 从&sin开始的长度为sizeof(sin)的内存清0
	sin.sin_family = AF_INET;							 // 因特网地址簇
	sin.sin_addr.s_addr = inet_addr(host);				 // 服务器IP地址(32位)
	sin.sin_port = htons(port);							 // 服务器端口号
	if (connect(sock, (struct sockaddr *)&sin, sizeof(sin)) != 0) { // 连接到服务器
		printf("[-] 失败：%ld\n", GetLastError());
		printf("[-] 请检查输入的地址是否正确。按回车退出。\n");
		exit(1);
	}

	HANDLE recv_thread = (HANDLE)_beginthreadex(NULL, 0, &recvThread, (void *)&sock, 0, NULL);
	while (1)
	{
		fgets(buf, BUFLEN, stdin);
		size_t cmdlen = strlen(buf) - 1;
		buf[cmdlen] = '\r';
		buf[cmdlen+1] = '\n';  // 命令以"\r\n"结尾
		int sendlen = send(sock, buf, cmdlen + 2, 0);  // 发送命令
	}

	CloseHandle(recv_thread);
	closesocket(sock); // 关闭监听套接字
	WSACleanup();	  // 卸载winsock library

	return 0;
}

unsigned __stdcall recvThread(void *p)
{
	char rbuf[RBUFLEN + 1]; // 接收缓冲区

	SOCKET sock = *(SOCKET *)p;
	while (1)
	{
		int recvlen;
		while ((recvlen = recv(sock, rbuf, RBUFLEN, 0)) > 0) {
			rbuf[recvlen] = '\0';
			printf("%s", rbuf);
		}

		if (recvlen == SOCKET_ERROR)
		{
			printf("[-] Error: %ld.\n", GetLastError());
			exit(-1);
		}
		else if (recvlen == 0)
		{
			printf("[-] Connection closed!\n");
			closesocket(sock); // 关闭监听套接字
			WSACleanup();	  // 卸载winsock library
			exit(-1);  //退出程序
			break;
		}
	}

	return 0;
}