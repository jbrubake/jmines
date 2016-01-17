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

/** @file
 *
 * This file provides error handling routines. The purpose of
 * each function is denoted by its name:
 *
 *    - *_return: print message and return
 *    - *_exit:   print message and return
 *       + sys routines call `exit(errno`)
 *       + other routines call `exit(1)`
 *    - *_dump:   print message, dump core and terminate
 *    - *_sys:    append last `strerror(3)` message
 */

void err_sys_dump (const char *fmt, ...);
void err_sys_exit (const char *fmt, ...);
void err_sys_return (const char *fmt, ...);
void err_exit (const char *fmt, ...);
void err_return (const char *fmt, ...);

