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
#ifndef CAN_DEBUGGER_HARDWARE_ABSTRACT_CAN
#define CAN_DEBUGGER_HARDWARE_ABSTRACT_CAN

#include <xpcc/architecture/interface/can.hpp>

class
AbstractCan
{
public:
	/// (re)initialize can
	virtual void
	initialize(const xpcc::Can::Mode mode, const uint32_t bitrate) = 0;

	/// Returns true if a message was copied into the message buffer
	virtual bool
	getMessage(xpcc::can::Message& message) = 0;

	/**
	 * Send a message over the CAN.
	 * @return true if the message was send, false otherwise
	 */
	virtual bool
	sendMessage(const xpcc::can::Message& message) = 0;

	/// Get Receive Error Counter.
	virtual uint8_t
	getReceiveErrorCounter() = 0;

	/// Get Transmit Error Counter.
	virtual uint8_t
	getTransmitErrorCounter() = 0;

	virtual xpcc::Can::BusState
	getBusState() = 0;
};

template<class ClockSource, class CanHardware, uint8_t InterruptPriority, uint16_t Tolerance = xpcc::Tolerance::OnePercent>
class CanWrapper : public AbstractCan
{
public:
	void
	initialize(const xpcc::Can::Mode mode, const uint32_t bitrate) override
	{
		typename CanHardware::Mode mm = convertMode(mode);

		switch(bitrate) {
		case xpcc::Can::Bitrate::kBps10:
			CanHardware::template initialize<ClockSource, CanHardware::Bitrate::kBps10, Tolerance>(InterruptPriority, mm);
			break;
		case xpcc::Can::Bitrate::kBps20:
			CanHardware::template initialize<ClockSource, CanHardware::Bitrate::kBps20, Tolerance>(InterruptPriority, mm);
			break;
		case xpcc::Can::Bitrate::kBps50:
			CanHardware::template initialize<ClockSource, CanHardware::Bitrate::kBps50, Tolerance>(InterruptPriority, mm);
			break;
		case xpcc::Can::Bitrate::kBps100:
			CanHardware::template initialize<ClockSource, CanHardware::Bitrate::kBps100, Tolerance>(InterruptPriority, mm);
			break;
		case xpcc::Can::Bitrate::kBps125:
			CanHardware::template initialize<ClockSource, CanHardware::Bitrate::kBps125, Tolerance>(InterruptPriority, mm);
			break;
		case xpcc::Can::Bitrate::kBps250:
			CanHardware::template initialize<ClockSource, CanHardware::Bitrate::kBps250, Tolerance>(InterruptPriority, mm);
			break;
		case xpcc::Can::Bitrate::kBps500:
			CanHardware::template initialize<ClockSource, CanHardware::Bitrate::kBps500, Tolerance>(InterruptPriority, mm);
			break;
		case xpcc::Can::Bitrate::MBps1:
			CanHardware::template initialize<ClockSource, CanHardware::Bitrate::MBps1, Tolerance>(InterruptPriority, mm);
			break;
		}
	}

	bool
	getMessage(xpcc::can::Message& message) override
	{ return CanHardware::getMessage(message); }

	bool
	sendMessage(const xpcc::can::Message& message) override
	{ return CanHardware::sendMessage(message); }

	uint8_t
	getReceiveErrorCounter() override
	{ return CanHardware::getReceiveErrorCounter(); }

	uint8_t
	getTransmitErrorCounter() override
	{ return CanHardware::getTransmitErrorCounter(); }

	xpcc::Can::BusState
	getBusState() override
	{ return CanHardware::getBusState(); }

private:
	static inline typename CanHardware::Mode
	convertMode(const xpcc::Can::Mode mode){
		switch(mode) {
		case xpcc::Can::Mode::Normal: return CanHardware::Mode::Normal;
		case xpcc::Can::Mode::ListenOnly: return CanHardware::Mode::ListenOnly;
		case xpcc::Can::Mode::LoopBack: return CanHardware::Mode::LoopBack;
		case xpcc::Can::Mode::ListenOnlyLoopBack: return CanHardware::Mode::ListenOnlyLoopBack;
		default: return CanHardware::Mode::Normal;
		}
	}
};


#endif // CAN_DEBUGGER_HARDWARE_ABSTRACT_CAN
