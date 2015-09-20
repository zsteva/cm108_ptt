/**
 *  Hacked-together cm108-ptt library. Nearly 100% copied from Hamlib.
 *  Do-not-trust and will-do-evil-things generally apply.
 *
 *  Also the hidraw device is hardcoded to /dev/hidraw0 (see below in cm108_open)
 *
 *  Have fun with it :D
 */

/*
 *  Hamlib Interface - CM108 GPIO communication header
 *  Copyright (c) 2000-2003 by Frank Singleton
 *  Copyright (c) 2000-2010 by Stephane Fillod
 *  Copyright (c) 2011 by Andrew Errington
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef _CM108_H
#define _CM108_H 1

#define CM108_VID "0d8c"
#define CM108_PID "000c"

/* Hamlib internal use, see rig.c */
const char* cm108_find_device();
int cm108_open();
int cm108_close(int fd);
void cm108_ptt_set(int fd, int state);

#endif /* _CM108_H */
