/*
 * GraphLCD graphics library
 *
 * common.h  -  various functions
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * (c) 2004 Andreas Regel <andreas.regel AT powarman.de>
 */

#ifndef _GLCDGRAPHICS_COMMON_H_
#define _GLCDGRAPHICS_COMMON_H_

#include <string>

namespace GLCD
{

void clip(int & value, int min, int max);
void sort(int & value1, int & value2);
std::string trim(const std::string & s);

} // end of namespace

#endif
