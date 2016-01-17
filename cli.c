/*
 * This file is part of jmines
 *
 * Started on:  03/25/10 13:15:42
 *
 * Copyright 2010, 2012 Jeremy Brubaker <jbru362@gmail.com>
 *
 * jmines is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * jmines is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with jmines. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file */

#include <stdlib.h>
#include <stdio.h>

#include "grid.h"
#include "options.h"
#include "debug.h"
#include "errmsg.h"

/** @{ @name Main Loop Status Flags
 *
 * #LOST_GAME is equal to -1 to prevent a conflict with
 * error return codes in the program as it will be used
 * as the program exit code
 */
#define PLAYING_GAME  1 /**< @brief continue main loop */
#define LOST_GAME    -1 /**< @brief game has been lost */
#define WON_GAME      0 /**< @brief game has been won */
/** @} */

/**
 * Return a character corresponding to what the cell should look like
 *
 * @param y, x coordinates of cell
 *
 * @return character representing the cell
 *
 * @note Exit on error
 */
static char
cell_char (Grid *grid, int y, int x)
{
    int c;

    c = get_cell_visible (grid, y, x);
    if (c == -1)
        err_exit ("Invalid coordinates (%d,%d) in cell_char()", y, x);

    switch (c)
    {
        case EMPTY:
            return ' ';
        case ONE:
            return '1';
        case TWO:
            return '2';
        case THREE:
            return '3';
        case FOUR:
            return '4';
        case FIVE:
            return '5';
        case SIX:
            return '6';
        case SEVEN:
            return '7';
        case EIGHT:
            return '8';
        case MINE:
            return 'x';
        case NONE:
            return '.';
        case FLAG:
            return 'F';
        case GUESS:
            return '?';
        default:
            break;
    }
    err_exit ("Fell through cell_char() switch");
    return 0; /* should *never* get here */
}

#ifdef DEBUG
/**
 * Return the character corresponding to the cell contents
 *
 * @param y, x coordinates of cell
 *
 * @return character representing the cell
 */
static char
cell_contents_char (Grid *grid, int y, int x)
{
    int c;

    c = get_cell_contents (grid, y, x);
    if (c == -1)
        err_exit ("Invalid coordinates (%d,%d) in cell_contents_char()", y, x);

    switch (c)
    {
        case EMPTY:
            return ' ';
        case ONE:
            return '1';
        case TWO:
            return '2';
        case THREE:
            return '3';
        case FOUR:
            return '4';
        case FIVE:
            return '5';
        case SIX:
            return '6';
        case SEVEN:
            return '7';
        case EIGHT:
            return '8';
        case MINE:
            return 'x';
        default:
            TRACE ((DBG_STD, "Unkown cell contents (%d) at %d,%d\n",
                        c, x+1, y+1));
            return '#';
            break;
    }
    err_exit ("Fell through cell_contents_char() switch");
    return 0; /* should *never* get here */
}

/**
 * @brief Write the grid ocntents to stdout
 */
static void
draw_grid_contents (Grid *grid)
{
    int y, x, i;
    int height, width;

    get_size (grid, &height, &width);

    printf ("     ");
    for (x = 1; x <= width; x++)
        printf ("%4d ", x);
    printf ("\n");

    printf ("    +-");
    for (i = 0; i < width; i++)
        printf ("-----");
    printf ("+\n");

    for (y = 0; y < height; y++)
    {
        printf ("%3d | ", y+1);

        for (x = 0; x < width; x++)
            printf ("  %c  ", cell_contents_char (grid, y, x));

        printf ("| %d\n", y+1);

    }

    printf ("    +-");
    for (i = 0; i < width; i++)
        printf ("-----");
    printf ("+\n");

    printf ("     ");
    for (x = 1; x <= width; x++)
        printf ("%4d ", x);
    printf ("\n");
}
#endif /* DEBUG */

/**
 * @brief Write the grid to stdout
 */
/* FIXME: This is *super* ugly */
static void
draw_grid (Grid *grid)
{
    int y, x, i;
    int height, width;

    get_size (grid, &height, &width);

    printf ("     ");
    for (x = 1; x <= width; x++)
        printf ("%4d ", x);
    printf ("\n");

    printf ("    +-");
    for (i = 0; i < width; i++)
        printf ("-----");
    printf ("+\n");

    for (y = 0; y < height; y++)
    {
        printf ("%3d | ", y+1);

        for (x = 0; x < width; x++)
            printf ("  %c  ", cell_char (grid, y, x));

        printf ("| %d\n", y+1);

    }

    printf ("    +-");
    for (i = 0; i < width; i++)
        printf ("-----");
    printf ("+\n");

    printf ("     ");
    for (x = 1; x <= width; x++)
        printf ("%4d ", x);
    printf ("\n");
}

/**
 * @brief Print how many mines are marked and
 * total mines in grid to stdout
 */
static inline void
print_counter (Grid *grid)
{
    printf ("\n<<%d of %d mines marked>>\n\n", 
            get_nflags (grid),
            get_nmines (grid));
}

/**
 * @brief Print menu choices to stdout
 */
static inline void
print_menu (void)
{
    puts ("r) Reveal*");
    puts ("f) Flag*");
    puts ("g) Guess*");
    puts ("u) Unmark*");
#ifdef DEBUG
    puts ("x) Print contents");
#endif
    puts ("Q) Quit");
    puts ("(*: needs coordinates)");
    puts ("");
    printf ("Choice? ");
}

/**
 * @brief Get next command from user
 */
static char
get_command (int *y, int *x)
{
    char cmd;

    /* simple sanity check */
    if ((y == 0) || (x == NULL))
        err_exit ("NULL pointer passed to get_command");

    scanf (" %c", &cmd); 

    switch (cmd)
    {
        case 'Q':
            break;
        case 'r':
        case 'f':
        case 'g':
        case 'u':
            if (scanf (" %d %d", x, y) != 2)
                cmd = -1;
            (*x)--; (*y)--; /* convert user coordinates to game coordinates */
            TRACE ((DBG_STD, "actual coordinates are (x=%d,y=%d)\n", *x, *y));
            break;
#ifdef DEBUG
        case 'x':
            break;
#endif
        default:
            cmd = -1;
            break;
    }
    return cmd;
}

/**
 * Main game loop
 *
 * @param opt player options
 *
 * @return #WON_GAME if game is won and #LOST_GAME if game is lost
 *
 * @note exits if player requests to quit
 */
int
main_loop (Options *opt)
{
    Grid *grid;
    int y, x;
    int cmd;
    int status = PLAYING_GAME;

    if ((grid = new_grid (opt->height, opt->width, opt->nmines)) == NULL)
        err_sys_exit ("Couldn't create new grid");

    for (;;)
    {
        draw_grid (grid);
        print_counter (grid);
        print_menu ();

        cmd = get_command (&y, &x);
        switch (cmd)
        {
            case 'r':
                if (cell_reveal (grid, y, x) == -1)
                    status = LOST_GAME;
                break;
            case 'f':
                if (cell_flag (grid, y, x) == 0)
                    status = WON_GAME;
                break;
            case 'g':
                cell_guess (grid, y, x);
                break;
            case 'u':
                cell_unmark (grid, y, x);
                break;
            case 'Q':
                puts ("Bye!");
                exit (0);
#ifdef DEBUG
            case 'x':
                draw_grid_contents (grid);
                break;
#endif
            case -1:
                puts ("Invalid command!");
                break;
            default:
                break;
        }

        if (status != PLAYING_GAME) break;
    }

    if (status == WON_GAME)
    {
        puts ("");
        puts ("You won!!! Thanks for playing.");
        puts ("");
    }
    else if (status == LOST_GAME)
    {
        puts ("");
        puts ("You lost!!! Try again.");
        puts ("");

    }

    return status;
}
