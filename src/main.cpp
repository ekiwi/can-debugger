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

#include <xpcc/debug/logger.hpp>
#include "hardware/stm32f3_discovery.hpp"

#include <xpcc_git_info.hpp>		// needs env.GitInfoHeader() in SConstruct
#include <xpcc_build_info.hpp>		// needs env.BuildInfoHeader() in SConstruct

xpcc::IODeviceWrapper<
	Hardware::DebugCommunication,
	xpcc::IOBuffer::BlockIfFull> loggerDevice;	// TODO: change to discard
xpcc::log::Logger xpcc::log::debug(loggerDevice);
xpcc::log::Logger xpcc::log::info(loggerDevice);
xpcc::log::Logger xpcc::log::warning(loggerDevice);
xpcc::log::Logger xpcc::log::error(loggerDevice);

MAIN_FUNCTION
{
	Hardware::initialize();

	// print project information
	XPCC_LOG_INFO << "[log-start] " XPCC_BUILD_PROJECT_NAME << xpcc::endl;
	XPCC_LOG_INFO << "[build] " __DATE__            " @ " __TIME__           << xpcc::endl;
	XPCC_LOG_INFO << "[build] " XPCC_BUILD_USER     " @ " XPCC_BUILD_MACHINE << xpcc::endl;
	XPCC_LOG_INFO << "[build] " XPCC_BUILD_COMPILER " @ " XPCC_BUILD_OS      << xpcc::endl;
	XPCC_LOG_INFO << "[git] " XPCC_GIT_SHA_ABBR " "  XPCC_GIT_SUBJECT          << xpcc::endl;
	XPCC_LOG_INFO << "[git] " XPCC_GIT_AUTHOR   " <" XPCC_GIT_AUTHOR_EMAIL ">" << xpcc::endl;
}
