#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>

#define BUFLEN 2000				   // ��������С
#pragma comment(lib, "ws2_32.lib") // ����winsock 2.2 Llibrary

int connectAndSendto();
int main()
{
	printf("[UDP Echo Client]\n�ͻ��˿�ʼ���С�Ҫ�˳���������q���س���\n");
	printf("=======================================\n");

	while (connectAndSendto());

	printf("�ͻ������н�������������˳���\n");
	getchar();
	return 0;
}

int connectAndSendto()
{
	char *host = "127.0.0.1";
	u_short serverport = 54322;
	struct sockaddr_in toAddr;
	SOCKET sock;

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	memset(&toAddr, 0, sizeof(toAddr));
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(serverport);
	toAddr.sin_addr.s_addr = inet_addr(host);

	char buf[BUFLEN + 1]; // ����������

	printf("������Ϣ��");
	fgets(buf, BUFLEN, stdin);
	buf[strlen(buf) - 1] = '\0'; // �����з��滻Ϊ���ַ�
	if (buf[1] == '\0' && (buf[0] == 'q' || buf[0] == 'Q'))
	{
		closesocket(sock);
		WSACleanup();
		return 0;
	}
	int sendlen = sendto(sock, buf, BUFLEN, 0, (SOCKADDR *)&toAddr, sizeof(toAddr));
	if (sendlen == SOCKET_ERROR)
	{
		printf("[-] ����ʧ�ܣ�����ţ�%d", WSAGetLastError());
	}
	else
	{
		int tosize = sizeof(toAddr);
		int recvlen = recvfrom(sock, buf, BUFLEN, 0, (SOCKADDR *)&toAddr, &tosize);
		if (recvlen == SOCKET_ERROR)
		{
			printf("[-] recvfrom() failed; %d\n", WSAGetLastError());
			return 0;
		}
		else if (recvlen == 0)
		{
			return 0;
		}
		else
		{
			printf("\n%s", buf);
			printf("---------------------------------------\n");
		}
	}

	closesocket(sock);
	WSACleanup();
	return 1;
}
