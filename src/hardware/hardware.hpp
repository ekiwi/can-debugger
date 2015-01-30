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
#include "abstract_can.hpp"

class
ChangeModeReceiver
{
public:
	virtual bool
	changeMode(const uint8_t mode) = 0;
};

/// hardware interface that can be handed to the higher level code
class
Hardware
{
public:
	Hardware(AbstractCan& can) : can(can), changeModeReceiver(nullptr) {}

	virtual void
	initialize();

	/// @defgroup version Version and Hardware Information
	/// @{

	/// returns a string describing the hardware used
	virtual const char*
	getDescription() = 0;

	/// returns major version of hardware revision
	virtual uint8_t
	getMajorVersion() = 0;

	/// returns minor version of hardware revision
	virtual uint8_t
	getMinorVersion() = 0;

	/// @}


	/// @defgroup communication Host and Debug Interfaces
	/// @{

	/// returns a reference to an IODevice that can be used for debug output
	virtual xpcc::IODevice&
	getDebugIODevice() = 0;

	/// returns a reference to an IODevice that can be used to communicate with the host pc
	virtual xpcc::IODevice&
	getHostIODevice() = 0;

	/// @}



	/// @defgroup ui UI Interactions
	/// @{

	/// change LEDs/Display/... to indicate the mode
	virtual void
	indicateMode(const uint8_t mode) = 0;

	/// toggle LED to indicate an rx transaction
	virtual void
	indicateRx() = 0;

	/// toggle LED to indicate an tx transaction
	virtual void
	indicateTx() = 0;

	/// @}


	/// @defgroup mode Mode Change Management
	/// @{

	/// try to change to a new mode
	bool
	changeMode(const uint8_t mode);

	/// register class instance that will handle mode changes
	void
	registerChangeModeReceiver(ChangeModeReceiver* receiver);

	/// @}

	/// @defgroup can Can
	/// @{

	/// (re)initialize can
	inline void
	initializeCan(const xpcc::Can::Mode mode, const uint32_t bitrate)
	{ return can.initialize(mode, bitrate); }

	/// Returns true if a message was copied into the message buffer
	inline bool
	getCanMessage(xpcc::can::Message& message)
	{ return can.getMessage(message); }

	/**
	 * Send a message over the CAN.
	 * @return true if the message was send, false otherwise
	 */
	inline bool
	sendCanMessage(const xpcc::can::Message& message)
	{ return can.sendMessage(message); }

	/// Get Receive Error Counter.
	inline uint8_t
	getCanReceiveErrorCounter()
	{ return can.getReceiveErrorCounter(); }

	/// Get Transmit Error Counter.
	inline uint8_t
	getCanTransmitErrorCounter()
	{ return can.getTransmitErrorCounter(); }

	inline xpcc::Can::BusState
	getCanBusState()
	{ return can.getBusState(); }

// TODO: add generic can filter class
//	/// add/change a can filter
//	virtual void
//	setCanFilter(xpcc::Can::Filer filter) = 0;

	/// @}

private:
	AbstractCan& can;
	ChangeModeReceiver* changeModeReceiver;

};


#endif // CAN_DEBUGGER_HARDWARE
