// coding: utf-8
// -----------------------------------------------------------------------------
/*
 * Copyright (C) 2008 Fabian Greif, Roboterclub Aachen e.V.
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

#ifndef CAN_DEBUGGER_MODE_USBCAN
#define CAN_DEBUGGER_MODE_USBCAN

#include "mode.hpp"
#include "string_buffer.hpp"

class
UsbCan : public Mode
{
public:
	UsbCan(Hardware& hardware) : Mode(hardware), isChannelOpen(false) {}

	void
	enable();

	void
	disable();

	void
	run();

private:
	void
	decodeCommand();

	bool isChannelOpen;
	bool isBitrateSet;
	bool useTimestamps;

	uint32_t bitrate;
	xpcc::Can::Mode mode;

	uint8_t lastRxErrorCount;
	uint8_t lastTxErrorCount;

	StringBuffer<40> rxBuffer;
};

#endif // CAN_DEBUGGER_MODE_USBCAN
