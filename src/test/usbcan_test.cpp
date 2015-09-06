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

#include <xpcc/debug/logger.hpp>

// Set the log level
#undef	XPCC_LOG_LEVEL
#define	XPCC_LOG_LEVEL xpcc::log::DEBUG

#define TEST_ASSERT_CHAR_EQUALS(x,y) \
TEST_ASSERT_EQUALS(static_cast<uint32_t>(x), static_cast<uint32_t>(y))
#define TEST_ACK()  { char cc; host.get(cc); TEST_ASSERT_CHAR_EQUALS(cc, '\r'); }
#define TEST_NACK() { char cc; host.get(cc); TEST_ASSERT_CHAR_EQUALS(cc, '\x07'); }

TestHardware hardware;
UsbCan usbCan(hardware);
xpcc::IOStream host(hardware.hostDevice.other);

static inline void reset() {
	hardware.reset();
	usbCan.enable();
}

void
UsbcanTest::testOpenCloseChannel()
{
	reset();

	// try to open channel without bitrate set
	host << "O\r"; usbCan.run();
	TEST_NACK();

	// try to set bitrate to 125 kbps and then open regular channel
	host << "S4\rO\r"; usbCan.run();
	// check for acknoledgements
	TEST_ACK();
	TEST_ACK();
	// check if can was initialized
	TEST_ASSERT_TRUE(hardware.can.initializeCalled);
	TEST_ASSERT_TRUE(hardware.can.mode     == xpcc::Can::Mode::Normal);
	TEST_ASSERT_TRUE(hardware.can.bitrate  == xpcc::Can::Bitrate::kBps125);
	TEST_ASSERT_TRUE(hardware.can.busState == xpcc::Can::BusState::Connected);

	// try double close
	host << "S4\rO\r"; usbCan.run();
	TEST_NACK();
	TEST_NACK();

	// try to close channel
	host << "C\r"; usbCan.run();
	TEST_ACK();
	// FIXME: this currently does not work, as xpcc does not provide an
	//        interface to turn off the can module
	//TEST_ASSERT_TRUE(hardware.can.busState == xpcc::Can::BusState::Off);

	// try to close again
	host << "C\r"; usbCan.run();
	TEST_NACK();

	// try to set bitrate to 1 Mbps and then open listen only channel
	hardware.can.initializeCalled = false;
	host << "S8\rL\r"; usbCan.run();
	// check for acknoledgements
	TEST_ACK();
	TEST_ACK();
	// check if can was initialized
	TEST_ASSERT_TRUE(hardware.can.initializeCalled);
	TEST_ASSERT_TRUE(hardware.can.mode     == xpcc::Can::Mode::ListenOnly);
	TEST_ASSERT_TRUE(hardware.can.bitrate  == xpcc::Can::Bitrate::MBps1);
	TEST_ASSERT_TRUE(hardware.can.busState == xpcc::Can::BusState::Connected);
}

void
UsbcanTest::testSendCanMsg()
{
	reset();

	// transmit empty standard CAN frame (this should fail as the channel is closed)
	host << "t0f40\r"; usbCan.run();
	TEST_NACK();

	// open channel
	host << "S4\rO\r"; usbCan.run();
	TEST_ACK();
	TEST_ACK();

	// transmit empty standard CAN frame
	host << "t0f40\r"; usbCan.run();
	TEST_ACK();
	TEST_ASSERT_TRUE(!hardware.can.debugOutFifo.isEmpty());
	xpcc::can::Message msg = hardware.can.debugOutFifo.get();
	hardware.can.debugOutFifo.pop();
	TEST_ASSERT_EQUALS(msg.getIdentifier(), 0xf4u);
	TEST_ASSERT_EQUALS(msg.getLength(), 0);

	// send invalid send commands
	host << "tfff0\r"; usbCan.run();	// id too high only 11 bits supported
	TEST_NACK();
	host << "t0ff300ff4\r"; usbCan.run();	// one octet missing
	TEST_NACK();
	host << "t0ff30RMf4.\r"; usbCan.run();	// invalid character in payload
	TEST_NACK();
	host << "t0f.3000000\r"; usbCan.run();	// invalid character in id
	TEST_NACK();
}

void
UsbcanTest::testReceiveCanMsg()
{
	reset();
	char temp[100];

	// receive extended can message
	xpcc::can::Message msg(0xf450, 5);
	msg.flags.extended = true;
	msg.data[0] = 0xca;
	msg.data[1] = 0xff;
	msg.data[2] = 0xe0;
	msg.data[3] = 0x13;
	msg.data[4] = 0x37;
	hardware.can.debugInFifo.push(msg);
	usbCan.run();
	host.get(temp);
	TEST_ASSERT_EQUALS_STRING(temp, "T0000f4504caffe01337\r");

	// receive standard can message
	xpcc::can::Message msg2(0x00ff, 3);
	msg2.flags.extended = false;
	msg2.data[0] = 0x0e;
	msg2.data[1] = 0xff;
	msg2.data[2] = 0xca;
	hardware.can.debugInFifo.push(msg2);
	usbCan.run();
	host.get(temp);
	TEST_ASSERT_EQUALS_STRING(temp, "r00ff30effca\r");

}

void
UsbcanTest::testBaudrate()
{
	reset();

	// set invalid baudrate
	host << "S7\r"; usbCan.run();	// S7 (800kbps) is not availbale on our can debugger
	TEST_NACK();
	host << "S\rS9\rS54\r"; usbCan.run();
	TEST_NACK();
	TEST_NACK();
	TEST_NACK();
}

void
UsbcanTest::testUnsupportedAndInvalidCommands()
{
	reset();

	// some invalid and undefined commands
	char invalid[] = {
		// unsupported
		'M',	// SetAcceptanceCode
		'm',	// SetAcceptanceMask
		's',	// SetBtr0Btr1
		// invalid
		'A', 'B', '0', 'n', 'x'};
	for(char cc : invalid) {
		host << cc << '\r'; usbCan.run();
		TEST_NACK();
	}
}

void
UsbcanTest::testReadCommands()
{
	reset();
	char temp[100];

	// ReadStatusFlag
	host << "F\r";  usbCan.run();
	host.get(temp);
	// TODO: what is the actual response expected
	TEST_ASSERT_EQUALS_STRING(temp, "00\r");

	// ReadSerialNumber
	host << "N\r";  usbCan.run();
	host.get(temp);
	TEST_ASSERT_EQUALS_STRING(temp, "N0000\r");

	// ReadHardwareAndFirmwareVersion
	host << "V\r";  usbCan.run();
	host.get(temp);
	TEST_ASSERT_EQUALS_STRING(temp, "V1000\r");
}
