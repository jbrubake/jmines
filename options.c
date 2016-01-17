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
#include <getopt.h>

#include "options.h"
#include "version.h"

/* TODO: print_help and print_version should be in another module */

/**
 * @brief Print a help message and exit
 */
void
print_help (void)
{
    printf ("Usage: %s [OPTION]...\n", PROG_NAME);
    printf ("Play minesweeper on the command line.\n");
    printf ("\n");
    printf (" -n <num>\tset number of mines to <num>\n");
    printf (" -x <width>\tset grid with to <width>\n");
    printf (" -y <height>\tset grid height to <height>\n");
    printf (" -h\t\tdisplay this help end exit\n");
    printf (" -v\t\toutput version information and exit\n");

    exit (0);
}

/**
 * @brief Print program version and exit
 */
void
print_version (void)
{
    printf ("%s %s\n", PROG_NAME, VERSION);
    printf ("Copyright (C) %s\n", COPYRIGHT);
    printf ("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n");
    printf ("This is free software: you are free to change and redistribute it.\n");
    printf ("There is NO WARRANTY, to the extent permitted by law.\n");
    printf ("\n");
    printf ("Written by %s.\n", AUTHORS);

    exit (0);
}

/**
 * Process command line options
 *
 * @return pointer to populated Options struct
 *
 * @note This function will exit if a help or version
 * message is requested on the command line
 */
Options *
set_options (int argc, char **argv)
{
    int opt = 0;

    Options *options = malloc (sizeof (Options));
    if (options == NULL) return NULL;

    options->height = 10;
    options->width  = 10;
    options->nmines = 10;

    /* -h : print help
     * -v : print version
     * -nN: number of mines
     * -xX: minefield width
     * -yY: minefield height
     */
    while ((opt = getopt (argc, argv, "hvn:x:y:")) != -1)
    {
        switch (opt)
        {
            case 'v':
                print_version ();
                break;
            case 'y':
                options->height = atoi (optarg);
                break;
            case 'x':
                options->width = atoi (optarg);
                break;
            case 'n':
                options->nmines = atoi (optarg);
                break;
            case 'h': /* fall thru */
            default:
                print_help ();
                break;
        }
    }

    if (options->height == 0) options->height = 10;
    if (options->width  == 0) options->width  = 10;
    if (options->nmines == 0) options->nmines = 10;

    return options;
}

