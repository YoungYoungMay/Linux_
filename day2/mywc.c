//计算一个文件的行数、单词数、字符数
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc,char *argv[])
{
	//a.out filename
	if(argc != 2)
	{
		fprintf(stderr,"%s filename\n",argv[0]);
		exit(1);
	}
	
	FILE *fp = fopen(argv[1],"r");//以只读方式打开文件
	if(fp == NULL)
	{
		fprintf(stderr,"open file error\n");
		exit(1);
	}
	int line = 0;//行数
	int word = 0;//单词数
	int chr = 0;//字符数
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);
	rewind(fp);//回到文件的开始
	char *buf = (char *)malloc(len+1);
	fread(buf,len,1,fp);//将文件fp中的内容拷贝到buf里
	fclose(fp);

	int i =0;
	int status = 0;
	for(i=0;buf[i] != 0;i++)
	{
		chr++;
		if(buf[i] == '\n')
			line++;
		if(!isspace(buf[i]) & status == 0)//用于测试是否为空格、制表符、换行符是就返回true,否则返回NULL
		{
			word++;
			status = 1;
		}
		else if(isspace(buf[i]))
			status = 0;
	}
	printf("line = %d,word = %d,chr = %d\n",line,word,chr);
	return 0;
}	
