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
#ifndef CLI_H
#define CLI_H

#include "game.h"

int game_loop (game_data *);
void get_coordinates (int *x, int *y);

void print_grid    (game_data *);
void print_menu    ();

char print_cell_contents (game_data *, int, int);

#endif /* !defined (CLI_H) */
