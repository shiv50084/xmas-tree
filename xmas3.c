#include <stdlib.h>
#include <stdio.h>

int main(int argc, char*argv[])
{
    int length = 8;

    for (int i = 0;i<length;i++)
    {
        for (int j = 0;j<=i;j++)
        {
            printf("*");
        }
        printf("\n");
    }
    return 0;
}

