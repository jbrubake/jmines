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

/** @{ @name Maximum grid dimensions */
#define MAX_Y 100
#define MAX_X 100
/**@}*/

/**
 * Cell contents
 *
 * @note Values cannot duplicate those of CELL_MARKERS
 */
enum CELL_CONTENTS {

EMPTY  = 0,  /**< Cell is empty */
ONE    = 1,  /**< Cell is adjacent to one mine */
TWO    = 2,  /**< Cell is adjacent to one mine */
THREE  = 3,  /**< Cell is adjacent to one mine */
FOUR   = 4,  /**< Cell is adjacent to one mine */
FIVE   = 5,  /**< Cell is adjacent to one mine */
SIX    = 6,  /**< Cell is adjacent to one mine */
SEVEN  = 7,  /**< Cell is adjacent to one mine */
EIGHT  = 8,  /**< Cell is adjacent to one mine */
MINE   = 9,  /**< Cell contains a mine */
};

/**
 * Cell markers
 *
 * @note Values cannot duplicate those of CELL_CONTENTS
 */
enum CELL_MARKERS {

NONE   = 10, /**< Cell is unrevealed and unmarked */
FLAG   = 11, /**< Cell is flagged as a possible mine */
GUESS  = 12, /**< Cell is marked as a guess */
REVEAL = 13, /**< Cell is revealed */

};

typedef struct Grid Grid;

Grid *new_grid (int height, int width, int nmines);
void  delete_grid (Grid *grid);

int cell_reveal (Grid *grid, int y, int x);
int cell_flag   (Grid *grid, int y, int x);
int cell_guess  (Grid *grid, int y, int x);
int cell_unmark (Grid *grid, int y, int x);

/* return value of a cell's visual representation
 * return -1 if coordinates are invalid */
int get_cell_visible (Grid *grid, int y, int x);
#ifdef DEBUG
int get_cell_contents (Grid *grid, int y, int x);
#endif

int get_nmines (Grid *grid); /* return number of mines */
int get_nflags (Grid *grid); /* return number of flags placed */

/* return grid dimensions in height and width */
void get_size (Grid *grid, int *height, int *width);

