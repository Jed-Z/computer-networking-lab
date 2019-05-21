#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>
#include <process.h>
#include <string>
#pragma comment(lib, "ws2_32.lib") // 使用winsock 2.0 Llibrary

const int BUFLEN = 1000; // 发送缓冲区大小
const int RBUFLEN = 10; // 接收缓冲区大小

unsigned __stdcall recvThread(void *p);