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

#ifndef CAN_DEBUGGER_HARDWARE_STM32F3_DISCOVERY
#define CAN_DEBUGGER_HARDWARE_STM32F3_DISCOVER

#include <xpcc/architecture.hpp>
#include "../../xpcc/examples/stm32f3_discovery/stm32f3_discovery.hpp"

class
Hardware
{
private:
	// Tx: GpioOutputA2, Rx: GpioInputA3
	typedef Usart2 HostUart;
	// Tx: GpioOutputA9, Rx: GpioInputA10
	typedef Usart1 DebugUart;

	static constexpr uint32_t HostUartBaudrate  = 921600;
	static constexpr uint32_t DebugUartBaudrate = 115200;

public:
	typedef ::LedNorth     ModeLed0;
	typedef ::LedNorthEast ModeLed1;
	typedef ::Button ModeSwitch;

	typedef ::xpcc::stm32::Can1 Can;

	static constexpr const char * Name = "STM32F3 Discovery";

	struct
	HostCommunication
	{
		static inline bool
		read(uint8_t& data)
		{ return HostUart::read(data); }
		static inline bool
		write(uint8_t data)
		{ return HostUart::write(data); }
	};

	struct
	DebugCommunication
	{
		static inline bool
		read(uint8_t& data)
		{ return DebugUart::read(data); }
		static inline bool
		write(uint8_t data)
		{ return DebugUart::write(data); }
	};

	static inline void
	initialize()
	{
		defaultSystemClock::enable();

		GpioOutputA9::connect(DebugUart::Tx);
		GpioInputA10::connect(DebugUart::Rx);
		DebugUart::initialize<defaultSystemClock, DebugUartBaudrate>(10);

		GpioOutputA2::connect(HostUart::Tx);
		GpioInputA3::connect(HostUart::Rx);
		HostUart::initialize<defaultSystemClock, HostUartBaudrate>(8);
	}

};	// class Hardware


#endif // CAN_DEBUGGER_HARDWARE_STM32F3_DISCOVER
