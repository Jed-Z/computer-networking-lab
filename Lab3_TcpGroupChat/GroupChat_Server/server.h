#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <time.h>
#include <process.h>
#include <string>
#include <sstream>
#include "conio.h"
#pragma comment(lib, "ws2_32.lib") //ʹ��winsock 2.2 library

const int MAXSOCKS = 5;// ��󲢷�������
const size_t BUFLEN = 2000;// ��������С

struct ThreadInfo {
	int thread_index;//�̱߳��������������±�
	HANDLE handle;// �����NULL������δ������
	SOCKET ssock;//�Ϳͻ������ӵĴ��׽���
	struct sockaddr_in fsin;//�ͻ��˵�ַ

	ThreadInfo() :handle(NULL) {}// ���캯��
};
ThreadInfo threadinfo[MAXSOCKS];


unsigned __stdcall serveThread(void* p);//����һ���ͻ��˵��߳�
void sendToAll(const char* msg);// �����пͻ��˷����ַ���
void makeReplyMsg(char* msg, struct sockaddr_in fsin);// ����Ϣǰ����ϸÿͻ��˵�IP��ַ�Ͷ˿ں��Լ��������ĵ�ǰʱ��