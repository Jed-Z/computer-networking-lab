/*
 * @Author: Jed
 * @Description: TcpEcho_Client.c
 * @Date: 2019-03-11
 * @LastEditTime: 2019-03-20
 */
#include <stdio.h>
#include <winsock2.h>
#define BUFLEN 2000				   // ��������С
#pragma comment(lib, "ws2_32.lib") // ʹ��Winsock 2.0 Library

int connectAndSend();

int main()
{
	printf("[TCP Echo Client]\n�ͻ��˿�ʼ���С�Ҫ�˳���������q���س���\n");
	printf("=======================================\n");

	while (connectAndSend());

	printf("�ͻ������н�������������˳���\n");
	getchar();
	return 0;
}

/* ***** connectAndSend()����˵�� ********
 * �ú���ִ�������Ľ������ӺͶϿ����ӹ��̣�
 * д��һ�������еĺô��ǿ��Խ�����һ�γ���
 * �Ϳ��Է��Ͷ�����Ϣ��
 * 
 * �������ķ���ֵ�����û�������q����������
 * �򷵻�0���������������1��
 */
int connectAndSend()
{
	char *host = "127.0.0.1";
	u_short serverport = 54321;
	struct sockaddr_in sin;
	char buf[BUFLEN + 1];
	SOCKET sock;

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata);

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(host);
	sin.sin_port = htons(serverport);
	int ret = connect(sock, (struct sockaddr *)&sin, sizeof(sin));
	if (ret == -1)
	{
		printf("[-] ���ӵ�������ʧ�ܣ�\n");
		closesocket(sock);
		WSACleanup();
		return 0;
	}
	printf("����һ��Ҫ���͵���Ϣ��");
	fgets(buf, BUFLEN, stdin);
	buf[strlen(buf) - 1] = '\0'; // �ѻ��з��滻Ϊ���ַ�
	if (buf[1] == '\0' && (buf[0] == 'q' || buf[0] == 'Q'))
	{
		closesocket(sock);
		WSACleanup();
		return 0;
	} // �û��˳�

	send(sock, buf, strlen(buf) + 1, 0);	  // ������Ϣ����Ϣĩβ�������з�
	int recvlen = recv(sock, buf, BUFLEN, 0); // ������Ϣ
	if (recvlen == SOCKET_ERROR)
	{
		printf("[-] Error: %ld.\n", GetLastError());
	}
	else if (recvlen == 0)
	{
		printf("[-] Server closed!");
	}
	else if (recvlen > 0)
	{
		printf("[+] �յ�����Ϣ��\n");
		buf[recvlen] = '\0';
		printf("%s\n", buf);
		printf("---------------------------------------\n");
	}
	closesocket(sock);
	WSACleanup();
	return 1;
}
