/*
 * @Author: Jed
 * @Description: client.h
 * @Date: 2019-03-23
 * @LastEditTime: 2019-03-23
 */
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>
#include <process.h>
#include <string>
#pragma comment(lib, "ws2_32.lib") // 使用winsock 2.0 Llibrary

const int BUFLEN = 2000; // 缓冲区大小

unsigned __stdcall recvThread(void *p);