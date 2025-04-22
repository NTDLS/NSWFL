///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
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
				WCHAR sAssert[2048]; \
				swprintf_s(sAssert, _countof(sAssert), \
					_T("A fatal error has occurred and the application must terminate.\r\n") \
					_T("\tPlease take note of the following error message and conditions.\t\n\r\n") \
					_T("Procedure: %ws\r\n") \
					_T("Source File: %ws\r\n") \
					_T("Build Date/Time: %ws (%ws)\r\n") \
					_T("Expression: \"%ws\"\r\n") \
					_T("Line: %d\r\n\r\n") \
					_T("%ws"), \
					_T(__FUNCTION__), _T(__FILE__), _T(__DATE__), _T(__TIME__), _T(#sExpression), __LINE__, _T(sMsg)); \
				MessageBox(NULL, sAssert, _T("Fatal Application Error!"), MB_ICONSTOP | MB_SYSTEMMODAL); \
			}
#else
#define Assert(sExpression, sMsg)
#endif

		typedef bool(*DebugConsoleHandler)(const WCHAR* sText, int iTextLength);

		void RedirectIO(FILE* hFrom, HANDLE hTo);
		bool OpenDebugConsole(const WCHAR* sTitle, DebugConsoleHandler pHandler);
		bool OpenDebugConsole(const WCHAR* sTitle, DebugConsoleHandler pHandler, WCHAR* sInitialText);
		void StartDebugConsoleHandler(DebugConsoleHandler pHandler);
		bool OpenDebugConsole(const WCHAR* sTitle);
		void DebugRedirectOutput(void);
		void DebugRedirectInput(void);
		void CloseDebugConsole(void);

		typedef struct _tag_DEBUG_THREAD_DATA {
			HANDLE hEvent;
			DebugConsoleHandler Handler;
		} _DEBUG_THREAD_DATA, * _LPDEBUG_THREAD_DATA;


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Debug
} //namespace::NSWFL
#endif
