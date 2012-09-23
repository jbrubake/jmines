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

        input = getc (stdin);        /* Only need the first character */
        while (getc (stdin) != '\n') /* Discard rest of line */
            ;

        switch (input)
        {
            case '1':
                get_coordinates (&x, &y);
                if (uncover_cell (data, x, y) == 1)
                    return 1;
                break;
            case '2':
                get_coordinates (&x, &y);
                flag_cell (data, x, y);
                break;
            case '3':
                get_coordinates (&x, &y);
                guess_cell (data, x, y);
                break;
            case '4':
                get_coordinates (&x, &y);
                unmark_cell (data, x, y);
                break;
            case '5':
                return 0;
                break;
            default:
                printf ("%s\n", "Invalid choice!");
                break;
        }
    }
}

void
get_coordinates (int *x, int *y)
{
    char input[80] = "";

    printf ("%s", "Coordinates (x, y)? ");

    /* TODO: Make this safer!! */
    fgets (input, 80, stdin);
    *x = atoi (strtok (input, ", "));
    *y = atoi (strtok (NULL, " \n"));
}

void
print_menu ()
{
    printf ("%s", "\n\
1) Uncover cell\n\
2) Flag cell as mine\n\
3) Mark cell as guess\n\
4) Unmark cell\n\
5) Quit\n\
\n\
Choice? ");
}

void
print_grid (game_data *data)
{
    int x = 0;
    int y = 0;
    int i = 0;

    printf ("%s", "  ");

    for (i = 1; i <= data->grid.width; i++)
    {
        printf ("%c", ' ');
        (i < 100) && printf ("%c", ' ');
        (i < 10) && printf ("%c", ' ');
        printf ("%d ", i);
    }

    printf ("%s", "\n");

    for (y = 1; y < data->grid.height+1; y++)
    {
        (y < 100) && printf ("%c", ' ');
        (y < 10) && printf ("%c", ' ');
        printf ("%d", y);
        for (x = 1; x < data->grid.width+1; x++)
        {
            printf ("  %c  ", print_cell_contents (data, x, y));
        }
        printf ("%s", "\n");
    }
}

char
print_cell_contents (game_data *data, int x, int y)
{
    char printable = '\0';

    if (data->grid.cell[x][y].marker == FLAG)
        printable = 'F';
    else if (data->grid.cell[x][y].marker == GUESS)
        printable = 'G';
    else if (data->grid.cell[x][y].is_covered == TRUE)
        printable = '.';
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
