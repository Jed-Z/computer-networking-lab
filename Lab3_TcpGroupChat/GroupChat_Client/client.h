#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib") // ʹ��winsock 2.0 Llibrary

const int BUFLEN = 2000; // ��������С

unsigned __stdcall recvThread(void *p);