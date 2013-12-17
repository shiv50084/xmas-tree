/*Build: gcc -std=c99 -o xmas3 xmas3.c*/
#include <stdlib.h>
#include <stdio.h>

int print_tree(int length)
{
    int i = 0, j = 0;

    for (i = 0;i<length;i++)
    {
        for (j = 0;j<=i;j++)
        {
            printf("*");
        }
        printf("\n");
    }

    return 0;
}

int main(int argc, char*argv[])
{
    if (argc != 2)
    {
        printf("USAGE: %s [length]\n", argv[0]);
        exit(-1);
    }

    int length = atoi(argv[1]);

    print_tree(length);

    return 0;
}

