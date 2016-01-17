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
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "../game.h"

struct Ui
{
    int grid_height;
    int grid_width;

    WINDOW *gridw;
    int curx;
    int cury;
};

/****** PRIVATE METHODS ********/
chtype
cell_character (int c)
{
    switch (c)
    {
        case NONE:
            return '.';
        case FLAG:
            return 'F';
        case GUESS:
            return '?';
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
            return ' '; /* better not end up here */
    }
    return ' '; /* really don't end up here */
}

/****** PUBLIC METHODS ********/
Ui *
open_ui (int height, int width)
{
    initscr ();
    start_color ();
    cbreak ();
    noecho ();
    nonl ();
    intrflush (stdscr, false);
    keypad (stdscr, true);
    curs_set (0);
    atexit ((void (*)(void))endwin);

    refresh ();

    Ui *ui = malloc (sizeof (Ui));
    if (ui == NULL)
        goto open_ui_error;

    ui->gridw = newwin (height, width, GRID_ORIGIN_Y, GRID_ORIGIN_X);
    if (ui->gridw == NULL)
    {
        free (ui);
        goto open_ui_error;
    }

    ui->grid_height = height;
    ui->grid_width  = width;

    ui->cury = 0;
    ui->curx = 0;

    return ui;

open_ui_error:
    fprintf (stderr, "Couldn't create user interface");
    return NULL;
}

void
close_ui (Ui *ui)
{
    free (ui->gridw);
    free (ui);
}

int
get_input (Ui *ui)
{
    int c;

    c = wgetch (ui->gridw);

    switch (c)
    {
        case 'u':
            return UNCOVER_CELL;
        case 'f':
            return FLAG_CELL;
        case 'g':
            return GUESS_CELL;
        case 'U':
            return UNMARK_CELL;
        case 'N':
            return NEW_GAME;
        case 'Q':
            return QUIT_GAME;
        case 'h':
            return CUR_L;
        case 'l':
            return CUR_R;
        case 'j':
            return CUR_D;
        case 'k':
            return CUR_U;
    }

    return INVALID_INPT;
}

int
draw_grid (Ui *ui, Grid *grid)
{
    int y = 0;
    int x = 0;
    int cell = 0;
    chtype c;

    int height = 0;
    int width = 0;

    get_size (grid, &height, &width);

    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++)
        {
            cell = cell_visible (grid, y, x);
            c = cell_character (cell);
            mvwaddch (ui->gridw, y, x, c);
        }

    mvwaddch (ui->gridw, ui->cury, ui->curx, 'X');
    wrefresh (ui->gridw);

    return 0;
}

int
move_cursor (Ui *ui, int direction)
{
    int curx = ui->curx;
    int cury = ui->cury;

    int height = ui->grid_height;
    int width  = ui->grid_width;

    switch (direction)
    {
        case CUR_L:
            if (curx > 0) curx--;
            break;
        case CUR_R:
            if (curx < width-1) curx++;
            break;
        case CUR_U:
            if (cury > 0) cury--;
            break;
        case CUR_D:
            if (cury < height-1) cury++;
            break;
    }

    ui->curx = curx;
    ui->cury = cury;

    return 0;
}

int
get_cursor (Ui *ui, int *y, int *x)
{
    *y = ui->cury;
    *x = ui->curx;

    return 0;
}
