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
#ifndef CAN_DEBUGGER_HARDWARE
#define CAN_DEBUGGER_HARDWARE

#include <xpcc/io/iodevice.hpp>
#include <xpcc/architecture/interface/can.hpp>

class
CanMessageReceiver(
{
public:
	virtual void
	receiveCanMessage()
}

/// hardware interface that can be handed to the higher level code
class
Hardware
{
public:
	virtual void
	initialize();

	/// returns a string describing the hardware used
	virtual const char*
	getDescription() = 0;

	/// returns a reference to an IODevice that can be used for debug output
	virtual IODevice&
	getDebugIODevice() = 0;

	/// returns a reference to an IODevice that can be used to communicate with the host pc
	virtual IODevice&
	getHostIODevice() = 0;

	/// change LEDs/Display/... to indicate the mode
	virtual void
	displayMode(const uint8_t mode) = 0;

	/// toggle LED to indicate an rx transaction
	virtual void
	indicateRx() = 0;

	/// toggle LED to indicate an tx transaction
	virtual void
	indicateTx() = 0;

	/// (re)initialize can
	virtual void
	initializeCan(xpcc::can::Bitrate bitrate) = 0;

	/// change the can mode
	virtual void
	setCanMode(xpcc::can::Mode mode) = 0;

	/// returns true if a new message was received
	virtual bool
	isCanMessageAvailable() = 0;

	/// returns true if a new message was received
	virtual bool
	sendCanMessage(const can::Message& message) = 0;

// TODO: ass generic can filter class
//	/// add/change a can filter
//	virtual void
//	setCanFilter(xpcc::can::Filer filter) = 0;

}



#endif // CAN_DEBUGGER_HARDWARE
