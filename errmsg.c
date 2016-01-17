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

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "errmsg.h"

/**
 * Maximum length of error messages
 */
#define MAXLINE 1024

/* do_err */
/**
 * Perform error handling requested by wrapper functions
 *
 * @param errnoflag append system error if equal to 1
 * @param fmt       `printf(3)` style format string
 * @param ap        fmt parameters
 */
static void
do_err (int errnoflag, int error, const char *fmt, va_list ap)
{
    char buf[MAXLINE];

    /* the '-1' is so we save room for terminating newline */
    vsnprintf (buf, MAXLINE-1, fmt, ap);
    if (errnoflag) /* we want the system error */
        snprintf (buf+strlen(buf), MAXLINE-strlen (buf)-1, ": %s",
                strerror (error));

    strcat (buf, "\n");

    fflush (stdout); /* in case stdio and stderr are the same */
    fputs (buf, stderr);
    fflush (NULL); /* all stdio output */
}

/* err_sys_dymp */
/**
 * Handle fatal system errors. Print custom error message with
 * the last system error appended and then dump core.
 *
 * @param fmt `printf(3)` style format string
 * @param ... fmt parameters
 */
void
err_sys_dump (const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    do_err (1, errno, fmt, ap);
    va_end (ap);

    abort (); /* dump core and terminate */
    exit (1); /* just in case */
}

/* err_sys_exit*/
/**
 * Handle fatal system errors. Print custom error message with
 * the last system error appended and then exit.
 *
 * @param fmt `printf(3)` style format string
 * @param ... fmt parameters
 *
 * @return exit with a value of `errno`
 */
void
err_sys_exit (const char *fmt, ...)
{
    va_list ap;
    int e = errno;

    va_start (ap, fmt);
    do_err (1, errno, fmt, ap);
    va_end (ap);

    exit (e);
}

/* err_sys_return*/
/**
 * Handle non-fatal system errors. Print custom error message with
 * the last system error appended and return.
 *
 * @param fmt `printf(3)` style format string
 * @param ... fmt parameters
 */
void
err_sys_return (const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    do_err (1, errno, fmt, ap);
    va_end (ap);
}

/* err_return*/
/**
 * Handle non-fatal non-system errors. Print custom error message
 * and return.
 *
 * @param fmt `printf(3)` style format string
 * @param ... fmt parameters
 */
void
err_return (const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    do_err (0, 0, fmt, ap);
    va_end (ap);
}

/* err_exit*/
/**
 * Handle fatal non-system errors. Print custom error message
 * and exit.
 *
 * @param fmt `printf(3)` style format string
 * @param ... fmt parameters
 *
 * @return exit with a value of 1
 */
void
err_exit (const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    do_err (0, 0, fmt, ap);
    va_end (ap);

    exit (1);
}
