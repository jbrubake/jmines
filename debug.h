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

/**
 * Debug level constants
 *
 * Add as needed. All debug messages with level
 * <= DEBUG_LEVEL will be printed.
 */
enum DEBUG_LEVELS {

DBG_STD = 0, /**< Standard debug messages */

};

/**
 * All debug messages with a level
 * <= DEBUG_LEVEL will be printed
 */

/*
 * This should be defined in the Makefile,
 * but we check it here  just in case.
 */
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL DBG_STD
#endif

/* TRACE */
/** 
 * `TRACE((int level, char *fmt, ...))`
 *
 * @param level debug level at which output will appear
 * @param fmt   printf(3) format string
 * @param ...   format string args
 *
 * @note Double parens are *required*
 */
#ifdef DEBUG
#   define TRACE(x) dbg_trace x
#else
#   define TRACE(x) do {if (0) dbg_trace x; } while (0)
#endif /* DEBUG */

extern void dbg_trace (int, const char *, ...);

