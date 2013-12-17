/*Build: gcc -std=c99 -o xmas3 xmas3.c*/
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char*argv[])
{
    if (argc != 2)
    {
        printf("USAGE: %s [length]\n", argv[0]);
        exit(-1);
    }

    int length = atoi(argv[1]);

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

