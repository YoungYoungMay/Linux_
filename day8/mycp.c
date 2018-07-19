#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

void copy_file(const char *src,const char *dst,mode_t md_src)
{
	struct stat sbuf_dst;
	lstat(dst,&sbuf_dst);
	sbuf_dst.st_mode = md_src;
	FILE *fp_src = fopen(argv[2],"r");
    if(fp_dst == NULL || fp_src == NULL)
    {
		fprintf(stderr,"open file error\n");
        exit(1);
    }
	FILE *fp_src = fopen(argv[2],"w");
	if(fp_dst == NULL)
	{
		fprintf(stderr,"error of open %s\n",dst);
		exit(1);
	}
    char buf[4096];
    int r;
    while((r = fread(buf,1,sizeof(buf),fp_src)) > 0)
		fwrite(buf,1,r,fp_dst);
    fclose(fp_dst);
    fclose(fp_src);
}

void cp_dir(const char *src,const char *dst)
{
	DIR *pdir = opendir(src);
	struct dirent *pd = NULL;
	if(pdir == NULL)
	{
		fprintf(stderr,"the open of %s error\n",src);
		exit(1);
	}
	struct stat sbuf_src;
	while((pd = readdir(pdir)) != NULL)
	{
		if(strcmp(pd->pd_name,".") == 0 || strcmp(pd->pd_name,"..") == 0)
			continue;
		int len = strlen(src) + 2 + strlen(dst);
		char * src_name = (char *) malloc(len);
		if(src_name == NULL)
		{
			fprintf(stderr,"malloc error\n");
			exit(1);
		}
		memset(src_name,0X00,len);
		sprintf(src_name,"%s/%s",src,pd->pd_name);
		lstat(src_name,&sbuf_src);
		if(S_ISREG(sbuf_src.st_mode))
		{
			int len1 = strlen(pd->pd_name)+strlen(dst)+2;
			char *tmp1 = (char *)malloc(len1);
			if(tmp1 == NULL)
			{
				fprintf(stderr,"error malloc\n");
				exit(1);
			}
			sprintf(tmp1,"%s/%s",dst,pd->pd_name);
			copy_file(src_name,tmp1,sbuf_src.st_mode);
			free(tmp1);
		}
		else if(S_ISDIR(sbuf_src.st_mode))
		{
			int len2 = 	strlen(pd->pd_name)+strlen(dst)+2;
			char *tmp2 = (char *)malloc(len2);
			if(tmp2 == NULL)
			{
				fprintf(stderr,"error malloc\n"):
				exit(1);
			}	
			sprintf(tmp2,"%s/%s",dst,pd->pd_name);
			mkdir(tmp2,sbuf_src,st_mode);
			copy_dir(src_name,tmp2);
			free(tmp2);
		}
		free(src_name);
	}
	closedir(src_name);
}

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		fprintf(stderr,"%s scr dst\n",argv[0]);
		exit(1);
	}
	struct stat sbuf_src;
	if(lstat(argv[1],&sbuf_src) == -1)
	{
		fprintf(stderr,"%s No exist\n",argv[1]);
		exit(1);
	}
	mozzde_t old = umask(0);
	if(S_ISREG(sbuf_src.st_mode))
	{
		struct stat sbuf_dst;
		if(lstat(argv[2],&sbuf_dst) == -1)
		{
			copy_file(argv[1],argv[2],sbuf_src.st_mode);
		}
		else if(S_ISREG(sbuf_dst.st_mode))
		{
			printf("the aim dirctry existed,cover?\n");
			char c = 'n';
			scanf("%[yYnN]",&c);
			if(c == 'y' || c == 'Y')
				copy_file(argv[1],argv[2],sbuf_src.st_mode);
		}
		else if(S_ISDIR(sbuf_dst.st_mode)
		{
			int len = strlen(argv[1]) + 1 + strlen(argv[2]) + 1;
			char *tmp = (char *)malloc(len);
			if(tmp == NULL)
			{
				fprintf(stderr,"error of malloc\n");
				exit(1);
			}
			memset(tmp,0X00,len);
			sprintf(tmp,"%s / %s",argv[2],argv[1]);
			copy_file(argv[1],tmp,sbuf_src.st_mode);
			free(tmp);
		}
	}
	else if(S_ISDIR(sbuf_src.st_mode)
	{
		//struct stat sbuf_dst;
		if(lstat(argv[2],&sbuf_dst) == -1)
		{
			mkdir(argv[2],sbuf_src.st_mode);
			cp_dir(argv[1],argv[2]);
		}
		else if(S_ISREG(sbuf_dst.st_mode))
		{
			fprintf(stderr,"can't copy directoy to file\n");
			exit(1);
		}
		else if(S_ISDIR(sbuf_dst.st_mode))
		{
			int len = strlen(argv[1]) + strlen(argv[2]) + 2;
			char *tmp = (char *)malloc(len);
			if(tmp == NULL)
			{
				printf("error malloc \n");
				exit(1);
			}
			memset(tmp,0X00,len);
			sprintf(tmp,"%s / %s",argv[2],argv[1]);
			cp_dir(argv[1],tmp);
			free(tmp);
		}
		else
		{
			fprintf(stderr,"not copy direction for file\n");
			exit(1);
		}
	}
	umask(old);
}
