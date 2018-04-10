///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_DEBUG_CPP_
#define _NSWFL_DEBUG_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace Debug {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		#ifdef _DEBUG
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		#include <Windows.H>
		#include <StdIO.H>
		#include <fcntl.h>
		#include "IO.H"
		#include "Debug.H"

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		FILE *hRedirectedOutputHandle = NULL;
		FILE *hRedirectedErrorHandle = NULL;
		FILE *hRedirectedInputHandle = NULL;
		bool bIsConsoleOpen = false;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
		AllocConsole();
		RedirectIO(stdin, GetStdHandle(STD_INPUT_HANDLE));
		RedirectIO(stdout, GetStdHandle(STD_OUTPUT_HANDLE));
		RedirectIO(stderr, GetStdHandle(STD_OUTPUT_HANDLE));
		*/

		void RedirectIO(FILE *hFrom, HANDLE hTo)
		{
			int fd = _open_osfhandle((intptr_t)hTo, _O_WRONLY | _O_TEXT);
			_dup2(fd, _fileno(hFrom));
			setvbuf(hFrom, NULL, _IONBF, 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD WINAPI DebugConsoleThread(LPVOID pVoid)
		{
			_DEBUG_THREAD_DATA *data = (_DEBUG_THREAD_DATA *)pVoid;
			DebugConsoleHandler handler = (DebugConsoleHandler)data->Handler;
			SetEvent(data->hEvent);

			char sConsolBuffer[5120];

			while (true)
			{
				printf("\n>");
				gets_s(sConsolBuffer, sizeof(sConsolBuffer));
				int iLength = (int)strlen(sConsolBuffer);
				if (iLength > 0)
				{
					if (!handler(sConsolBuffer, iLength))
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
		bool OpenDebugConsole(const char *sTitle, DebugConsoleHandler pHandler)
		{
			return OpenDebugConsole(sTitle, pHandler, NULL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Open read/write console with a console input handler. If the handler returns false, the console is closed.
		bool OpenDebugConsole(const char *sTitle, DebugConsoleHandler pHandler, char *sInitialText)
		{
			if (AllocConsole())
			{
				bIsConsoleOpen = true;

				SetConsoleTitle(sTitle);
				DebugRedirectOutput();

				if (pHandler)
				{
					DebugRedirectInput();

					if (sInitialText)
					{
						printf("%s", sInitialText);
					}

					_DEBUG_THREAD_DATA data;
					memset(&data, 0, sizeof(data));

					data.Handler = pHandler;

					char sEventName[255];
					sprintf_s(sEventName, sizeof(sEventName), "DCT_%d", GetTickCount());
					data.hEvent = CreateEvent(NULL, FALSE, FALSE, sEventName);
					CreateThread(0, NULL, DebugConsoleThread, &data, 0, NULL);
					WaitForSingleObject(data.hEvent, INFINITE);
				}

				return true;
			}

			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Open write-only console.
		bool OpenDebugConsole(const char *sTitle)
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
		#endif
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Debug
} //namespace::NSWFL
#endif
