#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>
#include <process.h>
#include <string>
#pragma comment(lib, "ws2_32.lib") // ʹ��winsock 2.0 Llibrary

const int BUFLEN = 1000; // ���ͻ�������С
const int RBUFLEN = 10; // ���ջ�������С

unsigned __stdcall recvThread(void *p);