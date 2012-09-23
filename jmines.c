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

#include <string.h>

#include "jmines.h"
#include "cli.h"

int
main (int argc, char **argv)
{
    int num_mines = 10;
    int height    = 10;
    int width     = 10;

    game_data *jmines = NULL;

    /* Set locale according to environment */
    (void) setlocale (LC_ALL, "");

    /* Process options */
    /* XXX: Testing code only */
    if (argc >= 3)
    {
        num_mines = atoi (argv[1]);
        height    = atoi (argv[2]);
        width     = atoi (argv[3]);
    }

    /* Initialize interface */

    /* Initialize game data */
    jmines = init_game (num_mines, height, width);
    if (!jmines)
        goto init_game_fail;

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

init_game_fail:
    printf ("%s\n", "Could not allocate game_data");

    exit (errno);
}
