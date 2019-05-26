#include "TcpClient.h"

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("[-] Invalid arguments\n");
		printf("Usage: TcpClient.exe IPAddress PortNumber\n");
		exit(-1);
	}

	const char* host = argv[1]; // server IP to connect
	u_short port = atoi(argv[2]);			   // server port to connect
	struct sockaddr_in sin;			   // an Internet endpoint address
	char buf[BUFLEN + 1];			   // buffer for one line of text
	SOCKET sock;					   // socket descriptor

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata); //����winsock library

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	//�����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
	memset(&sin, 0, sizeof(sin));						 // ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
	sin.sin_family = AF_INET;							 // ��������ַ��
	sin.sin_addr.s_addr = inet_addr(host);				 // ������IP��ַ(32λ)
	sin.sin_port = htons(port);							 // �������˿ں�
	if (connect(sock, (struct sockaddr *)&sin, sizeof(sin)) != 0) { // ���ӵ�������
		printf("[-] ʧ�ܣ�%ld\n", GetLastError());
		printf("[-] ��������ĵ�ַ�Ƿ���ȷ�����س��˳���\n");
		exit(1);
	}

	HANDLE recv_thread = (HANDLE)_beginthreadex(NULL, 0, &recvThread, (void *)&sock, 0, NULL);
	while (1)
	{
		fgets(buf, BUFLEN, stdin);
		size_t cmdlen = strlen(buf) - 1;
		buf[cmdlen] = '\r';
		buf[cmdlen+1] = '\n';  // ������"\r\n"��β
		int sendlen = send(sock, buf, cmdlen + 2, 0);  // ��������
	}

	CloseHandle(recv_thread);
	closesocket(sock); // �رռ����׽���
	WSACleanup();	  // ж��winsock library

	return 0;
}

unsigned __stdcall recvThread(void *p)
{
	char rbuf[RBUFLEN + 1]; // ���ջ�����

	SOCKET sock = *(SOCKET *)p;
	while (1)
	{
		int recvlen;
		while ((recvlen = recv(sock, rbuf, RBUFLEN, 0)) > 0) {
			rbuf[recvlen] = '\0';
			printf("%s", rbuf);
		}

		if (recvlen == SOCKET_ERROR)
		{
			printf("[-] Error: %ld.\n", GetLastError());
			exit(-1);
		}
		else if (recvlen == 0)
		{
			printf("[-] Connection closed!\n");
			closesocket(sock); // �رռ����׽���
			WSACleanup();	  // ж��winsock library
			exit(-1);  //�˳�����
			break;
		}
	}

	return 0;
}