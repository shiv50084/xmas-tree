/*Build: gcc -std=c99 -o xmas3 xmas3.c -lncurses*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>

#define PHI 1.61803

#define CALC_HEIGHT_TRUNK(x) ((x) / (PHI * 2.0))

#define HEADER_ROWS 1
#define FOOTER_ROWS 1

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

int print_header(int maxY, int maxX)
{
    char buf[50];
    int header_width = 0;

    memset(buf, '\0', sizeof buf);
    int char_ret1 = snprintf(buf, sizeof buf, "Max Height: %d", maxY);
    mvaddstr(0, 0, buf);
    header_width += char_ret1;

    memset(buf, '\0', sizeof buf);
    int char_ret2 = snprintf(buf, sizeof buf, "Max Width: %d", maxX);
    mvaddstr(0, ++header_width, buf);
    header_width += char_ret2;

    refresh();
    return 0;
}

int print_footer(int maxY, int height_tree, int height_trunk)
{
    char buf[50];
    int footer_width = 0;

    memset(buf, '\0', sizeof buf);
    int char_ret1 = snprintf(buf, sizeof buf, "Height tree: %d", height_tree);
    mvaddstr(maxY - 1, 0, buf);
    footer_width += char_ret1;

    memset(buf, '\0', sizeof buf);
    int char_ret2 = snprintf(buf, sizeof buf, "Height trunk: %d", height_trunk);
    mvaddstr(maxY - 1, ++footer_width, buf);
    footer_width += char_ret2;

    memset(buf, '\0', sizeof buf);
    int char_ret3 = snprintf(buf, sizeof buf, "Total tree height: %d", height_tree + height_trunk);
    mvaddstr(maxY - 1, ++footer_width, buf);
    footer_width += char_ret3;

    refresh();
    return 0;
}

int print_trunk(int height_tree, int height_trunk, char half)
{
    int width_trunk = 2 * height_trunk - 1;

    if (!half) // full tree
    {
        for (int i = height_tree; i < height_tree + height_trunk; i++)
        {
            for (int j = (height_tree - height_trunk);j < (height_tree - height_trunk) + width_trunk;j++)
            {
                color_str(i+HEADER_ROWS, j, j % COLORS, i % COLORS, "#");
            }
        }
    }
    else // half tree
    {
        for (int i = height_tree; i < height_tree + height_trunk; i++)
        {
            for (int j = 0;j < width_trunk / 2;j++)
            {
                color_str(i+HEADER_ROWS, j, i % COLORS, j % COLORS, "#");
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

                color_str(i+HEADER_ROWS, j, j % COLORS, i % COLORS, "*");

                if (j == i)
                    attroff(A_BOLD | A_BLINK | A_UNDERLINE);
            }
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

                    color_str(i+HEADER_ROWS, j, i % COLORS, j % COLORS, "*");

                    if (j == (height-1) - i ||
                        j == width-1)
                    {
                        attroff(A_BOLD | A_BLINK | A_UNDERLINE);
                    }
                }
            }
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
    int height_trunk = CALC_HEIGHT_TRUNK(height_tree);

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

        // Initialize color pairs.
        short pair_count = 0;
        for(short i = 0;i<COLORS;i++)
            for(short j = 0;j<COLORS;j++)
                init_pair(pair_count++, i, j);
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
    print_header(maxY, maxX);
    print_tree(height_tree, half);
    print_trunk(height_tree, height_trunk, half);
    print_footer(maxY, height_tree, height_trunk);

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
                height_trunk = CALC_HEIGHT_TRUNK(height_tree);

                if (height_tree + height_trunk + HEADER_ROWS > maxY - FOOTER_ROWS)
                {
                    height_tree -= 1;
                    // Recalculate trunk
                    height_trunk = CALC_HEIGHT_TRUNK(height_tree);
                }

                break;
            case KEY_DOWN: // Press Down to decrease height
                height_tree -= 1;
                height_trunk = CALC_HEIGHT_TRUNK(height_tree);

                if (height_tree - height_trunk < 1)
                {
                    height_tree = 1;
                    height_trunk = CALC_HEIGHT_TRUNK(height_tree);
                }

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
        print_header(maxY, maxX);
        print_tree(height_tree, half);
        print_trunk(height_tree, height_trunk, half);
        print_footer(maxY, height_tree, height_trunk);
    }


    delwin(mainwin);
    endwin();
    refresh();

    return 0;
}

