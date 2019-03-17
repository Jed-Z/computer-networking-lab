#include "server.h"

int main()
{
	printf("[Group Chat Server]\n��������Ⱥ�ķ�����...");

	struct sockaddr_in sin;  // an Internet endpoint address
	struct sockaddr_in fsin; // the from address of a client
	u_short port = 50500;

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata);			   // ����winsock library
	SOCKET msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // �����׽���
	memset(&sin, 0, sizeof(sin));					   // ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
	sin.sin_family = AF_INET;						   // ��������ַ��(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;				   // ��������(�ӿڵ�)IP��ַ
	sin.sin_port = htons(port);						   // �����Ķ˿ں�
	bind(msock, (struct sockaddr *)&sin, sizeof(sin)); // �󶨼�����IP��ַ�Ͷ˿ں�
	listen(msock, 5); // �ȴ��������ӵĶ��г���Ϊ5

	time_t now = time(NULL);
	printf("�ɹ���\n����ʱ�䣺%s\n", ctime(&now));
	printf("=======================================\n");

	while (!_kbhit())
	{															// ����Ƿ��а���
		int alen = sizeof(struct sockaddr);							// ȡ����ַ�ṹ�ĳ���
		SOCKET new_ssock = accept(msock, (struct sockaddr*)&fsin, &alen); // ������µ��������󣬷��������׽��֣����򣬱�������fsin�����ͻ���IP��ַ�Ͷ˿ں�

		int ssocks_full = 1;// �Ƿ�ﵽ��󲢷����ı�־λ
		for (int i = 0; i < MAXSOCKS; i++) {
			if (threadinfo[i].handle == NULL) {// �ҵ������е�һ������λ��
				threadinfo[i].thread_index = i;
				threadinfo[i].handle = (HANDLE)_beginthreadex(NULL, 0, &serveThread, (void*)&threadinfo[i], 0, NULL);
				threadinfo[i].ssock = new_ssock;
				ssocks_full = 0;// δ�ﵽ��󲢷����������־λ
				break;
			}
		}
		if (ssocks_full) {// �Ѿ��ﵽ��󲢷������޷����ɸ�������
			const char* sorry_msg = "�Բ���Ⱥ����������������ʱ���ܼ��롣";
			send(new_ssock, sorry_msg, strlen(sorry_msg), 0);
		}
	}
	closesocket(msock); // �رռ����׽���
	WSACleanup();			  // ж��winsock library

	return 0;
}

unsigned __stdcall serveThread(void* p)
{
	ThreadInfo mythread = *(ThreadInfo*)p;
	char buf[BUFLEN + 1];// �̻߳�����

	while (1) {
		int recvlen = recv(mythread.ssock, buf, BUFLEN, 0); // ������Ϣ
		if (recvlen == SOCKET_ERROR)
		{
			printf("[-] Error: %ld.\n", GetLastError());
			printf("---------------------------------------\n");
			break;
		}
		else if (recvlen == 0)
		{
			printf("[-] �ͻ��������˳���\n");
			printf("---------------------------------------\n");
			break;
		}
		else {
			buf[recvlen] = '\0';					   // ��֤�Կ��ַ���β
			printf("�յ���Ϣ��%s\n",buf);
			printf("---------------------------------------\n");

			sendToAll(buf);
		}
	}
	//Call CloseHandle?
	threadinfo[mythread.thread_index].handle = NULL;// ���������Ƴ����߳�
	closesocket(mythread.ssock);
	return 0;
}

void sendToAll(const char* msg)
{
	for (int i = 0; i < MAXSOCKS; i++) {
		if (threadinfo[i].handle != NULL) {
			send(threadinfo[i].ssock, msg, strlen(msg), 0);
		}
	}
}