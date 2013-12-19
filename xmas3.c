/*Build: gcc -std=c99 -o xmas3 xmas3.c -lncurses*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>

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
                mvprintw(i, j, "*");
            }
            mvprintw(i, j, "\n");
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
                    mvprintw(i, j, " ");
                else
                    mvprintw(i, j, "*");
            }
            mvprintw(i, j, "\n");
        }
    }

    refresh();
    return 0;
}

int main(int argc, char *argv[])
{
    WINDOW *mainwin;
    int ch;
    int maxX = 0, maxY = 0;

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

    mainwin = initscr();
    if (mainwin == NULL)
    {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(-1);
    }

    // Get the maximum size of the screen
    getmaxyx(mainwin, maxY, maxX);

    // Check if colors are supported
    if (has_colors() == FALSE)
    {
        delwin(mainwin);
        endwin();
        fprintf(stderr,"Your terminal does not support color\n");
        exit(-1);
    }

    clear();
    // Initialize colors
    start_color();
    // Turn off key echoing
    noecho();
    // Line buffering disabled
    cbreak();
    // Enable the keypad for non-char keys
    keypad(mainwin, TRUE);

    // Print tree and then wait for a key
    print_tree(height, half);

    // Loop until press q
    while ((ch = getch()) != 'q')
    {
        switch(ch)
        {
            case KEY_UP: // Press Up to increase height
                height += 1;

                if (height > maxY)
                    height = maxY;

                break;
            case KEY_DOWN: // Press Down to decrease height
                height -= 1;

                if (height < 1)
                    height = 1;

                break;
            case 'f': // Press f or F for full tree
            case 'F':
                half = 0;
                break;
            case 'h': // Press h or H for half tree
            case 'H':
                half = 1;
                break;
        }

        erase();
        print_tree(height, half);
    }


    delwin(mainwin);
    endwin();
    refresh();

    return 0;
}

