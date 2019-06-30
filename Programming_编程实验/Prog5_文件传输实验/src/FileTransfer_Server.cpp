#include <stdio.h>
#include <time.h>
#include <winsock2.h>
#include <conio.h>
#include <string>
#include <io.h>
#pragma comment(lib, "ws2_32.lib") // ʹ��winsock 2.2 library
#define BUFLEN 2000				   // ��������С
using namespace std;

void validateFilename(const char* folder, char* filename);

int main()
{
	printf("[File Transfer Server]\n��������������...");

	struct sockaddr_in clsin; // the from address of a client
	struct sockaddr_in sin;   // an Internet endpoint address
	SOCKET msock, ssock;	  // master & slave sockets
	u_short port = 50500;	  // server port

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
	
	char folder[100] = {0}; 
	do {
		printf("��������ļ��У�");
		scanf("%s", folder);
		if(access(folder, F_OK) != 0) {  // ���Ŀ¼�Ƿ�������з���Ȩ�� 
			fprintf(stderr, "[-] ��ЧĿ¼��\n");
		}
	} while(access(folder, F_OK) != 0);
	if(folder[strlen(folder)-1] != '\\') {
		folder[strlen(folder)+1] = '\0';
		folder[strlen(folder)] = '\\';  // ��֤Ŀ¼�Է�б�ܽ�β 
	}
	printf("�����ļ���������Ϊ %s\n׼�����������Կ�ʼ�����ļ�\n", folder);
	printf("=======================================\n");

	char buf[BUFLEN + 1]; // ����������

	while (!_kbhit())
	{															 // ����Ƿ��а���,���û�������ѭ����ִ��
		int alen = sizeof(struct sockaddr);						 // ȡ����ַ�ṹ�ĳ���
		ssock = accept(msock, (struct sockaddr *)&clsin, &alen); // ��socket�������ش�������������д�������

		char filename[100] = {0};
		int recvlen = recv(ssock, filename, 100, 0); // ������Ϣ
		buf[recvlen] = '\0';					   // ��֤�Կ��ַ���β
		
		now = time(NULL);
		char *timestr = ctime(&now);
		printf("%s", timestr);
		printf("���ڽ����ļ���%s\n", filename);
		
		validateFilename(folder, filename); // Ϊ�����ļ�������
		
		string full_filename = (string(folder) + filename);
		FILE* fp = fopen(full_filename.c_str(), "wb");
		int length = 0;
		/* ѭ���������ݲ�д���ļ� */
		while((length = recv(ssock, buf, BUFLEN, 0)) > 0) {
			fwrite(buf, sizeof(char), length, fp);
		}
		fclose(fp);
		
		printf("������ϣ��ѱ����� %s\n", full_filename.c_str());
		printf("---------------------------------------\n");

		strncpy(buf, "�������ѳɹ������ļ���\n", BUFLEN);
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

/* ������Ч���ļ�������Ϊ�����ļ������ţ� */
void validateFilename(const char* folder, char* filename)
{
	string full_filename = (string(folder) + filename);
	char temp[300];
	char numstr[10];
	int num = 1;
	while(access(full_filename.c_str(), 0)==0) {	// �ļ��Ѵ��ڣ�������
		int dotpos = strrchr(filename, '.') - filename;	// ���������ҵ�С�����λ��
		if(strrchr(filename, '(')!=NULL && strrchr(filename, ')')!=NULL) {
			int leftpos = strrchr(filename, '(') - filename;	// �����ŵ�λ��
			strncpy(temp, filename+dotpos, 300);
			filename[leftpos] = '\0';
			strncat(filename, temp, 300);	// �ָ�������ŵ��ļ���
		}
		dotpos = strrchr(filename, '.') - filename;	// ����С�����λ��
		num++;							// �������
		numstr[0] = '(';				// ���������
		itoa(num, numstr+1, 10);		// ������
		numstr[strlen(numstr)+1] = '\0';
		numstr[strlen(numstr)] = ')';	// ���������
		strncpy(temp, filename, dotpos);
		temp[dotpos] = '\0';
		strcat(temp, numstr);
		strcat(temp, filename + dotpos);
		strncpy(filename, temp, 300);	// ���������ŵ��ļ���������filename
		
		full_filename = (string(folder) + filename);
	}
}