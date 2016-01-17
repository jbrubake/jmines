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
#ifndef UI_H
#define UI_H

#include "../game.h"

/******** INPUT DEFINES *********/
#define INVALID_INPT -1
#define UNCOVER_CELL 0
#define FLAG_CELL    1
#define GUESS_CELL   2
#define UNMARK_CELL  3
#define NEW_GAME     4
#define QUIT_GAME    5

#define CUR_L 6
#define CUR_R 7
#define CUR_U 8
#define CUR_D 9

#define GRID_ORIGIN_Y 0
#define GRID_ORIGIN_X 0

typedef struct Ui Ui;

Ui   *open_ui (int height, int width);
void  close_ui (Ui *ui);

int get_input (Ui *ui);
int move_cursor (Ui *ui, int direction);
int get_cursor  (Ui *ui, int *y, int *x);

int draw_grid (Ui *ui, Grid *grid);

#endif /* !defined (UI_H) */
