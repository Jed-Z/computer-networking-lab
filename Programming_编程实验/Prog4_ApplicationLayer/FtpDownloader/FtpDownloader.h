#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#pragma comment(lib, "ws2_32.lib") // ʹ��winsock 2.0 Llibrary

const int BUFLEN = 2000; // ��������С
const int FILEBUFLEN = 10000;//�����ļ��Ļ�������С

int sendmsg(SOCKET sock, char* msg);
int recvmsg(SOCKET sock, char* buf);
SOCKET dataConn(char* received_msg);
int saveFile(SOCKET sock, char* fileName);