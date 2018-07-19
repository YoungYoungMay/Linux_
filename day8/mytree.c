#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

void print_tree(const char *str,int gap,char shp)//路径、缩进、前置符号
{
	DIR *pdir = opendir(str);//打开路径
	if(pdir == NULL)
		return;
	struct dirent *pd = NULL;
	while((pd = readdir(pdir)) != NULL)
	{
		if(strcmp(pd->d_name,".") == 0 || strcmp(pd->d_name,"..") == 0)
			continue;
		int len = strlen(str) + 1 + strlen(pd->d_name);
		char *path = (char *)malloc(len);
		sprintf(path,"%s/%s",str,pd->d_name);//拼凑目录
		//printf("%s\n",pd->d_name);//打印文件名
		struct stat sbuf;
		lstat(path,&sbuf);
		int i = 0;
		for(i=0; i<gap; i++)
			printf("%c",shp);
		char *p = strrchr(path,'/');
		if(p == NULL)
			printf("%s\n",path);
		else
			printf("%s\n",p+1);
		if(S_ISREG(sbuf.st_mode))
		{
			//print
		}
		else if(S_ISDIR(sbuf.st_mode))
			print_tree(path,gap+4,shp);
	}
}

int main()
{
	print_tree(".",0,'-');
}
