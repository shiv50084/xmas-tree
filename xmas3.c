/*Build: gcc -std=c99 -o xmas3 xmas3.c*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage(char *argv0)
{
    printf("USAGE: %s [height] [half|full]\n", argv0);
    exit(-1);
}

int print_tree(int height, char half)
{
    int i = 0, j = 0;

    if (half) // Half tree
    {
        for (i = 0;i < height;i++)
        {
            for (j = 0;j <= i;j++)
            {
                printf("*");
            }
            printf("\n");
        }
    }
    else if (!half) // full tree
    {
        for (i = 0;i < height;i++)
        {
            int width = i + height;
            for (j = 0;j < width;j++)
            {
                if (j < (height-1) - i)
                    printf(" ");
                else
                    printf("*");
            }
            printf("\n");
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
        usage(argv[0]);

    int height = atoi(argv[1]);

    char half = -1;
    if (!strncmp("half", argv[2], sizeof 4))
        half = 1;
    else if(!strncmp("full", argv[2], sizeof 4))
        half = 0;
    else
        usage(argv[0]);

    print_tree(height, half);

    return 0;
}

