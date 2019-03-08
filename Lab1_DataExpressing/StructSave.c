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
	char username[USER_NAME_LEN]; // Ա����
	int level;					  // ���ʼ���
	char email[EMAIL_LEN];		  // Email��ַ
	DWORD sendtime;               // ����ʱ�䣨��дÿ���û���Ϣ��ʱ�䣩
	time_t regtime;               // ע��ʱ�䣨������������ʱ�䣩
};

void inputPerson(time_t regtime); // �����û����벢д��Persons.stru
void printPersons();			  // �Ӷ���Persons.stru����ʾ��Ϣ

int main()
{
	time_t startup_time = time(NULL);	// ���������ʱ��
	printf(ctime(&startup_time));

	inputPerson(startup_time); 
	printf("Input finished. Starting to output.\n\n");
	printPersons();

	printf("Press any key to exit.\n");
	getchar(); 
	return 0;
}

/* �����û����벢д��Persons.stru */
void inputPerson(time_t regtime)
{
	FILE* outfile;
	char input_buf[BUF_LEN];	// �������뻺���� 
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
		if(input_buf[0]=='\n') { break; }	// ����Ϊ��ʱ����ѭ��
		input_buf[strlen(input_buf)-1] = '\0';	// �����з��滻Ϊ���ַ�
		strcpy(person.username, input_buf);

		printf("Level: ");
		scanf("%d", &person.level);

		printf("Email: ");
		scanf("%s", input_buf);
		strcpy(person.email, input_buf);

		person.sendtime = (DWORD)time(NULL);
		person.regtime = regtime;   // ע��ʱ�䣺������������ʱ��

		if (fwrite(&person, sizeof(struct Person), 1, outfile) != 1) {
			printf("File write error!\n");
		}
		getchar();	// �������һ�����з�
	}
	fclose(outfile);
}

/* ����Persons.stru����ʾ��Ϣ */
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
		printf("�û�����%s\n", person.username);
		printf("����%d\n", person.level);
		printf("�����ʼ���%s\n", person.email);
		time_t sendtime = person.sendtime;
		printf("����ʱ�䣺%s", ctime(&sendtime));
		printf("ע��ʱ�䣺%s", ctime(&person.regtime));
		printf("-----------------------------------------\n");
	}
}
