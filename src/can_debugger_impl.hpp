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

#ifndef CAN_DEBUGGER_CAN_DEBUGGER
	#error	"Don't include this file directly, use 'can_debugger.hpp' instead!"
#endif

#include <xpcc/debug/logger.hpp>
#undef	XPCC_LOG_LEVEL
#define	XPCC_LOG_LEVEL xpcc::log::DEBUG

template<class Hardware>
bool
CanDebugger<Hardware>::run()
{
	PT_BEGIN();
	XPCC_LOG_DEBUG << "[can-debugger] running" << xpcc::endl;

	while(true) {
		PT_YIELD();
	}

	PT_END();
}

