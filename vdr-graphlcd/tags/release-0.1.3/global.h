/**
 *  GraphLCD plugin for the Video Disk Recorder 
 *
 *  global.h  -  global definitions
 *
 *  (c) 2001-2004 Carsten Siebholz <c.siebholz AT t-online de>
 **/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program;                                              *
 *   if not, write to the Free Software Foundation, Inc.,                  *
 *   59 Temple Place, Suite 330, Boston, MA  02111-1307  USA               *
 *                                                                         *
 ***************************************************************************/

#ifndef _GRAPHLCD_GLOBAL_H_
#define _GRAPHLCD_GLOBAL_H_


#include <stdlib.h>

#include <glcddrivers/driver.h>


#define PLUGIN_NAME "graphlcd"
#define SPACEWIDTH  2

#define FREENULL(p) (free (p), p = NULL)

template<class T> inline void clip(T & value, T min, T max)
{
	if (value < min) value = min;
	if (value > max) value = max;
}

typedef unsigned char   byte;
typedef unsigned short  word;
typedef unsigned int    dword;

extern GLCD::cDriver * LCD;

#endif
