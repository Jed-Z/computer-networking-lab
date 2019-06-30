#include <stdio.h>
#include <time.h>
#include <winsock2.h>
#include <conio.h>
#include <string>
#include <io.h>
#pragma comment(lib, "ws2_32.lib") // 使用winsock 2.2 library
#define BUFLEN 2000				   // 缓冲区大小
using namespace std;

void validateFilename(const char* folder, char* filename);

int main()
{
	printf("[File Transfer Server]\n正在启动服务器...");

	struct sockaddr_in clsin; // the from address of a client
	struct sockaddr_in sin;   // an Internet endpoint address
	SOCKET msock, ssock;	  // master & slave sockets
	u_short port = 50500;	  // server port

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata); // 加载Winsock library，使用2.0版本

	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 创建主socket
	memset(&sin, 0, sizeof(sin));					   // 清零
	sin.sin_family = AF_INET;						   // 因特网地址簇(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;				   // 监听所有(接口的)IP地址
	sin.sin_port = htons(port);						   // 监听的端口号（网络序）
	bind(msock, (struct sockaddr *)&sin, sizeof(sin)); // 绑定监听的IP地址和端口号
	listen(msock, 5);								   // 建立长度为5的连接请求队列，并把到来的连接请求加入队列等待处理

	time_t now = time(NULL);
	printf("成功。\n服务器启动时间：%s", ctime(&now));
	printf("=======================================\n");
	
	char folder[100] = {0}; 
	do {
		printf("输入接收文件夹：");
		scanf("%s", folder);
		if(access(folder, F_OK) != 0) {  // 检查目录是否存在且有访问权限 
			fprintf(stderr, "[-] 无效目录！\n");
		}
	} while(access(folder, F_OK) != 0);
	if(folder[strlen(folder)-1] != '\\') {
		folder[strlen(folder)+1] = '\0';
		folder[strlen(folder)] = '\\';  // 保证目录以反斜杠结尾 
	}
	printf("接收文件夹已设置为 %s\n准备就绪，可以开始传输文件\n", folder);
	printf("=======================================\n");

	char buf[BUFLEN + 1]; // 建立缓冲区

	while (!_kbhit())
	{															 // 检测是否有按键,如果没有则进入循环体执行
		int alen = sizeof(struct sockaddr);						 // 取到地址结构的长度
		ssock = accept(msock, (struct sockaddr *)&clsin, &alen); // 从socket，阻塞地从连接请求队列中创建连接

		char filename[100] = {0};
		int recvlen = recv(ssock, filename, 100, 0); // 接收信息
		buf[recvlen] = '\0';					   // 保证以空字符结尾
		
		now = time(NULL);
		char *timestr = ctime(&now);
		printf("%s", timestr);
		printf("正在接收文件：%s\n", filename);
		
		validateFilename(folder, filename); // 为重名文件添加序号
		
		string full_filename = (string(folder) + filename);
		FILE* fp = fopen(full_filename.c_str(), "wb");
		int length = 0;
		/* 循环接收数据并写入文件 */
		while((length = recv(ssock, buf, BUFLEN, 0)) > 0) {
			fwrite(buf, sizeof(char), length, fp);
		}
		fclose(fp);
		
		printf("接收完毕，已保存至 %s\n", full_filename.c_str());
		printf("---------------------------------------\n");

		strncpy(buf, "服务器已成功接收文件。\n", BUFLEN);
		send(ssock, buf, strlen(buf) + 1, 0); // 发送信息
		shutdown(ssock, SD_SEND);			  // 禁止收发数据
		closesocket(ssock);					  // 关闭从socket
	}

	closesocket(msock); // 关闭主socket
	WSACleanup();		// 卸载winsock library
	printf("服务器已关闭，按任意键退出。\n");
	getchar();
	return 0;
}

/* 构造有效的文件名（即为重名文件添加序号） */
void validateFilename(const char* folder, char* filename)
{
	string full_filename = (string(folder) + filename);
	char temp[300];
	char numstr[10];
	int num = 1;
	while(access(full_filename.c_str(), 0)==0) {	// 文件已存在（重名）
		int dotpos = strrchr(filename, '.') - filename;	// 从右往左找到小数点的位置
		if(strrchr(filename, '(')!=NULL && strrchr(filename, ')')!=NULL) {
			int leftpos = strrchr(filename, '(') - filename;	// 左括号的位置
			strncpy(temp, filename+dotpos, 300);
			filename[leftpos] = '\0';
			strncat(filename, temp, 300);	// 恢复不带序号的文件名
		}
		dotpos = strrchr(filename, '.') - filename;	// 更新小数点的位置
		num++;							// 递增序号
		numstr[0] = '(';				// 添加左括号
		itoa(num, numstr+1, 10);		// 添加序号
		numstr[strlen(numstr)+1] = '\0';
		numstr[strlen(numstr)] = ')';	// 添加右括号
		strncpy(temp, filename, dotpos);
		temp[dotpos] = '\0';
		strcat(temp, numstr);
		strcat(temp, filename + dotpos);
		strncpy(filename, temp, 300);	// 把添加了序号的文件名拷贝回filename
		
		full_filename = (string(folder) + filename);
	}
}