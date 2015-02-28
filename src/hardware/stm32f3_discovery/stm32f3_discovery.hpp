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
#include "../hardware.hpp"

class
Stm32F3Discovery : public Hardware
{
public:
	Stm32F3Discovery();

	virtual void
	initialize() override;

	virtual const char*
	getDescription() override
	{ return Description; }

	virtual uint8_t
	getMajorVersion() override
	{ return MajorVersion; }

	virtual uint8_t
	getMinorVersion() override
	{ return MinorVersion; }

	virtual xpcc::IODevice&
	getDebugIODevice() override
	{ return debugDevice; }

	virtual xpcc::IODevice&
	getHostIODevice() override
	{ return hostDevice; }

	virtual void
	indicateMode(const uint8_t mode) override
	{ ModeLeds::write(mode % 4); }

	virtual void
	indicateRx() override
	{ RxLed::toggle(); }

	virtual void
	indicateTx() override
	{ TxLed::toggle(); }

private:
	static constexpr const char * Description = "STM32F3 Discovery";
	static constexpr uint8_t MajorVersion = 3;
	static constexpr uint8_t MinorVersion = 0;

private:
	// Tx: GpioOutputA2, Rx: GpioInputA3
	using HostUart = xpcc::stm32::Usart2;
	static constexpr uint32_t HostUartBaudrate  = 921600;

	// Tx: GpioOutputA9, Rx: GpioInputA10
	using DebugUart = xpcc::stm32::Usart1;
	static constexpr uint32_t DebugUartBaudrate = 115200;

	// 72 MHz System Clock
	using defaultSystemClock = xpcc::stm32::SystemClock< xpcc::stm32::Pll< xpcc::stm32::ExternalClock<MHz8>, MHz72> >;

	// UI: Leds
	using ModeLeds = xpcc::SoftwareGpioPort<xpcc::stm32::GpioOutputE10, xpcc::stm32::GpioOutputE9>;
	using RxLed    = xpcc::stm32::GpioOutputE15;	// LedWest
	using TxLed    = xpcc::stm32::GpioOutputE14;	// LedSouthWest

	// UI: Buttons
	using ModeSwitch = xpcc::stm32::GpioInputA0;	// Button

	// Can
	using Can = xpcc::stm32::Can1;
	static constexpr uint32_t CanInterruptPriority = 10;

private:
	xpcc::IODeviceWrapper<DebugUart, xpcc::IOBuffer::BlockIfFull> debugDevice;
	xpcc::IODeviceWrapper<HostUart,  xpcc::IOBuffer::BlockIfFull> hostDevice;
	CanWrapper<defaultSystemClock, Can, CanInterruptPriority> can;

};	// class Hardware


#endif // CAN_DEBUGGER_HARDWARE_STM32F3_DISCOVER
