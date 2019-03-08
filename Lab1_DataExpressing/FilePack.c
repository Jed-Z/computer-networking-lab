#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h> 

#define BUF_SIZE 1024
char buf[BUF_SIZE]; 		// 建立全局缓冲区

void packFiles(const char* destfile);
void unpackFiles(const char* destfile);
void validateFilename(char* filename);

int main()
{
	printf("注意：仅接受文件名，不支持绝对路径。\n\n");
	packFiles("FileSet.pak");	// 调用打包函数
    unpackFiles("FileSet.pak");	// 调用解包函数

	printf("Press any key to exit.\n");
	getchar(); 
    return 0;
}

/* 打包 */
void packFiles(const char* destfilename)
{
    FILE *destfile = fopen(destfilename, "wb");
    if (destfile == NULL) {
        printf("[-] Can't open FileSet.pak! Exiting...");
        getchar();
        exit(1);
    }
    char filename[300];	// 用于储存文件名的缓冲区
    int count = 1;		// 文件计数
    
    while(1) { 
    	printf("[+] Name of file %d: ", count);
    	fgets(filename, 300, stdin);
    	if(filename[0]=='\n') { break; }	// 输入为空时跳出循环
    	filename[strlen(filename)-1] = '\0';// 将换行符替换为空字符
    	
        FILE *srcfile = fopen(filename, "rb");
        if (srcfile == NULL)
        {
            printf("[-] Can't open the file! Try again.\n");
            continue;
        }
        count++;

        /* 写入文件名（300个字节） */
        fwrite(filename, 300, 1, destfile);
        
        /* 写入文件大小（__int64） */
        fseek(srcfile, 0, SEEK_END);
        __int64 filesize = ftell(srcfile);
        fseek(srcfile, 0, SEEK_SET);
        fwrite(&filesize, sizeof(__int64), 1, destfile);
        
        /* 写入文件内容 */
        int remain_bytes = 0;
        while((remain_bytes = fread(buf, 1, BUF_SIZE, srcfile)) >= BUF_SIZE) {
            fwrite(buf, 1, BUF_SIZE, destfile);
        }
        fwrite(buf, 1, remain_bytes, destfile);
        fclose(srcfile);
        printf("Succeeded on %s, %I64d byte(s) written.\n", filename, filesize);
    }
    fclose(destfile);	// 打包完成，关闭文件
    printf("%d file(s) have been packed into FileSet.pak.\n", count-1);
    printf("-----------------------------------------\n");
}

/* 解包 */
void unpackFiles(const char* srcfilename)
{
    FILE *srcfile = fopen(srcfilename, "rb");
    if (srcfile == NULL) {
        printf("[-] Can't open FileSet.pak! Exiting...");
        getchar();
        exit(1);
    }
    
    while(1) {
    	/* 读文件名 */ 
        char filename[300];
        if(fread(filename, 300, 1, srcfile) != 1) { break; }
        
        validateFilename(filename);	// 构造有效的（不重名）的文件名 
        
        FILE* destfile = fopen(filename, "wb");
        if (destfile == NULL) {
            printf("[-] Can't open %s! Exiting...", filename);
            getchar();
            exit(1);
        }
        
        /* 读文件大小 */ 
        __int64 filesize;
        fread(&filesize, sizeof(__int64), 1, srcfile);
        
        /* 读文件内容 */ 
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

/* 构造有效的文件名（即为重名文件添加序号） */
void validateFilename(char* filename)
{
	char temp[300];
	char numstr[10];
	int num = 1;
	while(access(filename, 0)==0) {	// 文件已存在（重名）
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
	}
}
