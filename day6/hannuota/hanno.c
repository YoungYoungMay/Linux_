#include <stdio.h>
#include "hanno.h"

void hanno(int n,char a,char b,char c)
{
	if(n == 1)
		printf("%c -> %c\n",a,c);
	else
	{
		hanno(n-1,a,c,b);
		printf("%c -> %c\n",a,c);
		hanno(n-1,b,a,c);
	}
}
