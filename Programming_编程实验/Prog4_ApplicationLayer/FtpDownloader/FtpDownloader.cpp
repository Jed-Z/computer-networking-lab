#include "FtpDownloader.h"

int main(int argc, char *argv[])
{
	printf("[FTP Downloader]\n");

	if (argc != 6) {
		printf("[-] 无效参数\n"
				"Command: FtpDownloader.exe ServerIPAddress User Password FileName DestFile\n"
				"Example: FtpDownloader.exe 172.18.187.10 abc 123666 /ebook/ftp.pdf ftp1.pdf\n");
		exit(-1);
	}

	const char *host = argv[1]; // server IP to connect
	u_short port = 21;			   // server port to connect
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
		printf("连接到 ftp://%s 成功！\n", argv[1]);
		printf("---------------------------------------\n");
	}
	else {
		printf("[-] 失败：%d\n", GetLastError());
		printf("[-] 请检查输入的地址是否正确。按回车退出。\n");
		exit(1);
	}

	/* 接收服务器欢迎信息 */
	recvmsg(sock, buf);

	/* 登录账户 */
	sprintf(buf, "USER %s\r\n", argv[2]);
	sendmsg(sock, buf);
	recvmsg(sock, buf);
	sprintf(buf, "PASS %s\r\n", argv[3]);
	sendmsg(sock, buf);
	recvmsg(sock, buf);

	/* 被动连接 */
	sprintf(buf, "PASV\r\n");
	sendmsg(sock, buf);
	recvmsg(sock, buf);
	SOCKET data_sock = dataConn(buf);

	/* 下载文件 */
	sprintf(buf, "RETR %s\r\n", argv[4]);
	sendmsg(sock, buf);
	recvmsg(sock, buf);
	int filesize = saveFile(data_sock, argv[5]);

	/* 关闭连接 */
	sprintf(buf, "QUIT\r\n");
	sendmsg(sock, buf);
	recvmsg(sock, buf);

	closesocket(sock); // 关闭监听套接字
	WSACleanup();	  // 卸载winsock library

	printf("---------------------------------------\n");
	printf("下载结束：%s，共传输 %d 字节。\n", argv[5], filesize);

	return 0;
}

int sendmsg(SOCKET sock, char* msg) {
	printf(">>> ");
	printf(msg);  //显示消息
	return send(sock, msg, strlen(msg) + 1, 0);  // 发送消息
}

int recvmsg(SOCKET sock, char* buf) {
	int result = recv(sock, buf, BUFLEN, 0);
	buf[result] = '\0';
	printf("[+] ");
	printf(buf);
	return result;
}

SOCKET dataConn(char* received_msg) {
	SOCKET result = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	int ip1, ip2, ip3, ip4, port1, port2;
	char data_ipaddr[17];
	int data_port;
	char* pos = strchr(received_msg, '(');
	sscanf(pos + 1, "%d,%d,%d,%d,%d,%d", &ip1, &ip2, &ip3, &ip4, &port1, &port2);
	sprintf(data_ipaddr, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
	data_port = port1 * 256 + port2;

	struct sockaddr_in data_sin;			   // an Internet endpoint address
	memset(&data_sin, 0, sizeof(data_sin));
	data_sin.sin_family = AF_INET;							 // 因特网地址簇
	data_sin.sin_addr.s_addr = inet_addr(data_ipaddr);				 // 服务器IP地址(32位)
	data_sin.sin_port = htons(data_port);							 // 服务器端口号

	connect(result, (struct sockaddr *)&data_sin, sizeof(data_sin));
	return result;
}

int saveFile(SOCKET sock, char* fileName) {
	int size = 0;  // 下载的文件大小
	char filebuf[FILEBUFLEN+1];
	FILE* destfile = fopen(fileName, "wb");
	int recvlen = 0;  // 每次接收到的数据量
	while ((recvlen = recv(sock, filebuf, FILEBUFLEN, 0)) > 0) {  // 循环接收数据，直到文件传输完毕
		size += recvlen;
		fwrite(filebuf, 1, FILEBUFLEN, destfile);
	}
	fclose(destfile);
	return size;  // 返回下载的文件大小
}