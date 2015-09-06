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
#include "usbcan_test.hpp"
#include "../hardware/hosted/test_hardware.hpp"
#include "../mode/usbcan.hpp"

#define TEST_ASSERT_CHAR_EQUALS(x,y) \
TEST_ASSERT_EQUALS(static_cast<uint32_t>(x), static_cast<uint32_t>(y))
#define TEST_ACK()  { char cc; host.get(cc); TEST_ASSERT_CHAR_EQUALS(cc, '\r'); }
#define TEST_NACK() { char cc; host.get(cc); TEST_ASSERT_CHAR_EQUALS(cc, '\x07'); }

TestHardware hardware;
UsbCan usbCan(hardware);
xpcc::IOStream host(hardware.hostDevice.other);


void
UsbcanTest::testOpenCloseChannel()
{
	hardware.reset();

	// try to set bitrate to 125 kbps and then regular open channel
	host << "S4\rO\r"; usbCan.run();
	// check for acknoledgements
	TEST_ACK();
	TEST_ACK();
	// check if can was initialized
	TEST_ASSERT_TRUE(hardware.can.initializeCalled);
	TEST_ASSERT_TRUE(hardware.can.mode     == xpcc::Can::Mode::Normal);
	TEST_ASSERT_TRUE(hardware.can.bitrate  == xpcc::Can::Bitrate::kBps125);
	TEST_ASSERT_TRUE(hardware.can.busState == xpcc::Can::BusState::Connected);

	// try to close channel
	host << "C\r"; usbCan.run();
	TEST_ACK();
	// FIXME: this currently does not work, as xpcc does not provide an
	//        interface to turn off the can module
	//TEST_ASSERT_TRUE(hardware.can.busState == xpcc::Can::BusState::Off);

	// try to close again
	host << "C\r"; usbCan.run();
	TEST_NACK();
}

void
UsbcanTest::testBaudrate()
{
	// set invalid baudrate
	host << "S7\r"; usbCan.run();	// S7 (800kbps) is not availbale on our can debugger
	TEST_NACK();
	host << "S9\rS54\r"; usbCan.run();
	TEST_NACK();
	TEST_NACK();
}
