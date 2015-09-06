// coding: utf-8
// -----------------------------------------------------------------------------
/*
 * Copyright (C) 2008 Fabian Greif, Roboterclub Aachen e.V.
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

#include <xpcc/driver/can/can_lawicel_formatter.hpp>
#include "usbcan.hpp"


void
UsbCan::enable()
{
	isChannelOpen = false;
	isBitrateSet  = false;
	rxBuffer.reset();

	lastRxErrorCount = hardware.getCanReceiveErrorCounter();
	lastTxErrorCount = hardware.getCanTransmitErrorCounter();
}

void
UsbCan::disable()
{
	// TODO: implement
}

void
UsbCan::run()
{
	xpcc::can::Message msg;

	// 1.) check for new can message that were received
	if(hardware.getCanMessage(msg) && isChannelOpen) {
		char str[128];
		xpcc::CanLawicelFormatter::convertToString(msg, str);
		host << str;
		// TODO: timestamps, check if format is correct
	}

	// 2.) check if any command from the host was received
	char cc = xpcc::IOStream::eof;
	host.get(cc);
	while(cc != xpcc::IOStream::eof){
		if(cc == '\r') {
			decodeCommand();
			rxBuffer.reset();
		} else {
			if(!rxBuffer.write(cc)) {
				// format-error: command to long!
				rxBuffer.reset();
			}
		}
		host.get(cc);
	}

	// 3.) check if there was a can error
	uint8_t new_rx = hardware.getCanReceiveErrorCounter();
	uint8_t new_tx = hardware.getCanTransmitErrorCounter();

	if(lastRxErrorCount != new_rx || lastTxErrorCount != new_tx) {
		host << 'E' << xpcc::hex << new_rx << new_tx << xpcc::ascii << '\r';
	}

	lastRxErrorCount = new_rx;
	lastTxErrorCount = new_tx;
}

enum class
Command : char
{
	SetBitrate = 'S',
	SendFrameR = 'R',
	SendFramer = 'r',
	SendFrameT = 'T',
	SendFramet = 't',
	SetAcceptanceCode = 'M',
	SetAcceptanceMask = 'm',
	SetBtr0Btr1 = 's',
	OpenChannel = 'O',
	OpenListenOnly = 'L',
	CloseChannel = 'C',
	ReadStatusFlag = 'F',
	ReadSerialNumber = 'N',
	ReadHardwareAndFirmwareVersion = 'V',
	TurnOnOrOffTimeStamps = 'Z',
};

enum class
Error
{
	None,
	ChannelAlreadyOpen,
	InvalidCommandBody,
	InvalidCanMsgString,
	FailedToSendCanMsg,
	NotSupported,
	BitrateNotSet,
	ChannelNotOpen,
	InvalidOrEmptyCommand,
};

static inline uint32_t
convertBitrate(char bitrate)
{
	switch(bitrate) {
	case 0:  return xpcc::Can::Bitrate::kBps10;
	case 1:  return xpcc::Can::Bitrate::kBps20;
	case 2:  return xpcc::Can::Bitrate::kBps50;
	case 3:  return xpcc::Can::Bitrate::kBps100;
	case 4:  return xpcc::Can::Bitrate::kBps125;
	case 5:  return xpcc::Can::Bitrate::kBps250;
	case 6:  return xpcc::Can::Bitrate::kBps500;
	case 7:  return xpcc::Can::Bitrate::MBps1;
	default: return xpcc::Can::Bitrate::kBps125;
	}
}

void
UsbCan::decodeCommand()
{
	Error error = Error::None;
	Command command = static_cast<Command>(rxBuffer.read(0));

	switch(command){
	case Command::SetBitrate:
	{
		// Lawicel has the folowing settings:
		// S0   10 kbps
		// S1   20 kbps
		// S2   50 kbps
		// S3  100 kbps
		// S4  125 kbps
		// S5  250 kbps
		// S6  500 kbps
		// S7  800 kbps
		// S8    1 mbps; is index 7 on USB2CAN
		// Note that the USB2CAN adapter does not support 800 kbps.
		// With index 7 USB2CAN runs on 1 mbps.
		// Report error if the range is wrong, communication channel
		// already open, Parameter wrong or t
		char bitrate = rxBuffer.read(1) - '0';
		if(isChannelOpen) {
			error = Error::ChannelAlreadyOpen;
		} else if(rxBuffer.getLength() != 2 || bitrate > 8 || bitrate == 7) {
			error = Error::InvalidCommandBody;
		} else {
			bitrate = (bitrate == 8)? 7 : bitrate;
			this->bitrate = convertBitrate(bitrate);
			isBitrateSet = true;
		}
		break;
	}
	case Command::SendFrameR:
	case Command::SendFramer:
	case Command::SendFrameT:
	case Command::SendFramet:
		if(!isChannelOpen) {
			// TODO: shouldn't this also check for isBitrateSet??
			error = Error::ChannelNotOpen;
		} else {
			// send message
			xpcc::can::Message msg;
			if(xpcc::CanLawicelFormatter::convertToCanMessage(rxBuffer.getRaw(), msg)) {
				if(!hardware.sendCanMessage(msg)) {
					error = Error::FailedToSendCanMsg;
				}
			} else {
				error = Error::InvalidCanMsgString;
			}
		}
		break;
	case Command::SetAcceptanceCode:
	case Command::SetAcceptanceMask:
	case Command::SetBtr0Btr1:
		error = Error::NotSupported;
		break;
	case Command::OpenChannel:
	case Command::OpenListenOnly:
		if(isChannelOpen) {
			error = Error::ChannelAlreadyOpen;
		} else if(!isBitrateSet) {
			error = Error::BitrateNotSet;
		} else {
			if(command == Command::OpenChannel) {
				this->mode = xpcc::Can::Mode::Normal;
			} else if(command == Command::OpenListenOnly) {
				this->mode = xpcc::Can::Mode::ListenOnly;
			}
			hardware.initializeCan(this->mode, this->bitrate);
			// TODO: initialize filters
			isChannelOpen = true;
		}
		break;
	case Command::CloseChannel:
		if(!isChannelOpen) {
			error = Error::ChannelNotOpen;
		} else {
			// TODO: shouldn't we disable the CAN peripheral here?
			isChannelOpen = false;
		}
		break;
	case Command::ReadStatusFlag:
		host << xpcc::hex << static_cast<uint8_t>(0) << xpcc::ascii;
		// TODO But what?
		break;
	case Command::ReadSerialNumber:
		host << 'N' << xpcc::hex
			<< static_cast<uint8_t>(0)
			<< static_cast<uint8_t>(0) << xpcc::ascii;
		break;
	case Command::ReadHardwareAndFirmwareVersion:
		{
		const uint8_t hw_version =
			(hardware.getMajorVersion() << 4) | (hardware.getMinorVersion() & 0x0f);
		const uint8_t sw_version = 0;	// TODO
		host << 'V' << xpcc::hex << hw_version << sw_version << xpcc::ascii;
		break;
		}
	case Command::TurnOnOrOffTimeStamps:
		// Switch on or off timestamps.
		// On Lawicel this value is stored in EEPROM.
		if(isChannelOpen) {
			// TODO: why can this not be changed while the channel is open?
			error = Error::ChannelAlreadyOpen;
		} else if(rxBuffer.getLength() != 2) {
			error = Error::InvalidCommandBody;
		} else {
			useTimestamps = (rxBuffer.read(1) != '0');
		}
		break;
	default:
		error = Error::InvalidOrEmptyCommand;
		break;
	}

	if(error == Error::None) {
		host << '\r';	// command could be executed
	} else {
		host << '\x07';
	}
}
