/*--------------------------------------------------------------------*\
| This file is part of jmines
|
| Header file for game engine
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
#ifndef GAME_H
#define GAME_H

/* XXX: There must be another way to do this! */
#include <ncurses.h>

/* Max dimensions of grid */
#define MAX_Y 100
#define MAX_X 100

/* Function return codes */
#define SUCCESS 0
#define WINNER  1
#define LOSER   2

typedef enum cell_contents {
    EMPTY,                                          /* Cell is empty */
    ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, /* Cell is adjacent to mines */
    MINE                                            /* Cell contains a mine */
} cell_contents;

typedef enum cell_marker {
    NONE, /* Cell is not marked */
    FLAG, /* Cell is marked as a possible mine */
    GUESS /* Cell is marked as a guess */
} cell_marker;

/* Game data structure */
typedef struct game_data {
    int num_mines; /* Number of mines in grid */
    int num_flags; /* Number of actual mines flagged */

    /* The grid */
    struct {
        int height; /* Grid dimensions */
        int width;

        /* The cells */
        struct {
            bool is_covered;        /* True if cell contents are hidden */
            cell_contents contents; /* What the cell contains */
            cell_marker marker;     /* What the cell is marked with */
        } cell[MAX_X+2][MAX_Y+2];   /* Add one cell padding on each side */
    } grid;

    /* TIMER */

} game_data;

/* Init a game structure
 *  @null@ tells splint we can return NULL */
/*@null@*/ game_data *init_game (int, int, int);

int uncover_cell (game_data *, int, int);
int unmark_cell  (game_data *, int, int);
int flag_cell    (game_data *, int, int);
int guess_cell   (game_data *, int, int);

#endif /* !defined (GAME_H) */
