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

#include <stdlib.h>
#include <time.h>

#include "game.h"

/*
 * Generate a random number from 0 to limit (inclusiv) */
int
rand_limit (int limit)
{
    int divisor = RAND_MAX / (limit + 1);
    int retval = 0;

    do {
        retval = rand () / divisor;
    } while (retval > limit);

    return retval;
}

/*
 * Initialize a game data structure and return a pointer
 */
game_data *
init_game (int num_mines, int height, int width)
{
    int i = 0; /* Counters */
    int x = 0;
    int y = 0;

    game_data *data = (game_data *)malloc (sizeof (game_data));
    if (!data)
        return NULL;

    data->num_mines = num_mines; /* User requested number of mines */
    data->num_flags = 0;         /* No flags have been placed */
    data->grid.height = height;  /* User requested dimensions */
    data->grid.width  = width;

    /* Set each cell to empty, covered and free of markers */
    for (x = 0; x < MAX_X+2; x++) /* MAX_* + 2 in order to handle
                                     one cell padding of grid */
    {
        for (y = 0; y < MAX_Y+2; y++)
        {
            data->grid.cell[x][y].is_covered = TRUE;
            data->grid.cell[x][y].contents   = EMPTY;
            data->grid.cell[x][y].marker     = NONE;
        }
    }
    
    /* Initialize rand () */
    srand ((unsigned)time (NULL));

    /* Place num_mines mines in the grid
     *
     * width-1, height-1, x+1 and y+1 are there
     * to handle the one cell padding of the grid */
    for (i = 0; i < num_mines;)
    {
        x = rand_limit (width-1);  /* Randomly generate coordinates */
        y = rand_limit (height-1);

        /* Only place a mine if the cell is currently empty */
        if (data->grid.cell[x+1][y+1].contents == EMPTY)
        {
            data->grid.cell[x+1][y+1].contents = MINE;
            i++;
        }
    }

    /* Set all cell adjacent to a mine to the proper number
     *
     * x=1, y=1 and <= data->grid.[height|width] are there
     * to handle the one cell padding of the grid */
    for (x = 1; x <= data->grid.height; x++)
    {
        for (y = 1; y <= data->grid.width; y++)
        {
            /* Next iteration if cell contains a mine */
            if (data->grid.cell[x][y].contents == MINE)
                continue;

            /* Check each adjacent cell and increment contents if it contains a
             * mine. Grid padding is always empty */
            (data->grid.cell[x  ][y-1].contents == MINE) && data->grid.cell[x][y].contents++;
            (data->grid.cell[x  ][y+1].contents == MINE) && data->grid.cell[x][y].contents++;
            (data->grid.cell[x-1][y-1].contents == MINE) && data->grid.cell[x][y].contents++;
            (data->grid.cell[x-1][y  ].contents == MINE) && data->grid.cell[x][y].contents++;
            (data->grid.cell[x-1][y+1].contents == MINE) && data->grid.cell[x][y].contents++;
            (data->grid.cell[x+1][y-1].contents == MINE) && data->grid.cell[x][y].contents++;
            (data->grid.cell[x+1][y  ].contents == MINE) && data->grid.cell[x][y].contents++;
            (data->grid.cell[x+1][y+1].contents == MINE) && data->grid.cell[x][y].contents++;
        }
    }

    return data;
}

void
flag_cell (game_data *data, int x, int y)
{
    mark_cell (data, x, y, FLAG);
}

void
guess_cell (game_data *data, int x, int y)
{
    mark_cell (data, x, y, GUESS);
}

void
unmark_cell (game_data *data, int x, int y)
{
    mark_cell (data, x, y, NONE);
}

void
mark_cell (game_data *data, int x, int y, cell_marker marker)
{
    data->grid.cell[x][y].marker = marker;
}

/* Uncover the selected cell
 *
 * Return 1 if the cell contains a mine
 * Otherwise do a flood fill uncover, stopping
 * at numbers
 */
int
uncover_cell (game_data *data, int x, int y)
{
    if ((x == 0) || (y == 0) || (x == data->grid.width+1) || (y == data->grid.height+1))
        return SUCCESS;

    if (data->grid.cell[x][y].is_covered == FALSE)
    {
    }
    else if (data->grid.cell[x][y].contents == MINE)
    {
        return LOSER;
    }
    else if (data->grid.cell[x][y].contents != EMPTY)
    {
        data->grid.cell[x][y].is_covered = FALSE ;
    }
    else
    {
        data->grid.cell[x][y].is_covered = FALSE;
        uncover_cell (data, x-1, y-1);
        uncover_cell (data, x-1, y);
        uncover_cell (data, x-1, y+1);
        uncover_cell (data, x  , y-1);
        uncover_cell (data, x  , y+1);
        uncover_cell (data, x+1, y-1);
        uncover_cell (data, x+1, y);
        uncover_cell (data, x+1, y+1);
    }

    return SUCCESS;
}

/* TODO: Need a function to handle "uncovering" an already uncovered number
 * TODO: like middle clicking in Windows Minesweeper */
