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

#include "can.hpp"

void
TestCan::initialize(const xpcc::Can::Mode mode, const uint32_t bitrate)
{
	this->initializeCalled = true;
	this->busState = xpcc::Can::BusState::Connected;
	this->mode = mode;
	this->bitrate = bitrate;
}

bool
TestCan::getMessage(xpcc::can::Message& message)
{
	if(this->debugInFifo.isEmpty()) {
		return false;
	} else {
		message = this->debugInFifo.get();
		this->debugInFifo.pop();
		return true;
	}
}

bool
TestCan::sendMessage(const xpcc::can::Message& message)
{
	if(this->simulateFullSendBuffer || this->debugOutFifo.isFull()) {
		return false;
	} else {
		this->debugOutFifo.push(message);
		return true;
	}
}

uint8_t
TestCan::getReceiveErrorCounter()
{ return this->rxErrorCount; }

uint8_t
TestCan::getTransmitErrorCounter()
{ return this->txErrorCount; }

xpcc::Can::BusState
TestCan::getBusState()
{ return this->busState; }
