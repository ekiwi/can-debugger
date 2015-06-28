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


#ifndef CAN_DEBUGGER_TEST_RIG_CAN
#define CAN_DEBUGGER_TEST_RIG_CAN

#include "../../hardware/abstract_can.hpp"
#include <xpcc/container/queue.hpp>

class TestCan : public AbstractCan
{
public:
	bool initializeCalled;
	xpcc::Can::Mode mode;
	uint32_t bitrate;
	bool simulateFullSendBuffer;
	uint8_t txErrorCount;
	uint8_t rxErrorCount;
	xpcc::Can::BusState busState;

	static constexpr size_t FifoSize = 5;
	xpcc::BoundedQueue<xpcc::can::Message, FifoSize> debugInFifo;
	xpcc::BoundedQueue<xpcc::can::Message, FifoSize> debugOutFifo;

	void
	reset()
	{
		initializeCalled = false;
		mode = xpcc::Can::Mode::Normal;
		bitrate = 0;
		simulateFullSendBuffer = false;
		txErrorCount = 0;
		rxErrorCount = 0;
		busState = xpcc::Can::BusState::Off;
		while(!debugInFifo.isEmpty())  debugInFifo.pop();
		while(!debugOutFifo.isEmpty()) debugOutFifo.pop();
	}

public:
	void
	initialize(const xpcc::Can::Mode mode, const uint32_t bitrate) override;

	bool
	getMessage(xpcc::can::Message& message) override;

	bool
	sendMessage(const xpcc::can::Message& message) override;

	uint8_t
	getReceiveErrorCounter() override;

	uint8_t
	getTransmitErrorCounter() override;

	xpcc::Can::BusState
	getBusState() override;
};

#endif	// CAN_DEBUGGER_TEST_RIG_CAN
