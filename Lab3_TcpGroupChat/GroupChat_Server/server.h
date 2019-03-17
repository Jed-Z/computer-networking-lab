#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <time.h>
#include <process.h>
#include <string>
#include "conio.h"
#pragma comment(lib, "ws2_32.lib") //使用winsock 2.2 library

const int MAXSOCKS = 5;// 最大并发连接数
const size_t BUFLEN = 2000;// 缓冲区大小

struct ThreadInfo {
	int thread_index;//线程被分配至的数组下标
	HANDLE handle;// 句柄，NULL代表尚未被分配
	SOCKET ssock;//和客户端连接的从套接字

	ThreadInfo() :handle(NULL) {}// 构造函数
};
ThreadInfo threadinfo[MAXSOCKS];


unsigned __stdcall serveThread(void* p);//服务一个客户端的线程
void sendToAll(const char* msg);// 向所有客户端发送字符串