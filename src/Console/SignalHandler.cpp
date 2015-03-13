/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <iostream>

#include "SignalHandler.hpp"

using namespace std;
using namespace geogen;
using namespace console;

static bool abortFlag = false;
static bool ignoreNext = false;

#ifdef _WIN32
bool geogen::console::HandleCtrlEvent(DWORD event)
{
	if (event == CTRL_C_EVENT)
	{
		if (ignoreNext)
		{
			ignoreNext = false;
			return true;
		}

		abortFlag = true;
		return true;
	}

	return false;
}
#else
void geogen::console::HandleIntSignal(int sig)
{
	if (ignoreNext)
	{
		ignoreNext = false;
		return true;
	}

	abortFlag = true;
	return true;
}
#endif

void geogen::console::IgnoreNextSignal()
{
	bool current = GetAndClearAbortFlag();
	if (!current)
	{
		ignoreNext = true;
	}
}

bool geogen::console::GetAndClearAbortFlag()
{
	bool value = abortFlag;
	abortFlag = false;
	return value;
}