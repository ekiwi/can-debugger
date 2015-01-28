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

#ifndef CAN_DEBUGGER_MODE
#define CAN_DEBUGGER_MODE

#include <xpcc/io/iostream.hpp>
#include "../hardware/hardware.hpp"


class
Mode
{
public:
	Mode(Hardware& hardware) : hardware(hardware), host(hardware.getHostIODevice()) {}

	virtual void
	enable() = 0;

	virtual void
	disable() = 0;

	virtual void
	run() = 0;

protected:
	Hardware& hardware;
	IOStream& host;
};

#endif // CAN_DEBUGGER_MODE
