#include <stdio.h>
#include <winsock2.h>
#include <io.h>
#include <string>
#define BUFLEN 2000				   // 缓冲区大小
#pragma comment(lib, "ws2_32.lib") // 使用Winsock 2.0 Library
using namespace std;

int connectAndSend();

int main()
{
	printf("[File Transfer Client]\n客户端开始运行。要退出，请输入`exit`。\n");
	printf("=======================================\n");

	while (connectAndSend());

	printf("客户端运行结束，按任意键退出。\n");
	getchar();
	return 0;
}

/* ***** connectAndSend()函数说明 ********
 * 该函数执行完整的建立连接和断开连接过程，
 * 写在一个函数中的好处是可以仅运行一次程序
 * 就可以多次接收文件。
 * 
 * 本函数的返回值：若用户输入了exit或遇到错误
 * 则返回0；其他情况均返回1。
 */
int connectAndSend()
{
	char *host = "127.0.0.1";
	u_short serverport = 50500;
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
	
	char filename[128] = {0};
	do {
		printf("输入要传送的文件名：");
		fgets(filename, BUFLEN, stdin);
		filename[strlen(filename) - 1] = '\0'; // 把换行符替换为空字符
		if (strcmp("exit", filename) == 0)
		{
			closesocket(sock);
			WSACleanup();
			return 0;
		} // 用户退出
		if(access(filename, R_OK) != 0) {
			fprintf(stderr, "[-] 无效文件名！\n");
		}
	} while(access(filename, R_OK) != 0);

	size_t filename_lastslash = string(filename).find_last_of('\\');  // 从完整路径中文件名
	if(filename_lastslash == string::npos) {
		send(sock, filename, strlen(filename) + 1, 0);	  // 发送文件名
	}
	else {
		string filename_partial = string(filename).substr(filename_lastslash+1);
		send(sock, filename_partial.c_str(), strlen(filename) + 1, 0);  // 不带路径的文件名 
	}
	
	
	FILE* fp = fopen(filename, "rb");
	int length;
	/* 循环读取文件并发送 */
	while((length = (int)fread(buf, sizeof(char), BUFLEN, fp)) > 0) {
		send(sock, buf, length, 0);
	}
	fclose(fp);
	shutdown(sock, SD_SEND);  // 发送FIN包，表示发送完毕 
	
	int recvlen = recv(sock, buf, BUFLEN, 0); // 接收反馈信息
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
		buf[recvlen] = '\0';
		printf("%s\n", buf);
		printf("---------------------------------------\n");
	}
	closesocket(sock);
	WSACleanup();
	return 1;
}