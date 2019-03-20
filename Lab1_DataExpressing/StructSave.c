/*
 * @Author: Jed
 * @Description: StructSave.c
 * @Date: 2019-03-08
 * @LastEditTime: 2019-03-20
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN 100
#define USER_NAME_LEN 20
#define EMAIL_LEN 80
#define TIME_BUF_LEN 30

typedef unsigned long DWORD;

struct Person
{
	char username[USER_NAME_LEN]; // 员工名
	int level;					  // 工资级别
	char email[EMAIL_LEN];		  // Email地址
	DWORD sendtime;               // 发送时间（填写每个用户信息的时间）
	time_t regtime;               // 注册时间（本程序启动的时间）
};

void inputPerson(time_t regtime); // 接受用户输入并写入Persons.stru
void printPersons();			  // 从读出Persons.stru并显示信息

int main()
{
	time_t startup_time = time(NULL);	// 启动程序的时间
	printf(ctime(&startup_time));

	inputPerson(startup_time); 
	printf("Input finished. Starting to output.\n\n");
	printPersons();

	printf("Press any key to exit.\n");
	getchar(); 
	return 0;
}

/* 接受用户输入并写入Persons.stru */
void inputPerson(time_t regtime)
{
	FILE* outfile;
	char input_buf[BUF_LEN];	// 建立输入缓冲区 
	struct Person person;

	if ((outfile = fopen(".\\Persons.stru", "wb")) == NULL)
	{
		printf("Can't open the file! Exiting...");
		getchar();
		exit(1);
	}

//	while (~scanf("%s", input_buf)) {
	while (1) {
		printf("\nUsername: ");
		fgets(input_buf, BUF_LEN, stdin);
		if(input_buf[0]=='\n') { break; }	// 输入为空时跳出循环
		input_buf[strlen(input_buf)-1] = '\0';	// 将换行符替换为空字符
		strcpy(person.username, input_buf);

		printf("Level: ");
		scanf("%d", &person.level);

		printf("Email: ");
		scanf("%s", input_buf);
		strcpy(person.email, input_buf);

		person.sendtime = (DWORD)time(NULL);
		person.regtime = regtime;   // 注册时间：本程序启动的时间

		if (fwrite(&person, sizeof(struct Person), 1, outfile) != 1) {
			printf("File write error!\n");
		}
		getchar();	// 丢弃最后一个换行符
	}
	fclose(outfile);
}

/* 读出Persons.stru并显示信息 */
void printPersons()
{
	FILE * infile;
	struct Person person;
	int count = 1;

	if ((infile = fopen(".\\Persons.stru", "rb")) == NULL)
	{
		printf("Can't open the file! Exiting...");
		getchar();
		exit(1);
	}
	printf("-----------------------------------------\n");
	while (fread(&person, sizeof(struct Person), 1, infile) == 1) {
		printf("No.%d\n", count++);
		printf("用户名：%s\n", person.username);
		printf("级别：%d\n", person.level);
		printf("电子邮件：%s\n", person.email);
		time_t sendtime = person.sendtime;
		printf("发送时间：%s", ctime(&sendtime));
		printf("注册时间：%s", ctime(&person.regtime));
		printf("-----------------------------------------\n");
	}
}
