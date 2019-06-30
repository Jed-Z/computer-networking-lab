#include <stdio.h>
#include <winsock2.h>
#include <io.h>
#include <string>
#define BUFLEN 2000				   // ��������С
#pragma comment(lib, "ws2_32.lib") // ʹ��Winsock 2.0 Library
using namespace std;

int connectAndSend();

int main()
{
	printf("[File Transfer Client]\n�ͻ��˿�ʼ���С�Ҫ�˳���������`exit`��\n");
	printf("=======================================\n");

	while (connectAndSend());

	printf("�ͻ������н�������������˳���\n");
	getchar();
	return 0;
}

/* ***** connectAndSend()����˵�� ********
 * �ú���ִ�������Ľ������ӺͶϿ����ӹ��̣�
 * д��һ�������еĺô��ǿ��Խ�����һ�γ���
 * �Ϳ��Զ�ν����ļ���
 * 
 * �������ķ���ֵ�����û�������exit����������
 * �򷵻�0���������������1��
 */
int connectAndSend()
{
	char *host = "127.0.0.1";
	u_short serverport = 50500;
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
	
	char filename[128] = {0};
	do {
		printf("����Ҫ���͵��ļ�����");
		fgets(filename, BUFLEN, stdin);
		filename[strlen(filename) - 1] = '\0'; // �ѻ��з��滻Ϊ���ַ�
		if (strcmp("exit", filename) == 0)
		{
			closesocket(sock);
			WSACleanup();
			return 0;
		} // �û��˳�
		if(access(filename, R_OK) != 0) {
			fprintf(stderr, "[-] ��Ч�ļ�����\n");
		}
	} while(access(filename, R_OK) != 0);

	size_t filename_lastslash = string(filename).find_last_of('\\');  // ������·�����ļ���
	if(filename_lastslash == string::npos) {
		send(sock, filename, strlen(filename) + 1, 0);	  // �����ļ���
	}
	else {
		string filename_partial = string(filename).substr(filename_lastslash+1);
		send(sock, filename_partial.c_str(), strlen(filename) + 1, 0);  // ����·�����ļ��� 
	}
	
	
	FILE* fp = fopen(filename, "rb");
	int length;
	/* ѭ����ȡ�ļ������� */
	while((length = (int)fread(buf, sizeof(char), BUFLEN, fp)) > 0) {
		send(sock, buf, length, 0);
	}
	fclose(fp);
	shutdown(sock, SD_SEND);  // ����FIN������ʾ������� 
	
	int recvlen = recv(sock, buf, BUFLEN, 0); // ���շ�����Ϣ
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
		buf[recvlen] = '\0';
		printf("%s\n", buf);
		printf("---------------------------------------\n");
	}
	closesocket(sock);
	WSACleanup();
	return 1;
}