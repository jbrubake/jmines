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
#include <time.h>
#include <stdio.h>
#include <assert.h>

#include "grid.h"
#include "debug.h"
#include "errmsg.h"

typedef struct Cell Cell;

/**
 * Representation of a single cell in the grid
 */
struct Cell {
    int contents; /**< @brief @private get values from #CELL_CONTENTS */
    int marker;   /**< @brief @private get values from #CELL_MARKERS */
};

/*
 * Representation of the entire grid
 */
struct Grid
{
    int height; /**< @brief @private grid height */
    int width;  /**< @brief @private grid width */

    int nmines; /**< @brief @private number of mines in the grid */
    int nfound; /**< @brief @private number of mines correctly flagged */
    int nflags; /**< @brief @private number of flags placed */

    Cell *cells; /**< @private */
};

/** @{ @name Private Methods */

/**
 * Generate a random number
 *
 * @param limit upper bound
 *
 * @return random __int__ from 0 to *limit* (inclusive)
 */
static unsigned int
rand_limit (unsigned int limit)
{
    int divisor = RAND_MAX / (limit + 1);
    int r = 0;

    do {
        r = rand () / divisor;
    } while (r > limit);

    return r;
}

/**
 * Return pointer to cell at (y,x)
 *
 * @param y, x coordinates of cell to get
 *
 * @return pointer to Cell at (y,x) or
 *         NULL if coordinates are invalid
 */
static Cell *
get_cell (Grid *grid, int y, int x)
{
    /* bounds checking */
    if ((y < 0) ||
        (x < 0) ||
        (y > grid->height-1) ||
        (x > grid->width-1))
        return NULL;

    return &grid->cells[ y * grid->width + x ];
}

/**
 * Place visible marker at (y,x)
 *
 * @param marker type of marker to place (see #CELL_MARKERS)
 *
 * @pre `cell` is valid
 */
static inline void
cell_mark (Cell *cell, int marker)
{
    assert (cell != NULL);

    /* don't mark a revealed cell */
    if (cell->marker != REVEAL)
        cell->marker = marker;
}
/**@} End Private Methods */

/** @{ @name Public Methods */

/**
 * @brief Grid constructor
 *
 * @param height, width dimensions requested
 * @param nmines        number of mines to place
 *
 * @return pointer to new grid or NULL on failure
 */
Grid *
new_grid (int height, int width, int nmines)
{
    int i, y, x;
    Cell *cellptr;

    Grid *grid = malloc (sizeof (Grid)); 
    if (grid == NULL)
        goto fail_alloc;

    /* clamp dimensions */
    if (height > MAX_Y) height = MAX_Y;
    if (width  > MAX_X) width  = MAX_X;

    /* create the cell array */
    grid->cells = malloc (height * width * sizeof (Cell));
    if (grid->cells == NULL)
    {
        free (grid);
        goto fail_alloc;
    }

    TRACE ((DBG_STD, "Created grid (0x%x)\n", grid));

    grid->height = height;
    grid->width  = width;
    grid->nmines = nmines;
    grid->nfound = 0; /* haven't found any mines yet */
    grid->nflags = 0; /* no flags placed yet */

    /* start empty and covered */
    cellptr = grid->cells;
    for (i = 0; i < height * width; i++)
    {
        cellptr->contents = EMPTY;
        cellptr->marker   = NONE;
        cellptr++;
    }

    TRACE ((DBG_STD, "Created cells (0x%x)\n", grid->cells));

    /* boring seed */
    srand ((unsigned)time (NULL));

    /* randomly place the mines */
    for (i = 0; i < nmines;)
    {
        y = rand_limit (height-1);
        x = rand_limit (width-1);

        cellptr = get_cell (grid, y, x);        
        if (cellptr->contents != MINE)
        {
            cellptr->contents = MINE;
            i++; /* one less mine to place */

            /* the following creates the numbers correctly
             *
             * depends on ONE, TWO...EIGHT being defined
             * as 1, 2...8 respectively
             */
            if ((cellptr = get_cell (grid, y-1, x-1)) != NULL &&
                 cellptr->contents != MINE)
                cellptr->contents++;
            if ((cellptr = get_cell (grid, y-1, x  )) != NULL &&
                 cellptr->contents != MINE)
                cellptr->contents++;
            if ((cellptr = get_cell (grid, y-1, x+1)) != NULL &&
                 cellptr->contents != MINE)
                cellptr->contents++;
            if ((cellptr = get_cell (grid, y  , x-1)) != NULL &&
                 cellptr->contents != MINE)
                cellptr->contents++;
            if ((cellptr = get_cell (grid, y  , x+1)) != NULL &&
                 cellptr->contents != MINE)
                cellptr->contents++;
            if ((cellptr = get_cell (grid, y+1, x-1)) != NULL &&
                 cellptr->contents != MINE)
                cellptr->contents++;
            if ((cellptr = get_cell (grid, y+1, x  )) != NULL &&
                 cellptr->contents != MINE)
                cellptr->contents++;
            if ((cellptr = get_cell (grid, y+1, x+1)) != NULL &&
                 cellptr->contents != MINE)
                cellptr->contents++;
        }
    }

    return grid;

fail_alloc:
    fprintf (stderr, "Couldn't create grid");
    return NULL;
}

/**
 * @brief Grid destructor
 */
inline void
delete_grid (Grid *grid)
{
    free (grid->cells);
    free (grid);
}

/**
 * Reveal the selected cell and all adjacent
 * untile number or a mine is found
 *
 * @param y, x coordinates of cell to reveal
 *
 * @return -1 if a mine is revealed, 0 otherwise
 */
int
cell_reveal (Grid *grid, int y, int x)
{
    Cell *cellptr;

    cellptr = get_cell (grid, y, x);
    if (cellptr == NULL) return 0;

    /* do nothing if we try to reveal an already revealed cell */
    if (cellptr->marker == REVEAL)
        return 0;
    else if (cellptr->contents == MINE)
        return -1; /* oops! */
    else if (cellptr->contents != EMPTY)
    {
        cellptr->marker = REVEAL; /* reveal non-empty, non-mine cells */
        return 0;
    }
    else
    {
        /* cell is empty so, reveal it and all adjacent cells */
        cellptr->marker = REVEAL;
        cell_reveal (grid, y-1, x-1);
        cell_reveal (grid, y-1, x  );
        cell_reveal (grid, y-1, x+1);
        cell_reveal (grid, y  , x-1);
        cell_reveal (grid, y  , x+1);
        cell_reveal (grid, y+1, x-1);
        cell_reveal (grid, y+1, x  );
        cell_reveal (grid, y+1, x+1);
    }

    return 0;
}

/**
 * Place a flag at (y,x)
 *
 * @param y, x coordinates of cell to flag
 *
 * @return
 *    -  0 if all mines are properly marked
 *    -  1 if flag was placed
 *    - -1 on error
 */
int
cell_flag (Grid *grid, int y, int x)
{
    /* placed too many flags */
    if (grid->nflags == grid->nmines)
        return -1;

    Cell *cellptr = get_cell (grid, y, x);
    if (cellptr == NULL) return 1;

    cell_mark (cellptr, FLAG);

    grid->nflags++;

    /* if cell *actually* contains a mine,
     * increment counter */
    if (cellptr->contents == MINE)
        grid->nfound++;

    if (grid->nfound == grid->nmines)
        return 0;

    return 1;
}

/**
 * Mark (y,x) as a guess
 *
 * @param y, x coordinates of cell to mark as a guess
 *
 * @return 0 on success, -1 on error
 */
inline int
cell_guess   (Grid *grid, int y, int x)
{
    Cell *cellptr = get_cell (grid, y, x);
    if (cellptr == NULL) return 1;
    cell_mark (cellptr, GUESS);

    return 0;
}

/**
 * Clear previous marker at (y,x)
 *
 * @param y, x coordinates of cell to unmark
 *
 * @return 0 on success, -1 on error
 */
int
cell_unmark  (Grid *grid, int y, int x)
{
    Cell *cellptr = get_cell (grid, y, x);
    if (cellptr == NULL) return -1;

    /* if cell was a properly marked mine,
     * decrement the counter */
    if ((cellptr->marker == FLAG) && (cellptr->contents == MINE))
        grid->nfound--;

    cell_mark (cellptr, NONE);

    return 0;
}

/**
 * Return value corresponding to what cell should look like
 *
 * @param y, x coordinates of cell
 *
 * @return #CELL_MARKERS or #CELL_CONTENTS value or -1 if
 *     coordinats are invalid
 */
int
get_cell_visible (Grid *grid, int y, int x)
{
    int vis = 0;
    Cell *c = get_cell (grid, y, x);
    if (c == NULL) return -1;

    vis = c->marker; /* show marker */
    if (vis == REVEAL) /* if cell is revealed show contents */
        vis = c->contents;

    return vis;
}

#ifdef DEBUG
/**
 * Return value of cell's contents
 *
 * @param y, x coordinates of cell
 *
 * @return #CELL_CONTENTS value corresponding to coordinates
 */
inline int
get_cell_contents (Grid *grid, int y, int x)
{
    Cell *c = get_cell (grid, y, x);
    if (c == NULL) return -1;

    return c->contents;
}
#endif

/**
 * Get dimensions of grid
 *
 * @param [out] height
 * @param [out] width
 *
 * @return places grid dimensions in *height* and *width*
 */
inline void
get_size (Grid *grid, int *height, int *width)
{
    *height = grid->height;
    *width  = grid->width;
}

/**
 * @return number of mines in grid
 */
inline int
get_nmines (Grid *grid)
{
    return grid->nmines;
}

/**
 * @return number of flags placed
 */
inline int
get_nflags (Grid *grid)
{
    return grid->nflags;
}
/** @} End Public Methods */
