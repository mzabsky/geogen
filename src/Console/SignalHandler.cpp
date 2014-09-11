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