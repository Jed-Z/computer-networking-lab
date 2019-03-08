#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h> 

#define BUF_SIZE 1024
char buf[BUF_SIZE]; 		// ����ȫ�ֻ�����

void packFiles(const char* destfile);
void unpackFiles(const char* destfile);
void validateFilename(char* filename);

int main()
{
	printf("ע�⣺�������ļ�������֧�־���·����\n\n");
	packFiles("FileSet.pak");	// ���ô������
    unpackFiles("FileSet.pak");	// ���ý������

	printf("Press any key to exit.\n");
	getchar(); 
    return 0;
}

/* ��� */
void packFiles(const char* destfilename)
{
    FILE *destfile = fopen(destfilename, "wb");
    if (destfile == NULL) {
        printf("[-] Can't open FileSet.pak! Exiting...");
        getchar();
        exit(1);
    }
    char filename[300];	// ���ڴ����ļ����Ļ�����
    int count = 1;		// �ļ�����
    
    while(1) { 
    	printf("[+] Name of file %d: ", count);
    	fgets(filename, 300, stdin);
    	if(filename[0]=='\n') { break; }	// ����Ϊ��ʱ����ѭ��
    	filename[strlen(filename)-1] = '\0';// �����з��滻Ϊ���ַ�
    	
        FILE *srcfile = fopen(filename, "rb");
        if (srcfile == NULL)
        {
            printf("[-] Can't open the file! Try again.\n");
            continue;
        }
        count++;

        /* д���ļ�����300���ֽڣ� */
        fwrite(filename, 300, 1, destfile);
        
        /* д���ļ���С��__int64�� */
        fseek(srcfile, 0, SEEK_END);
        __int64 filesize = ftell(srcfile);
        fseek(srcfile, 0, SEEK_SET);
        fwrite(&filesize, sizeof(__int64), 1, destfile);
        
        /* д���ļ����� */
        int remain_bytes = 0;
        while((remain_bytes = fread(buf, 1, BUF_SIZE, srcfile)) >= BUF_SIZE) {
            fwrite(buf, 1, BUF_SIZE, destfile);
        }
        fwrite(buf, 1, remain_bytes, destfile);
        fclose(srcfile);
        printf("Succeeded on %s, %I64d byte(s) written.\n", filename, filesize);
    }
    fclose(destfile);	// �����ɣ��ر��ļ�
    printf("%d file(s) have been packed into FileSet.pak.\n", count-1);
    printf("-----------------------------------------\n");
}

/* ��� */
void unpackFiles(const char* srcfilename)
{
    FILE *srcfile = fopen(srcfilename, "rb");
    if (srcfile == NULL) {
        printf("[-] Can't open FileSet.pak! Exiting...");
        getchar();
        exit(1);
    }
    
    while(1) {
    	/* ���ļ��� */ 
        char filename[300];
        if(fread(filename, 300, 1, srcfile) != 1) { break; }
        
        validateFilename(filename);	// ������Ч�ģ������������ļ��� 
        
        FILE* destfile = fopen(filename, "wb");
        if (destfile == NULL) {
            printf("[-] Can't open %s! Exiting...", filename);
            getchar();
            exit(1);
        }
        
        /* ���ļ���С */ 
        __int64 filesize;
        fread(&filesize, sizeof(__int64), 1, srcfile);
        
        /* ���ļ����� */ 
        int currentsize = 0;
        while(currentsize < filesize) {
            int writesize = (BUF_SIZE>filesize-currentsize) ? filesize-currentsize : BUF_SIZE;
            fread(buf, 1, writesize, srcfile);
            currentsize += writesize;
            fwrite(buf, 1, writesize, destfile);
        }
        fclose(destfile);
        printf("%s created, sized %I64d byte(s).\n", filename, filesize);
    }
    printf("All finished!\n");
}

/* ������Ч���ļ�������Ϊ�����ļ������ţ� */
void validateFilename(char* filename)
{
	char temp[300];
	char numstr[10];
	int num = 1;
	while(access(filename, 0)==0) {	// �ļ��Ѵ��ڣ�������
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
	}
}
