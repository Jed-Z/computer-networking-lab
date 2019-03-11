#include <stdio.h>
#include <time.h>
#include <winsock2.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib") // ʹ��winsock 2.2 library
#define BUFLEN 2000				   // ��������С

void makeNewMsg(char *msg, char *timestr);

int main()
{
	printf("[TCP Echo Server]\n��������������...");

	struct sockaddr_in clsin; // the from address of a client
	struct sockaddr_in sin;   // an Internet endpoint address
	SOCKET msock, ssock;	  // master & slave sockets
	u_short port = 54321;	  // server port

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata); // ����Winsock library��ʹ��2.0�汾

	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // ������socket
	memset(&sin, 0, sizeof(sin));					   // ����
	sin.sin_family = AF_INET;						   // ��������ַ��(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;				   // ��������(�ӿڵ�)IP��ַ
	sin.sin_port = htons(port);						   // �����Ķ˿ںţ�������
	bind(msock, (struct sockaddr *)&sin, sizeof(sin)); // �󶨼�����IP��ַ�Ͷ˿ں�
	listen(msock, 5);								   // ��������Ϊ5������������У����ѵ������������������еȴ�����

	time_t now = time(NULL);
	printf("�ɹ���\n����������ʱ�䣺%s", ctime(&now));
	printf("=======================================\n");

	char buf[BUFLEN + 1]; // ����������

	while (!_kbhit())
	{															 // ����Ƿ��а���,���û�������ѭ����ִ��
		int alen = sizeof(struct sockaddr);						 // ȡ����ַ�ṹ�ĳ���
		ssock = accept(msock, (struct sockaddr *)&clsin, &alen); // ��socket�������ش�������������д�������

		int recvlen = recv(ssock, buf, BUFLEN, 0); // ������Ϣ
		buf[recvlen] = '\0';					   // ��֤�Կ��ַ���β
		now = time(NULL);
		char *timestr = ctime(&now);
		printf("�յ���Ϣ��%s\n", buf);
		printf("�յ�ʱ�䣺%s", timestr);
		printf("---------------------------------------\n");

		makeNewMsg(buf, timestr);			  // ����Ҫ���͸��ͻ��˵��ַ���
		send(ssock, buf, strlen(buf) + 1, 0); // ������Ϣ
		shutdown(ssock, SD_SEND);			  // ��ֹ�շ�����
		closesocket(ssock);					  // �رմ�socket
	}

	closesocket(msock); // �ر���socket
	WSACleanup();		// ж��winsock library
	printf("�������ѹرգ���������˳���\n");
	getchar();
	return 0;
}

void makeNewMsg(char *msg, char *timestr)
{
	char tempbuf[BUFLEN + 1];
	strncpy(tempbuf, timestr, BUFLEN);
	strncat(tempbuf, msg, BUFLEN);
	strncpy(msg, tempbuf, BUFLEN);
}
