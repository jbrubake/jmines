/*--------------------------------------------------------------------*\
| This file is part of jmines
|
| <+Description+>
|
| Copyright 2012, Jeremy Brubaker <jbru362@gmail.com>
|---------------------------------------------------------------------*\
| jmines is free software: you can redistribute it and/or modify       |
| it under the terms of the GNU General Public License as published by |
| the Free Software Foundation, either version 3 of the License, or    |
| (at your option) any later version.                                  |
|                                                                      |
| jmines is distributed in the hope that it will be useful,            |
| but WITHOUT ANY WARRANTY; without even the implied warranty of       |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         |
| GNU General Public License for more details.                         |
|                                                                      |
| You should have received a copy of the GNU General Public License    |
| along with jmines. If not, see <http://www.gnu.org/licenses/>.       |
\*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"
#include "game.h"

static void print_grid (game_data *);
static void print_menu ();

static int get_coordinates (int *x, int *y);
static char print_cell_contents (game_data *, int, int);

/*
 * User input loop
 */
int
game_loop (game_data *data)
{
    char input = '\0';
    int x = 0;
    int y = 0;

    for (;;)
    {
        print_grid (data);
        print_menu ();

        /* TODO: better input validation to prevent crashes */
        scanf (" %c", &input); /* get command */

        switch (input)
        {
            case 'u':
                if (get_coordinates (&x, &y) != 2) break;
                if (uncover_cell (data, x, y) == LOSER)
                    return LOSER;
                break;
            case 'f':
                if (get_coordinates (&x, &y) != 2) break;
                if (flag_cell (data, x, y) == WINNER)
                    return WINNER;
                break;
            case 'g':
                if (get_coordinates (&x, &y) != 2) break;
                (void) guess_cell (data, x, y);
                break;
            case 'U':
                if (get_coordinates (&x, &y) != 2) break;
                (void) unmark_cell (data, x, y);
                break;
            case 'q':
                return SUCCESS; /* Must be a cleaner way to quit */
                break;
            default:
                printf ("%s\n", "Invalid choice!");
                break;
        }

        while (getchar () != '\n') /* Discard rest of line */
            ;
    }
}

/*
 * Read and return x and y coordinates
 */
int
get_coordinates (int *x, int *y)
{
    return scanf (" %d %d", x, y);
}

/*
 * Print user choice menu
 */
void
print_menu ()
{
    printf ("%s", "\n\
u*) Uncover cell\n\
f*) Flag cell as mine\n\
g*) Mark cell as guess\n\
U*) Unmark cell\n\
q ) Quit\n\
\n\
*: cmd x-coord y-coord\n\
\n\
Choice? ");
}

/*
 * Print current state of grid
 */
void
print_grid (game_data *data)
{
    int x = 0;
    int y = 0;
    int i = 0;

    /*
     * Print x-coordinate
     */
    printf ("     ");
    for (i = 1; i <= data->grid.width; i++)
        printf (" %3d ", i);
    printf ("\n");

    /*
     * Print top of box
     */
    printf ("    +-");
    for (i = 1; i <= data->grid.width; i++)
        printf ("%s", "-----");
    printf ("+\n");

    /*
     * Print each row
     */
    for (y = 1; y < data->grid.height+1; y++)
    {
        printf ("%3d | ", y); /* y-coord and left of box */

        /* Print row of cells */
        for (x = 1; x < data->grid.width+1; x++)
            printf ("  %c  ", print_cell_contents (data, x, y));

        printf ("| %d\n", y); /* right of box and y-coord */
    }

    /*
     * Print bottom of box
     */
    printf ("    +-");
    for (i = 1; i <= data->grid.width; i++)
        printf ("-----"); 
    printf ("+\n");

    /*
     * Print x-coordinates
     */
    printf ("     ");
    for (i = 1; i <= data->grid.width; i++)
        printf (" %3d ", i);
    printf ("\n");
}

/*
 * Convert struct game_data cell contents to a printable character and return it
 */
char
print_cell_contents (game_data *data, int x, int y)
{
    char printable = '\0';

    /* First check for markers or if cell is covered */
    if (data->grid.cell[x][y].marker          == FLAG)
        printable = 'F';
    else if (data->grid.cell[x][y].marker     == GUESS)
        printable = 'G';
    else if (data->grid.cell[x][y].is_covered == true)
        printable = '.';
    /* If cell is uncovered and unmarked, print its contents */
    else
    {
        switch (data->grid.cell[x][y].contents)
        {
            case EMPTY:
                printable = ' ';
                break;
            case ONE:
                printable = '1';
                break;
            case TWO:
                printable = '2';
                break;
            case THREE:
                printable = '3';
                break;
            case FOUR:
                printable = '4';
                break;
            case FIVE:
                printable = '5';
                break;
            case SIX:
                printable = '6';
                break;
            case SEVEN:
                printable = '7';
                break;
            case EIGHT:
                printable = '8';
                break;
            case MINE:
                printable = 'X';
                break;
        }
    }

    return printable;
}
