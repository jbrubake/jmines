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
#include <stdlib.h>

#include "../game.h"
#include "ui.h"

int
main_loop (Grid *grid)
{
    int height = 0;
    int width  = 0;
    int input  = 0;

    int y = 0;
    int x = 0;

    get_size (grid, &height, &width);
    Ui *ui = open_ui (height, width);
    draw_grid (ui, grid);

    for (;;)
    {
        get_cursor (ui, &y, &x);
        input = get_input (ui);
        switch (input)
        {
            case UNCOVER_CELL:
                if (cell_uncover (grid, y, x) == -1)
                    exit (0);
                break;
            case FLAG_CELL:
                cell_flag (grid, y, x);
                break;
            case GUESS_CELL:
                cell_guess (grid, y, x);
                break;
            case UNMARK_CELL:
                cell_unmark (grid, y, x);
                break;
            case NEW_GAME:
                close_grid (grid);
                grid = new_grid (height, width, 10); /* XXX: Test code */
                break;
            case QUIT_GAME:
                exit (0);
                break;
            case CUR_L: /* one function to handle all four */
            case CUR_R:
            case CUR_D:
            case CUR_U:
                move_cursor (ui, input);
                break;
            default:
                break;
        }

        draw_grid (ui, grid);
    }
}
