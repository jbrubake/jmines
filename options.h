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

typedef struct Options Options;

/**
 * Store player options for a game
 */
struct Options {
    int height; /**< @brief grid height */
    int width;  /**< @brief grid width */
    int nmines; /**< @brief number of mines on the grid */
};

Options * set_options (int argc, char **argv);
