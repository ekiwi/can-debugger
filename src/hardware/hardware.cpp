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
#include "hardware.hpp"


void
Hardware::registerChangeModeReceiver(ChangeModeReceiver* receiver)
{
	if(receiver != nullptr) {
		this->changeModeReceiver = receiver;
	}
}

bool
Hardware::changeMode(const uint8_t mode)
{
	if(this->changeModeReceiver != nullptr) {
		if(this->changeModeReceiver->changeMode(mode)) {
			this->indicateMode(mode);
			return true;
		}
	}
	return false;
}
