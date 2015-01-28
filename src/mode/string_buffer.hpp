// coding: utf-8
// -----------------------------------------------------------------------------
/*
 * Copyright (C) 2015 eKiwi<electron.kiwi@gmail.com>, Roboterclub Aachen e.V.
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, see <http://www.gnu.org/licenses/>.
 */
// -----------------------------------------------------------------------------

#ifndef CAN_DEBUGGER_STRING_BUFFER
#define CAN_DEBUGGER_STRING_BUFFER

#include <stdint.h>

using StringBufferSizeT = uint8_t;


template<StringBufferSizeT Size>
class
StringBuffer
{
public:
	inline bool
	write(char cc)
	{
		if(count >= Size) return false;
		data[count] = cc;
		++count;
		return true;
	}

	inline char
	read(StringBufferSizeT index)
	{
		if(index >= count) return -1;
		return data[index];
	}

	inline StringBufferSizeT
	getLength()
	{
		return count;
	}

	inline char*
	getRaw()
	{
		// make sure string ending is marked
		data[count] = '\0';
		return data;
	}

	inline void
	reset()
	{
		count = 0;
	}


private:
	// one additonal char for the string delimiter '\0'
	char data[Size+1];
	StringBufferSizeT count;
}

#endif // CAN_DEBUGGER_STRING_BUFFER

