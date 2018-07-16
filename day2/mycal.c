#include <stdio.h>
#include <stdlib.h>

int months[2][13] = {{0,31,28,31,30,31,30,31,31,30,31,30,31},
				  {0,31,29,31,30,31,30,31,31,30,31,30,31}};

int isleap(int year)
{
	return ((year%4 == 0 && year%100 != 0) || year%400 == 0);
}

int main()
{
	int year = 0;
	int month = 0;
	int base_year = 2000;//将其实日期设置为2000.1.1
	int base_week = 6;//起始日期为星期6
	printf("year/month:");
	scanf("%d/%d",&year,&month);
	int i = 0;
	int total_days = 0;//用于统计输入日期与起始日期间的天数
	for(i=base_year;i<year;i++)
	{
		total_days += 365 + isleap(i);
	}
	int year_first = (total_days + base_week) % 7;//计算当前年的1.1是星期几
	printf("year_first = %d\n",year_first);
	//printf("%d-%d\n",year,month);
	total_days = 0;
	for(i=1;i<month;i++)
	{
		total_days += months[isleap(year)][i];
	}
	int month_first = (total_days + year_first) % 7;
	printf("month_first = %d\n",month_first);

	printf("sun mon tue wed thr fri sat\n");
	for(i=1;i<month_first;i++)
		printf("     ");
	for(i=1;i<months[isleap(year)][month];i++)
	{
		printf("%3d ",i);
		if((i+month_first) % 7 == 0)
			printf("\n");
	}
	printf("\n");
}
