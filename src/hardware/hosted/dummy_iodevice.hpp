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


#ifndef DUMMY_IODEVICE_HPP
#define DUMMY_IODEVICE_HPP

#include <xpcc/io/iodevice.hpp>
#include <xpcc/container/queue.hpp>

template<size_t InputBufferSize = 200, size_t OutputBufferSize = 200>
class DummyIODevice : public xpcc::IODevice
{
public:
	using InputFifo  = xpcc::BoundedQueue<uint8_t, InputBufferSize>;
	using OutputFifo = xpcc::BoundedQueue<uint8_t, OutputBufferSize>;

	InputFifo& input;
	OutputFifo& output;

	DummyIODevice(InputFifo& input, OutputFifo& output) : input(input), output(output) {}

	virtual void
	write(char c) {
		output.push(static_cast<uint8_t>(c));
	}

	virtual void
	write(const char *s) {
		while (*s)
		{
			output.push(static_cast<uint8_t>(*s));
			s++;
		}
	}

	virtual void
	flush() {
		while(!input.isEmpty())  input.pop();
		while(!output.isEmpty()) output.pop();
	}

	virtual bool
	read(char& c) {
		if(input.isEmpty()) {
			return false;
		} else {
			c = static_cast<char>(input.get());
			input.pop();
			return true;
		}
	}
};

template<size_t BufferSize = 200>
class BidirectionalDummyIODevice : public DummyIODevice<BufferSize, BufferSize>
{
public:
	using Underlying = DummyIODevice<BufferSize, BufferSize>;
	typename Underlying::InputFifo input;
	typename Underlying::OutputFifo output;

public:
	BidirectionalDummyIODevice() : Underlying(input, output), other(output, input) {}

public:
	Underlying other;
};

#endif	// DUMMY_IODEVICE_HPP
