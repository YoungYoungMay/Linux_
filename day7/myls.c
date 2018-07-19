//"ls"命令的实现
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>

void show_mode(mode_t md) 
{
		char mode[12]="?---------.";
		if((md & S_IFMT)==S_IFSOCK) mode[0]='s';
		if((md & S_IFMT)==S_IFLNK) mode[0]='l';
		if((md & S_IFMT)==S_IFDIR) mode[0]='d';
		if((md & S_IFMT)==S_IFBLK) mode[0]='b';
		if((md & S_IFMT)==S_IFCHR) mode[0]='c';
		if((md & S_IFMT)==S_IFREG) mode[0]='-';
		if((md & S_IFMT)==S_IFIFO) mode[0]='p';

		if(md & S_IRUSR) mode[1]='r';
		if(md & S_IWUSR) mode[2]='w';
		if(md & S_IXUSR) mode[3]='x';

		if(md & S_IRGRP) mode[4]='r';
		if(md & S_IWGRP) mode[5]='w';
		if(md & S_IXGRP) mode[6]='x';

		if(md & S_IROTH) mode[7]='r';
		if(md & S_IWOTH) mode[8]='w';
		if(md & S_IXOTH) mode[9]='x';

		printf("%s ",mode);
}

void show(char *file)
{
        struct stat fbuf;
        lstat(file,&fbuf);
        if(lstat(file,&fbuf)==-1)
        {
                printf("no have this file\n");
                exit(1);
        }
        show_mode(fbuf.st_mode);
        printf("%d ",fbuf.st_nlink);
        struct passwd *u=getpwuid(fbuf.st_uid);
        if(u!=NULL)
        {
                printf("%s ",u->pw_name);
        }
        struct group *g=getgrgid(fbuf.st_gid);
        if(g!=NULL)
       {
                printf("%s ",g->gr_name);
        }
        printf("%d ",fbuf.st_size);

        struct tm *t=localtime(&fbuf.st_mtime);
        if(t!=NULL)
        {
                printf("%04d-%02d-%02d ",t->tm_year+1900,t->tm_mon+1,t->tm_mday);
                printf("%02d:%02d:%02d ",t->tm_hour,t->tm_min,t->tm_sec);
        }

        printf("%s",file);
        if(S_ISLNK(fbuf.st_mode))
        {
                printf("->");
                char yf[256];
                readlink(file,yf,sizeof(yf));
                printf("%S",yf);
        }
}int main(int argc,char *argv[])
{
	//myls filename
        if(argc!=2)
        {
                fprintf(stderr,"please enter:%s,filename\n",argv[0]);
                exit(1);
        }
        struct stat sbuf;
        lstat(argv[1],&sbuf);
        if(lstat(argv[1],&sbuf)==-1)
        {
                printf("not have this file\n");
                return 1;
        }
        if(S_ISDIR(sbuf.st_mode))
        {
                DIR *dp=opendir(argv[1]);
                if(dp==NULL)
                {
                        perror("openfile error\n");
                        return 2;
                }
                struct dirent *d;
               while(d=readdir(dp))
                {
                        if(d->d_name[0]=='.')
                                continue;
                        char file[500];
                        sprintf(file,"%s/%S",argv[1],d->d_name);
                        show(file);
                }
                closedir(dp);
        }
        else
                show(argv[1]);
        printf("\n");
        return 0;
}
