/*====================================================================*\
| jmines-1.1.0
|
| Started on:  03/25/10 13:15:42
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
|======================================================================/
| <+Description+>
\*====================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <locale.h>
#include <getopt.h>
#include <string.h>

#include "jmines.h"
#include "cli.h"

int
main (int argc, char **argv)
{
    /* Default is 10 mines in a 10x10 grid */
    int num_mines = 10;
    int height    = 10;
    int width     = 10;

    /* Game data structure */
    game_data *jmines = NULL;

    /* Set locale according to environment */
    (void) setlocale (LC_ALL, "");

    /* Process options */
    if (process_options (argc, argv, &num_mines, &height, &width) == 1)
        goto exit_error;

    /* Initialize interface */

    /* Initialize game data */
    if (!(jmines = init_game (num_mines, height, width)))
    {
        printf ("%s\n", "Could not allocate game_data");
        goto exit_error;
    }

    /* Game loop */
    switch (game_loop (jmines))
    {
        case WINNER:
            printf ("%s\n", "YOU WIN!!!");
            break;
        case LOSER:
            printf ("%s\n", "YOU LOST!!! TRY AGAIN!");
            break;
    }

    exit (0);

exit_error:
    exit (errno);
}

/*
 * Rudimentary command line option processing
 */
int
process_options (int argc, char **argv, int *num_mines, int *h, int *w)
{
    int opt = 0;

    while ((opt = getopt (argc, argv, "hvn:x:y:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                print_help ();
                return 1;
                break;
            case 'v':
                print_version ();
                return 1;
                break;
            case 'n':
                *num_mines = atoi (optarg);
                if (*num_mines == 0) *num_mines = 10;
                break;
            case 'x':
                *w = atoi (optarg);
                if (*w == 0) *w = 10;
            case 'y':
                *h = atoi (optarg);
                if (*h == 0) *h = 10;
            default:
                print_help ();
                return 1;
                break;
        }
    }
    return 0;
}

void
print_help ()
{
    fprintf (stderr, "%s\n",
"Usage: jmines [OPTION]...\n"
"Play minesweeper on the command line.\n"
""
" -n <num>\tset number of mines to <num>\n"
" -x <width>\tset grid with to <width>\n"
" -y <height>\tset grid height to <height>\n"
" -h\t\tdisplay this help end exit\n"
" -v\t\toutput version information and exit");
}

void
print_version ()
{
    fprintf (stderr, "%s\n",
"jmines 1.0.0\n"
"Copyright (C) 2012 Jeremy Brubaker\n"
"License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n"
"\n"
"Written by Jeremy Brubaker.");
}
