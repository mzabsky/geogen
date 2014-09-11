#pragma once

#include <signal.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace geogen
{
	namespace console
	{
#ifdef _WIN32
		bool HandleCtrlEvent(DWORD event);

		static void InitializeSignalHandler()
		{
			SetConsoleCtrlHandler((PHANDLER_ROUTINE)HandleCtrlEvent, TRUE);
		}
#else
		void HandleIntSignal(int sig);

		static void InitializeSignalHandler()
		{
			signal(SIGINT, HandleIntSignal);
		}
#endif

		void IgnoreNextSignal();

		bool GetAndClearAbortFlag();
	}
}