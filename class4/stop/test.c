#include <stdio.h>
#include <stdlib.h>

void test()
{
    exit(1);
}

int main()
{
    printf("hi");
    test();
    return 0;
}
