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

int color_str(int y, int x, short fg_color, short bg_color, const char * str)
{
    short i;
    // Search all the pair of colors
    // to match with the given one.
    // Then apply the specific pair.
    // Naive way
    for (i = 1;i < COLOR_PAIRS;i++)
    {
        short f, b;
        pair_content(i, &f, &b);
        if (f == fg_color && b == bg_color)
            break;
    }

    attron(COLOR_PAIR(i));

    mvaddstr(y,x,str);

    attroff(COLOR_PAIR(i));
    return 0;
}

int print_trunk(int height_tree, int height_trunk, char half)
{
    if (!half) // full tree
    {
        for (int i = height_tree; i<height_tree+height_trunk;i++)
        {
            for (int j = (height_tree-height_trunk);j < (height_tree-height_trunk) + 2*(height_trunk-1)+1;j++)
            {
                mvaddstr(i, j, "*");
            }
        }
    }

    refresh();
    return 0;
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
                if (j == i)
                    attron(A_BOLD | A_BLINK | A_UNDERLINE);

                mvaddstr(i, j, "*");

                if (j == i)
                    attroff(A_BOLD | A_BLINK | A_UNDERLINE);
            }
            mvaddstr(i, j, "\n");
        }
    }
    else if (!half) // full tree
    {
        for (i = 0;i < height;i++)
        {
            int width = i + height;
            for (j = 0;j < width;j++)
            {
                if (j >= (height-1) - i)
                {
                    if (j == (height-1) - i ||
                        j == width-1)
                    {
                        attron(A_BOLD | A_BLINK | A_UNDERLINE);
                    }

                    mvaddstr(i, j, "*");

                    if (j == (height-1) - i ||
                        j == width-1)
                    {
                        attroff(A_BOLD | A_BLINK | A_UNDERLINE);
                    }
                }
            }
            mvaddstr(i, j, "\n");
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

    int height_tree = atoi(argv[1]);
    int height_trunk = height_tree / 5;

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
    if (!has_colors())
    {
        delwin(mainwin);
        endwin();
        fprintf(stderr,"Your terminal does not support color\n");
        exit(-1);
    }
    else
    {
        // Initialize colors
        start_color();

        // Assign terminal default foreground/background colors to color number -1
        use_default_colors();

        init_pair(1, COLOR_RED,     COLOR_BLACK);
        init_pair(2, COLOR_GREEN,   COLOR_BLACK);
        init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(4, COLOR_BLUE,    COLOR_BLACK);
        init_pair(5, COLOR_CYAN,    COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE,   COLOR_BLACK);
        init_pair(8, COLOR_RED,     COLOR_WHITE);
        init_pair(9, COLOR_GREEN,   COLOR_WHITE);
    }


    clear();

    // Turn off key echoing
    noecho();
    // Line buffering disabled
    cbreak();
    // This determines the visibility of the cursor.
    curs_set(FALSE);
    // Tell curses not to do NL->CR/NL on output
    nonl();
    // Enable the keypad for non-char keys
    keypad(mainwin, TRUE);


    // Print tree and then wait for a key
    print_tree(height_tree, half);

    // Loop until press q
    while ((ch = getch()) != 'q')
    {
        switch(ch)
        {
            case '0': // Set foreground/background colors to default
            case '1': // Set foreground/background colors according to init_pairs
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                attrset(COLOR_PAIR(ch - '0'));
                break;
            case KEY_UP: // Press Up to increase height
                height_tree += 1;
                height_trunk = height_tree / 5;

                if (height_tree+height_trunk > maxY)
                    height_tree = maxY - height_trunk;

                break;
            case KEY_DOWN: // Press Down to decrease height
                height_tree -= 1;
                height_trunk = height_tree / 5;

                if (height_tree-height_trunk < 1)
                    height_tree = 1;

                break;
            case KEY_RIGHT: // Press right key reverse foreground/background colour.
                attron(A_REVERSE | A_BLINK);
                break;
            case KEY_LEFT: // Press left key return to default foreground/background colour.
                attroff(A_REVERSE | A_BLINK);
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
        print_tree(height_tree, half);
        print_trunk(height_tree, height_trunk, half);
    }


    delwin(mainwin);
    endwin();
    refresh();

    return 0;
}

