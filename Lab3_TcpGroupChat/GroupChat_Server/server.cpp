#include "server.h"

int main()
{
	printf("[Group Chat Server]\n正在启动群聊服务器...");

	struct sockaddr_in sin;  // an Internet endpoint address
	struct sockaddr_in fsin; // the from address of a client
	u_short port = 50500;

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata);			   // 加载winsock library
	SOCKET msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 监听套接字
	memset(&sin, 0, sizeof(sin));					   // 从&sin开始的长度为sizeof(sin)的内存清0
	sin.sin_family = AF_INET;						   // 因特网地址簇(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;				   // 监听所有(接口的)IP地址
	sin.sin_port = htons(port);						   // 监听的端口号
	bind(msock, (struct sockaddr *)&sin, sizeof(sin)); // 绑定监听的IP地址和端口号
	listen(msock, 5); // 等待建立连接的队列长度为5

	time_t now = time(NULL);
	printf("成功。\n启动时间：%s\n", ctime(&now));
	printf("=======================================\n");

	while (!_kbhit())
	{															// 检测是否有按键
		int alen = sizeof(struct sockaddr);							// 取到地址结构的长度
		SOCKET new_ssock = accept(msock, (struct sockaddr*)&fsin, &alen); // 如果有新的连接请求，返回连接套接字，否则，被阻塞。fsin包含客户端IP地址和端口号

		int ssocks_full = 1;// 是否达到最大并发数的标志位
		for (int i = 0; i < MAXSOCKS; i++) {
			if (threadinfo[i].handle == NULL) {// 找到数组中的一个可用位置
				threadinfo[i].thread_index = i;
				threadinfo[i].handle = (HANDLE)_beginthreadex(NULL, 0, &serveThread, (void*)&threadinfo[i], 0, NULL);
				threadinfo[i].ssock = new_ssock;
				threadinfo[i].fsin = fsin;
				ssocks_full = 0;// 未达到最大并发数，清除标志位
				CloseHandle(threadinfo[i].handle); // 关闭线程句柄，但并没有结束线程
				break;
			}
		}
		if (ssocks_full) {// 已经达到最大并发数，无法接纳更多连接
			const char* sorry_msg = "对不起，群聊人数已满，您暂时不能加入。";
			send(new_ssock, sorry_msg, strlen(sorry_msg), 0);
		}
	}
	closesocket(msock); // 关闭监听套接字
	WSACleanup();		// 卸载winsock library

	return 0;
}

unsigned __stdcall serveThread(void* p)
{
	ThreadInfo mythread = *(ThreadInfo*)p;
	char buf[BUFLEN + 1];// 线程缓冲区

	printf("发现新客户端加入群聊！\n");
	strncpy(buf, "Enter!", BUFLEN);
	makeReplyMsg(buf, mythread.fsin);
	printf("%s", buf);
	sendToAll(buf);
	printf("---------------------------------------\n");

	while (1) {
		int recvlen = recv(mythread.ssock, buf, BUFLEN, 0); // 接收信息
		if (recvlen == SOCKET_ERROR)
		{
			if (GetLastError() == 10054) {
				strncpy(buf, "Leave!", BUFLEN);
				makeReplyMsg(buf, mythread.fsin);
				printf("%s", buf);
				sendToAll(buf);
			}
			else{
				printf("[-] Error: %ld.\n", GetLastError());
			}
			printf("---------------------------------------\n");
			break;
		}
		else if (recvlen == 0)
		{
			printf("[-] 客户端正常退出。\n");
			printf("---------------------------------------\n");
			break;
		}
		else {
			buf[recvlen] = '\0';					   // 保证以空字符结尾
			makeReplyMsg(buf, mythread.fsin);
			printf("%s", buf);
			sendToAll(buf);
			printf("---------------------------------------\n");
		}
	}
	threadinfo[mythread.thread_index].handle = NULL;// 从数组中移除该线程
	closesocket(mythread.ssock);
	return 0;
}

void sendToAll(const char* msg)
{
	for (int i = 0; i < MAXSOCKS; i++) {
		if (threadinfo[i].handle != NULL) {
			send(threadinfo[i].ssock, msg, strlen(msg), 0);
		}
	}
}

void makeReplyMsg(char* msg, struct sockaddr_in fsin) {
	using namespace std;
	ostringstream os;
	os << "客户端：" << (int)fsin.sin_addr.S_un.S_un_b.s_b1 << '.' << (int)fsin.sin_addr.S_un.S_un_b.s_b2
		<< '.' << (int)fsin.sin_addr.S_un.S_un_b.s_b3 << '.' << (int)fsin.sin_addr.S_un.S_un_b.s_b4
		<< ':' << fsin.sin_port << endl;
	time_t now = time(NULL);
	os << "时间：" << ctime(&now) << flush;
	os << "消息：" << msg << endl;
	strncpy(msg, os.str().c_str(), BUFLEN);
}