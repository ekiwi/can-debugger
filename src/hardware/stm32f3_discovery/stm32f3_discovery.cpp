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

#include "stm32f3_discovery.hpp"

using namespace xpcc::stm32;

Stm32F3Discovery::Stm32F3Discovery() : Hardware(can)
{}

void
Stm32F3Discovery::initialize()
{
	defaultSystemClock::enable();

	ModeLeds::setOutput();
	ModeLeds::write(0);

	RxLed::setOutput(xpcc::Gpio::Low);
	TxLed::setOutput(xpcc::Gpio::Low);

	ModeSwitch::setInput();	// has external pulldown

	GpioOutputA9::connect(DebugUart::Tx);
	GpioInputA10::connect(DebugUart::Rx);
	DebugUart::initialize<defaultSystemClock, DebugUartBaudrate>(10);

	GpioOutputA2::connect(HostUart::Tx);
	GpioInputA3::connect(HostUart::Rx);
	HostUart::initialize<defaultSystemClock, HostUartBaudrate>(8);
}
