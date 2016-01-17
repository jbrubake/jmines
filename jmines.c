/** @file
 *
 *  __jmines__ Another minesweeper clone
 *
 * @version   1.0.0
 * @author    Jeremy Brubaker
 * @date      2010-03-25
 * @copyright 2010, 2010 GNU Public License
 */

/*
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
#include <assert.h>

#include "options.h"
#include "errmsg.h"
#include "debug.h"

extern int main_loop (Options *opt); /* provided by the ui module */

/**
 * @return exit with the return value of `main_loop()`
 */
int
main (int argc, char **argv)
{
    Options *opt;

    /* process options */
    opt = set_options (argc, argv);
    if (opt == NULL) err_exit ("Invalid options");

    /* simple check to make sure grid is big enough
     * does not make sure grid size is sensible! */
    if (opt->height * opt->width < opt->nmines)
        err_exit ("Size too small. Cannot place requested mines");

    TRACE ((DBG_STD, "height = %d; width = %d; mines = %d\n",
                opt->height, opt->width, opt->nmines));

    exit (main_loop (opt));
}

