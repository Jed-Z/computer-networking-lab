/*
 * @Author: Jed
 * @Description: UdpEcho_Server.cpp
 * @Date: 2019-03-11
 * @LastEditTime: 2019-03-20
 */
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib") // ����Winsock 2.2 Library
#define BUFLEN 2000				   // ��������С

void makeNewMsg(char *msg, char *timestr, struct sockaddr_in from);
int main()
{
	printf("[UDP Echo Server]\n��������������...");

	u_short port = 54322;	// server port to connect
	struct sockaddr_in sin;  // an Internet endpoint address
	struct sockaddr_in from; // sender address
	int fromsize = sizeof(from);
	SOCKET sock; // socket descriptor

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata); // ����winsock library

	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); // ����UDP socket
	memset(&sin, 0, sizeof(sin));					 // ����
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	bind(sock, (struct sockaddr *)&sin, sizeof(sin)); //�󶨱��ض˿ںţ��ͱ���IP��ַ��

	time_t now = time(NULL);
	printf("�ɹ���\n����������ʱ�䣺%s", ctime(&now));
	printf("=======================================\n");

	char buf[BUFLEN + 1]; // ����������

	while (!_kbhit())
	{
		int recvlen = recvfrom(sock, buf, BUFLEN, 0, (SOCKADDR *)&from, &fromsize);
		if (recvlen == SOCKET_ERROR)
		{
			printf("[-] recvfrom() failed; %d\n", WSAGetLastError());
			break;
		}
		else if (recvlen == 0)
		{
			break;
		}
		else
		{
			time_t now = time(NULL);
			char *timestr = ctime(&now);
			buf[recvlen] = '\0'; // ��֤�Կ��ַ���β
			printf("�ͻ��˵���Ϣ��%s\n", buf);
			printf("�ͻ���IP��ַ��%u.%u.%u.%u\n", from.sin_addr.S_un.S_un_b.s_b1, from.sin_addr.S_un.S_un_b.s_b2, from.sin_addr.S_un.S_un_b.s_b3, from.sin_addr.S_un.S_un_b.s_b4);
			printf("�ͻ��˶˿ںţ�%hu\n", from.sin_port);
			printf("ʱ�䣺%s", timestr);
			printf("---------------------------------------\n");

			makeNewMsg(buf, timestr, from);
			sendto(sock, buf, BUFLEN, 0, (SOCKADDR *)&from, sizeof(from));
		}
	}
	closesocket(sock);
	WSACleanup();

	printf("�������ѹرգ���������˳���\n");
	getchar();
	return 0;
}

/* ����Ҫ�����ͻ��˵��ַ������õ���C++���� */
void makeNewMsg(char *msg, char *timestr, struct sockaddr_in from)
{
	using std::endl;
	std::ostringstream os;
	os << "�ͻ��˵���Ϣ��" << msg << endl;
	os << "�ͻ���IP��ַ��" << (int)from.sin_addr.S_un.S_un_b.s_b1 << '.' << (int)from.sin_addr.S_un.S_un_b.s_b2
	   << '.' << (int)from.sin_addr.S_un.S_un_b.s_b3 << '.' << (int)from.sin_addr.S_un.S_un_b.s_b4 << endl;
	os << "�ͻ��˶˿ںţ�" << from.sin_port << endl;
	os << "ʱ�䣺" << timestr << std::flush;
	strncpy(msg, os.str().c_str(), BUFLEN);
}
