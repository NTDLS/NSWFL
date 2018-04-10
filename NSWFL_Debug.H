///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_DEBUG_H_
#define _NSWFL_DEBUG_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace Debug {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		#ifdef _DEBUG
		#define Assert(sExpression, sMsg) \
			if(sExpression)\
			{ \
				char sAssert[2048]; \
				sprintf_s(sAssert, sizeof(sAssert), \
					"A fatal error has occured and the application must terminate.\r\n" \
					"\tPlease take note of the following error message and conditions.\t\n\r\n" \
					"Procedure: %s\r\n" \
					"Source File: %s\r\n" \
					"Build Date/Time: %s (%s)\r\n" \
					"Expression: \"%s\"\r\n" \
					"Line: %d\r\n\r\n" \
					"%s", \
					__FUNCTION__, __FILE__, __DATE__, __TIME__, #sExpression, __LINE__, sMsg); \
				MessageBox(NULL, sAssert, "Fatal Application Error!", MB_ICONSTOP | MB_SYSTEMMODAL); \
			}

		typedef bool(*DebugConsoleHandler)(const char *sText, int iTextLength);

		void RedirectIO(FILE *hFrom, HANDLE hTo);
		bool OpenDebugConsole(const char *sTitle, DebugConsoleHandler pHandler);
		bool OpenDebugConsole(const char *sTitle, DebugConsoleHandler pHandler, char *sInitialText);
		bool OpenDebugConsole(const char *sTitle);
		void DebugRedirectOutput(void);
		void DebugRedirectInput(void);
		void CloseDebugConsole(void);

		typedef struct _tag_DEBUG_THREAD_DATA {
			HANDLE hEvent;
			DebugConsoleHandler Handler;
		} _DEBUG_THREAD_DATA, *_LPDEBUG_THREAD_DATA;

		#else
		#define Assert(sExpression, sMsg)
		#define OpenDebugConsole()
		#define CloseDebugConsole()
		#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Debug
} //namespace::NSWFL
#endif
