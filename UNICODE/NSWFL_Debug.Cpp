///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_DEBUG_CPP_
#define _NSWFL_DEBUG_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#include <io.h>
#include <fcntl.h>

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Debug {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		FILE* hRedirectedOutputHandle = NULL;
		FILE* hRedirectedErrorHandle = NULL;
		FILE* hRedirectedInputHandle = NULL;
		bool bIsConsoleOpen = false;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
		AllocConsole();
		RedirectIO(stdin, GetStdHandle(STD_INPUT_HANDLE));
		RedirectIO(stdout, GetStdHandle(STD_OUTPUT_HANDLE));
		RedirectIO(stderr, GetStdHandle(STD_OUTPUT_HANDLE));
		*/

		void RedirectIO(FILE* hFrom, HANDLE hTo)
		{
			int fd = _open_osfhandle((intptr_t)hTo, _O_WRONLY | _O_TEXT);
			_dup2(fd, _fileno(hFrom));
			setvbuf(hFrom, NULL, _IONBF, 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD WINAPI DebugConsoleThread(LPVOID pVoid)
		{
			_DEBUG_THREAD_DATA* data = (_DEBUG_THREAD_DATA*)pVoid;
			DebugConsoleHandler handler = (DebugConsoleHandler)data->Handler;
			SetEvent(data->hEvent);

			WCHAR sConsoleBuffer[5120];

			while (true)
			{
				wprintf(_T("\n>"));
				_getws_s(sConsoleBuffer, _countof(sConsoleBuffer));
				int iLength = (int)wcslen(sConsoleBuffer);
				if (iLength > 0)
				{
					if (!handler(sConsoleBuffer, iLength))
					{
						break;
					}
				}
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void DebugRedirectOutput(void)
		{
			freopen_s(&hRedirectedOutputHandle, "CONOUT$", "w", stdout);
			freopen_s(&hRedirectedErrorHandle, "CONOUT$", "w", stderr);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void DebugRedirectInput(void)
		{
			freopen_s(&hRedirectedInputHandle, "CONIN$", "r", stdin);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Open read/write console with a console input handler. If the handler returns false, the console is closed.
		bool OpenDebugConsole(const WCHAR* sTitle, DebugConsoleHandler pHandler)
		{
			return OpenDebugConsole(sTitle, pHandler, NULL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Open read/write console with a console input handler. If the handler returns false, the console is closed.
		bool OpenDebugConsole(const WCHAR* sTitle, DebugConsoleHandler pHandler, WCHAR* sInitialText)
		{
			bool bIsAreadyOpen = (GetConsoleWindow() != NULL);

			if (bIsAreadyOpen || AllocConsole())
			{
				bIsConsoleOpen = true;

				SetConsoleTitle(sTitle);
				if (bIsAreadyOpen == false)
				{
					DebugRedirectOutput();
				}

				if (sInitialText)
				{
					wprintf(_T("%s"), sInitialText);
				}

				if (bIsAreadyOpen == false)
				{
					DebugRedirectInput();
				}

				if (pHandler)
				{
					StartDebugConsoleHandler(pHandler);
				}

				return true;
			}

			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Allows for interaction with the console.
		void StartDebugConsoleHandler(DebugConsoleHandler pHandler)
		{
			_DEBUG_THREAD_DATA data;
			memset(&data, 0, sizeof(data));

			data.Handler = pHandler;

			WCHAR sEventName[255];
			swprintf_s(sEventName, _countof(sEventName), _T("DCT_%I64i"), GetTickCount64());
			data.hEvent = CreateEvent(NULL, FALSE, FALSE, sEventName);
			CreateThread(0, NULL, DebugConsoleThread, &data, 0, NULL);
			if (data.hEvent != NULL)
			{
				WaitForSingleObject(data.hEvent, INFINITE);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Open write-only console.
		bool OpenDebugConsole(const WCHAR* sTitle)
		{
			return OpenDebugConsole(sTitle, NULL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CloseDebugConsole(void)
		{
			if (hRedirectedOutputHandle != NULL)
			{
				fclose(hRedirectedOutputHandle);
			}

			if (hRedirectedErrorHandle != NULL)
			{
				fclose(hRedirectedErrorHandle);
			}

			if (hRedirectedInputHandle != NULL)
			{
				fclose(hRedirectedInputHandle);
			}

			if (bIsConsoleOpen)
			{
				FreeConsole();
				bIsConsoleOpen = false;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Debug
} //namespace::NSWFL
#endif
