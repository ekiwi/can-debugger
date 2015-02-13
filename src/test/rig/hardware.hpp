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

#ifndef CAN_DEBUGGER_TEST_RIG_HARDWARE
#define CAN_DEBUGGER_TEST_RIG_HARDWARE

#include "../../hardware/hardware.hpp"

/**
 * This class emulates a hardware interface for running unittests.
 */
class
TestHardware
{
public:
	TestHardware();

	void
	initialize() override;

	const char*
	getDescription() override
	{ return Description; }

	uint8_t
	getMajorVersion() override
	{ return MajorVersion; }

	uint8_t
	getMinorVersion() override
	{ return MinorVersion; }

	xpcc::IODevice&
	getDebugIODevice() override
	{ return debugDevice; }

	xpcc::IODevice&
	getHostIODevice() override
	{ return hostDevice; }

	void
	indicateMode(const uint8_t mode) override
	{ currentMode = mode; }

	void
	indicateRx() override
	{ rxLedState = !rxLedState; }

	void
	indicateTx() override
	{ txLedState = !txLedState; }

public:
	bool rxLedState;
	bool txLedState;
	uint8_t currentMode;

private:
	static constexpr const char * Description = "Unittest Hardware";
	static constexpr uint8_t MajorVersion = 1;
	static constexpr uint8_t MinorVersion = 0;

};	// TestHardware


#endif	// CAN_DEBUGGER_TEST_RIG_HARDWARE
