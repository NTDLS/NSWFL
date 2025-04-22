///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_WINDOWS_CPP_
#define _NSWFL_WINDOWS_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#include <shlobj.h>

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace Windows {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CopyToClipboard(WCHAR* str)
		{
			OpenClipboard(NULL);
			EmptyClipboard();

			size_t iLength = wcslen(str);

			HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, iLength + 1);
			if (!hg)
			{
				CloseClipboard();
				return;
			}

			memcpy(GlobalLock(hg), str, iLength + 1);
			GlobalUnlock(hg);
			SetClipboardData(CF_TEXT, hg);
			CloseClipboard();
			GlobalFree(hg);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool EnableDialogSystemMenu(HWND hWnd, bool bState)
		{
			if (bState)
			{
				return(SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & WS_SYSMENU) != 0);
			}
			else {
				return(SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SYSMENU) != 0);
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool EnableDialogCloseButton(HWND hWnd, bool bState)
		{
			HMENU hMenu;
			UINT dwExtra = bState ? MF_ENABLED : (MF_DISABLED | MF_GRAYED);
			if ((hMenu = GetSystemMenu(hWnd, FALSE)) == NULL)
			{
				return FALSE;
			}

			return EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | dwExtra) != -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CopyTextBoxToClipboard(HWND hWnd)
		{
			SendMessage(hWnd, EM_SETSEL, (WPARAM)0, (LPARAM)-1);
			SendMessage(hWnd, WM_COPY, (WPARAM)0, (LPARAM)0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void TextBoxInsert(HWND hWnd, const WCHAR* sInBuf)
		{
			LRESULT iLen = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
			SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM)(LPSTR)sInBuf);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void TextBoxWrite(HWND hWnd, const WCHAR* sInBuf)
		{
			LRESULT iLen = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
			SendMessage(hWnd, EM_SETSEL, (WPARAM)iLen, (LPARAM)iLen);
			SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM)(LPSTR)sInBuf);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void MsgBox(const WCHAR* sValue)
		{
			MessageBox(GetActiveWindow(), sValue, _T("Message"), MB_TASKMODAL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int MsgBoxF(HWND hWnd, const WCHAR* sCaption, DWORD dwFlags, const WCHAR* sFormat, ...)
		{
			va_list ArgList;
			va_start(ArgList, sFormat);

			int iMemoryRequired = _vscwprintf(sFormat, ArgList);

#ifdef _USE_GLOBAL_MEMPOOL
			WCHAR* sBuf = (WCHAR*)pMem->Allocate(sizeof(WCHAR), iMemoryRequired + 1);
#else
			WCHAR* sBuf = (WCHAR*)calloc(sizeof(WCHAR), iMemoryRequired + 1);
#endif

			int iSz = _vswprintf_s_l(sBuf, iMemoryRequired + 1, sFormat, NULL, ArgList);
			va_end(ArgList);

			int iResult = MessageBox(hWnd, sBuf, sCaption, dwFlags);

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sBuf);
#else
			free(sBuf);
#endif

			return iResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int MsgBoxF(HWND hWnd, const WCHAR* sCaption, const WCHAR* sFormat, ...)
		{
			va_list ArgList;
			va_start(ArgList, sFormat);

			int iMemoryRequired = _vscwprintf(sFormat, ArgList);

#ifdef _USE_GLOBAL_MEMPOOL
			WCHAR* sBuf = (WCHAR*)pMem->Allocate(sizeof(WCHAR), iMemoryRequired + 1);
#else
			WCHAR* sBuf = (WCHAR*)calloc(sizeof(WCHAR), iMemoryRequired + 1);
#endif

			int iSz = _vswprintf_s_l(sBuf, iMemoryRequired + 1, sFormat, NULL, ArgList);
			va_end(ArgList);

			int iResult = MessageBox(hWnd, sBuf, sCaption, MB_TASKMODAL);

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sBuf);
#else
			free(sBuf);
#endif

			return iResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int MsgBoxF(HWND hWnd, const WCHAR* sFormat, ...)
		{
			va_list ArgList;
			va_start(ArgList, sFormat);

			int iMemoryRequired = _vscwprintf(sFormat, ArgList);

#ifdef _USE_GLOBAL_MEMPOOL
			WCHAR* sBuf = (WCHAR*)pMem->Allocate(sizeof(WCHAR), iMemoryRequired + 1);
#else
			WCHAR* sBuf = (WCHAR*)calloc(sizeof(WCHAR), iMemoryRequired + 1);
#endif

			int iSz = _vswprintf_s_l(sBuf, iMemoryRequired + 1, sFormat, NULL, ArgList);
			va_end(ArgList);

			int iResult = MessageBox(hWnd, sBuf, _T("Message"), MB_TASKMODAL);

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sBuf);
#else
			free(sBuf);
#endif

			return iResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int MsgBoxF(const WCHAR* sFormat, ...)
		{
			va_list ArgList;
			va_start(ArgList, sFormat);

			int iMemoryRequired = _vscwprintf(sFormat, ArgList);

#ifdef _USE_GLOBAL_MEMPOOL
			WCHAR* sBuf = (WCHAR*)pMem->Allocate(sizeof(WCHAR), iMemoryRequired + 1);
#else
			WCHAR* sBuf = (WCHAR*)calloc(sizeof(WCHAR), iMemoryRequired + 1);
#endif

			int iSz = _vswprintf_s_l(sBuf, iMemoryRequired + 1, sFormat, NULL, ArgList);
			va_end(ArgList);

			int iResult = MessageBox(GetActiveWindow(), sBuf, _T("Message"), MB_TASKMODAL);

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sBuf);
#else
			free(sBuf);
#endif

			return iResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void MsgBox(const __int64 i64Value)
		{
			WCHAR sValue[64];
			swprintf_s(sValue, sizeof(sValue), _T("%I64i"), i64Value);
			MessageBox(GetActiveWindow(), sValue, _T("Message"), MB_TASKMODAL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void MsgBox(const int iValue)
		{
			WCHAR sValue[64];
			swprintf_s(sValue, sizeof(sValue), _T("%d"), iValue);
			MessageBox(GetActiveWindow(), sValue, _T("Message"), MB_TASKMODAL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void MsgBox(const float fValue)
		{
			WCHAR sValue[1024];
			swprintf_s(sValue, sizeof(sValue), _T("%f"), fValue);
			MessageBox(GetActiveWindow(), sValue, _T("Message"), MB_TASKMODAL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void MsgBox(const double dValue)
		{
			WCHAR sValue[1024];
			swprintf_s(sValue, sizeof(sValue), _T("%f"), dValue);
			MessageBox(GetActiveWindow(), sValue, _T("Message"), MB_TASKMODAL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void ErrorMessage(HWND hWnd, const WCHAR* sInBuf)
		{
			HWND hOwner = NULL;

			if (hWnd == NULL)
			{
				hOwner = GetActiveWindow();
			}
			else hOwner = hWnd;

			MessageBox(hOwner, sInBuf, _T("Error"), MB_ICONHAND + MB_SYSTEMMODAL + MB_TASKMODAL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void AlertMessage(HWND hWnd, const WCHAR* sInBuf, const WCHAR* sTitle)
		{
			HWND hOwner = NULL;

			if (hWnd == NULL)
			{
				hOwner = GetActiveWindow();
			}
			else hOwner = hWnd;

			MessageBox(hOwner, sInBuf, sTitle, MB_SYSTEMMODAL + MB_TASKMODAL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Get_TextLength(HWND hWnd)
		{
			return GetWindowTextLength(hWnd);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		double Get_Double(HWND hWnd)
		{
			WCHAR sBuf[16];
			NSWFL::Windows::Get_Text(hWnd, sBuf, sizeof(sBuf));
			return NSWFL::Conversion::dFormattedValue(sBuf);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		float Get_Float(HWND hWnd)
		{
			return (float)Get_Double(hWnd);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		__int64 Get_Int64(HWND hWnd)
		{
			WCHAR sBuf[16];
			NSWFL::Windows::Get_Text(hWnd, sBuf, sizeof(sBuf));
			return NSWFL::Conversion::i64FormattedValue(sBuf);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		long Get_Long(HWND hWnd)
		{
			WCHAR sBuf[16];
			NSWFL::Windows::Get_Text(hWnd, sBuf, sizeof(sBuf));
			return (long)NSWFL::Conversion::i64FormattedValue(sBuf);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Get_Int(HWND hWnd)
		{
			return (int)NSWFL::Windows::Get_Long(hWnd);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Set_LongFormatted(HWND hWnd, long lInput)
		{
			WCHAR sBuf[64];
			NSWFL::Conversion::FormatInteger(sBuf, sizeof(sBuf), lInput);
			NSWFL::Windows::Set_Text(hWnd, sBuf);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Set_Long(HWND hWnd, long lInput)
		{
			WCHAR sBuf[64];
			swprintf_s(sBuf, sizeof(sBuf), _T("%d"), lInput);
			NSWFL::Windows::Set_Text(hWnd, sBuf);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Set_Double(HWND hWnd, double dInput, int iDecimals)
		{
			WCHAR sFmt[64];
			swprintf_s(sFmt, sizeof(sFmt), _T("%%.%df"), iDecimals);

			WCHAR sBuf[256];
			swprintf_s(sBuf, sizeof(sBuf), sFmt, dInput);
			NSWFL::Windows::Set_Text(hWnd, sBuf);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Set_Float(HWND hWnd, float fInput, int iDecimals)
		{
			WCHAR sFmt[64];
			swprintf_s(sFmt, sizeof(sFmt), _T("%%.%df"), iDecimals);

			WCHAR sBuf[256];
			swprintf_s(sBuf, sizeof(sBuf), sFmt, fInput);
			NSWFL::Windows::Set_Text(hWnd, sBuf);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Set_DoubleFormatted(HWND hWnd, double dInput, int iDecimals)
		{
			WCHAR sNumber[256];
			NSWFL::Conversion::FormatDouble(sNumber, sizeof(sNumber), dInput, iDecimals);
			NSWFL::Windows::Set_Text(hWnd, sNumber);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Set_FloatFormatted(HWND hWnd, float fInput, int iDecimals)
		{
			WCHAR sNumber[256];
			NSWFL::Conversion::FormatFloat(sNumber, sizeof(sNumber), fInput, iDecimals);
			NSWFL::Windows::Set_Text(hWnd, sNumber);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Set_Int(HWND hWnd, int iInput)
		{
			NSWFL::Windows::Set_Long(hWnd, (DWORD)iInput);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Set_Text(HWND hWnd, const WCHAR* sInBuf)
		{
			SendMessage(hWnd, (UINT)WM_SETTEXT, (WPARAM)0, (LPARAM)sInBuf);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Set_TextF(HWND hWnd, const WCHAR* sFormat, ...)
		{
			va_list ArgList;
			va_start(ArgList, sFormat);

			int iMemoryRequired = _vscwprintf(sFormat, ArgList);

#ifdef _USE_GLOBAL_MEMPOOL
			WCHAR* sBuf = (WCHAR*)pMem->Allocate(sizeof(WCHAR), iMemoryRequired + 1);
#else
			WCHAR* sBuf = (WCHAR*)calloc(sizeof(WCHAR), iMemoryRequired + 1);
#endif

			int iSz = _vswprintf_s_l(sBuf, iMemoryRequired + 1, sFormat, NULL, ArgList);
			va_end(ArgList);

			Set_Text(hWnd, sBuf);

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sBuf);
#else
			free(sBuf);
#endif

			return iSz;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//The pointer returned through [sOutBuf] must be freed externally.
		int Get_Text(HWND hWnd, WCHAR*& sOutBuf)
		{
			int iLength = NSWFL::Windows::Get_TextLength(hWnd);
			if (iLength > 0)
			{
#ifdef _USE_GLOBAL_MEMPOOL
				sOutBuf = (WCHAR*)pMem->Allocate(sizeof(WCHAR), iLength + 1);
#else
				sOutBuf = (WCHAR*)calloc(sizeof(WCHAR), iLength + 1);
#endif

				return GetWindowText(hWnd, sOutBuf, iLength + 1);
			}
			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Get_Text(HWND hWnd, WCHAR* sOutBuf, int iMaxSize)
		{
			int iLen = GetWindowTextLength(hWnd);

			if (iLen >= iMaxSize)
			{
				return iLen;
			}

			iLen = GetWindowText(hWnd, sOutBuf, iMaxSize);

			sOutBuf[iLen] = '\0';

			return iLen;
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CenterOverOwner(HWND hWnd)
		{
			NSWFL::Windows::CenterOverWindow(hWnd, GetParent(hWnd));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CenterOverWindow(HWND hWnd, HWND hOtherWindow)
		{
			if (hOtherWindow == NULL)
			{
				NSWFL::Windows::CenterWindow(hWnd);
				return;
			}

			int iTaksbarOffsetX = 0;
			int iTaksbarOffsetY = 0;

			int iScreenX = GetSystemMetrics(SM_CXSCREEN) - 10;
			int iScreenY = (GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYCAPTION)) - 10;

			MONITORINFO MonitorInfo;
			memset(&MonitorInfo, 0, sizeof(MonitorInfo));

			HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
			if (hMonitor)
			{
				MonitorInfo.cbSize = sizeof(MonitorInfo);

				if (GetMonitorInfo(hMonitor, &MonitorInfo))
				{
					iScreenX = MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left;
					iScreenY = MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top;

					//Figure the height/width of the taskbar.
					int iTaskScreenX = GetSystemMetrics(SM_CXSCREEN);
					int iTaskScreenY = (GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYCAPTION));
					int iTaskFullScreenX = GetSystemMetrics(SM_CXFULLSCREEN);
					int iTaskFullScreenY = (GetSystemMetrics(SM_CYFULLSCREEN) - GetSystemMetrics(SM_CYCAPTION));

					iTaksbarOffsetX = abs(iTaskScreenX - iTaskFullScreenX);
					iTaksbarOffsetY = abs(iTaskScreenY - iTaskFullScreenY);
				}
				else {
					memset(&MonitorInfo, 0, sizeof(MonitorInfo));
				}
			}

			RECT recOther;
			memset(&recOther, 0, sizeof(recOther));
			GetWindowRect(hOtherWindow, &recOther);

			RECT recDlg;
			memset(&recDlg, 0, sizeof(recDlg));
			GetWindowRect(hWnd, &recDlg);

			int iX = ((recOther.right - ((recOther.right - recOther.left) / 2)) - ((recDlg.right - recDlg.left) / 2));
			int iY = ((recOther.bottom - ((recOther.bottom - recOther.top) / 2)) - ((recDlg.bottom - recDlg.top) / 2));

			if (iX - MonitorInfo.rcMonitor.left > (iScreenX - (recDlg.right - recDlg.left)) - iTaksbarOffsetX)
			{
				iX = ((MonitorInfo.rcMonitor.left + (iScreenX - (recDlg.right - recDlg.left))) - iTaksbarOffsetX) - 10;
			}
			else if (iX - MonitorInfo.rcMonitor.left < 10)
			{
				iX = (MonitorInfo.rcMonitor.left + 10);
			}

			if (iY - MonitorInfo.rcMonitor.top > (iScreenY - (recDlg.bottom - recDlg.top)) - iTaksbarOffsetY)
			{
				iY = (MonitorInfo.rcMonitor.top + (iScreenY - (recDlg.bottom - recDlg.top)) - iTaksbarOffsetY) - 10;
			}
			else if (iY - MonitorInfo.rcMonitor.top < 10)
			{
				iY = MonitorInfo.rcMonitor.top + 10;
			}

			SetWindowPos(hWnd, NULL, iX, iY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CenterWindow(HWND hWnd)
		{
			POINT MousePos;
			memset(&MousePos, 0, sizeof(MousePos));

			int iScreenX = GetSystemMetrics(SM_CXSCREEN);
			int iScreenY = GetSystemMetrics(SM_CYSCREEN);

			int MonitorOffsetX = 0;
			int MonitorOffsetY = 0;

			if (GetCursorPos(&MousePos))
			{
				MONITORINFO MonitorInfo;
				HMONITOR hMonitor = MonitorFromPoint(MousePos, MONITOR_DEFAULTTONEAREST);
				if (hMonitor)
				{
					memset(&MonitorInfo, 0, sizeof(MonitorInfo));
					MonitorInfo.cbSize = sizeof(MonitorInfo);

					if (GetMonitorInfo(hMonitor, &MonitorInfo))
					{
						MonitorOffsetX = MonitorInfo.rcMonitor.left;
						MonitorOffsetY = MonitorInfo.rcMonitor.top;

						iScreenX = MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left;
						iScreenY = MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top;
					}
				}
			}

			RECT wRect;
			memset(&wRect, 0, sizeof(wRect));

			GetWindowRect(hWnd, &wRect);

			DWORD x = ((iScreenX - (wRect.right - wRect.left)) / 2) + MonitorOffsetX;
			DWORD y = ((iScreenY - (wRect.bottom - wRect.top + GetSystemMetrics(SM_CYCAPTION))) / 2) + MonitorOffsetY;

			SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		LRESULT CALLBACK Set_Color(int iFGColor, int iBKColor, WPARAM wParam)
		{
			static HBRUSH ReUsableBrush;
			DeleteObject(ReUsableBrush);
			ReUsableBrush = CreateSolidBrush(iBKColor);
			SetTextColor((HDC)wParam, iFGColor);
			SetBkColor((HDC)wParam, iBKColor);
			return (LRESULT)ReUsableBrush;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function returns HWND of the child window under the mouse cursor.
				The window must be visible and uncovered.
		*/
		HWND GetWindowUnderPoint(void)
		{
			POINT Pointer;
			memset(&Pointer, 0, sizeof(Pointer));
			if (GetCursorPos(&Pointer))
			{
				return WindowFromPoint(Pointer);
			}
			return NULL;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function returns TRUE if the mouse cursor is over the window specified by HWND.
				The window must be visible and uncovered.
		*/
		bool IsMouseOverWindow(HWND hWnd)
		{
			POINT Pointer;
			memset(&Pointer, 0, sizeof(Pointer));
			GetCursorPos(&Pointer);
			return(WindowFromPoint(Pointer) == hWnd);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function returns TRUE if the mouse cursor is over the window specified by HWND.
				This function does not care if the window is covered, enabled or visible.
		*/
		bool IsMouseOverHwnd(HWND hWnd)
		{
			RECT Rectangle;
			POINT Pointer;
			memset(&Rectangle, 0, sizeof(Rectangle));
			memset(&Pointer, 0, sizeof(Pointer));

			GetWindowRect(hWnd, &Rectangle);
			GetCursorPos(&Pointer);

			return(PtInRect(&Rectangle, Pointer) > 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int OSFileDialog(HWND hOwner, WCHAR* sInOutFileName, int iMaxOutBufSz, WCHAR* sInDfltExt,
			WCHAR* sInTitle, WCHAR* sInFilters, WCHAR* sInDfltDir,
			NSWFL::Windows::OpenOrSaveDialogType iDlgType, DWORD dwFlags)
		{
			WCHAR sFileName[MAX_PATH];
			int iRetVal = 0;
			WCHAR* sCurrDir = NULL;

			OPENFILENAME OFN;

			if ((iDlgType != OFD) && (iDlgType != SFD))
			{
				return 0;
			}

			wcscpy_s(sFileName, sizeof(sFileName), sInOutFileName);

			OFN.lpstrCustomFilter = NULL;
			OFN.nMaxCustFilter = 0;
			OFN.lpstrFileTitle = NULL;
			OFN.nMaxFileTitle = 0;

			if (!sInFilters)
			{
				OFN.lpstrFilter = _T("All-Files\0*.*\0Text-Files\0*.txt\0\0");
			}
			else OFN.lpstrFilter = sInFilters;

			if (!sInDfltExt)
			{
				OFN.lpstrDefExt = _T("txt");
			}
			else OFN.lpstrDefExt = sInDfltExt;

			if (!sInTitle)
			{
				if (iDlgType == OFD)
				{
					OFN.lpstrTitle = _T("Select file to open");
				}
				else if (iDlgType == SFD)
				{
					OFN.lpstrTitle = _T("Save file as");
				}
			}
			else OFN.lpstrTitle = sInTitle;

			if (!sInDfltDir)
			{
#ifdef _USE_GLOBAL_MEMPOOL
				if ((sCurrDir = (WCHAR*)pMem->Allocate(sizeof(WCHAR), MAX_PATH + 1)))
#else
				if ((sCurrDir = (WCHAR*)calloc(sizeof(WCHAR), MAX_PATH + 1)))
#endif
				{
					if (NSWFL::File::Get_CurrentDirectory(sCurrDir, MAX_PATH))
					{
						OFN.lpstrInitialDir = sCurrDir;
					}
				}
			}
			else OFN.lpstrInitialDir = sInDfltDir;

			OFN.lStructSize = sizeof(OFN);
			OFN.hwndOwner = hOwner;
			OFN.nFilterIndex = 1;
			OFN.lpstrFile = sFileName;
			OFN.nMaxFile = sizeof(sFileName);

			if (dwFlags == 0)
			{
				OFN.Flags = OFN_EXPLORER + OFN_LONGNAMES + OFN_PATHMUSTEXIST;
			}
			else OFN.Flags = dwFlags;

			if (iDlgType == OFD)
			{
				iRetVal = GetOpenFileName(&OFN);
			}
			else if (iDlgType == SFD)
			{
				iRetVal = GetSaveFileName(&OFN);
			}

			wcscpy_s(sInOutFileName, iMaxOutBufSz, sFileName);

			if (!sInDfltDir && sCurrDir)
			{
#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(sCurrDir);
#else
				free(sCurrDir);
#endif
			}

			return iRetVal;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CALLBACK BrowseDirectoryCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
		{
			if (uMsg == BFFM_INITIALIZED)
			{
				SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
			}
			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		LPITEMIDLIST ConvertPathToLpItemIdList(const WCHAR* pszPath)
		{
			LPITEMIDLIST  pidl = NULL;
			LPSHELLFOLDER pDesktopFolder = NULL;
			ULONG         chEaten;
			HRESULT       hr;

			if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
			{
				hr = pDesktopFolder->ParseDisplayName(NULL, NULL, const_cast<WCHAR*>(pszPath), &chEaten, &pidl, NULL);
				pDesktopFolder->Release();
				return pidl;
			}

			return NULL;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Assumes that sOutBuf is at least MAX_PATH, because SHGetPathFromIDList does not check for buffer size.
		bool BrowseDirectory(HWND hOwner, WCHAR* sInTitle,
			unsigned long ulFlags, WCHAR* sOutBuf, int iMaxOutBufSz)
		{
			return NSWFL::Windows::BrowseDirectory(hOwner, sInTitle, ulFlags, sOutBuf, iMaxOutBufSz, NULL, NULL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Assumes that sOutBuf is at least MAX_PATH, because SHGetPathFromIDList does not check for buffer size.
		bool BrowseDirectory(HWND hOwner, WCHAR* sInTitle, unsigned long ulFlags, WCHAR* sOutBuf, int iMaxOutBufSz, WCHAR* sInitialDirectory)
		{
			return BrowseDirectory(hOwner, sInTitle, ulFlags, sOutBuf, iMaxOutBufSz, sInitialDirectory, NULL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Assumes that sOutBuf is at least MAX_PATH, because SHGetPathFromIDList does not check for buffer size.
		/*
			BrowseDirectory(hWnd, "Select a folder.", BIF_RETURNONLYFSDIRS|BIF_USENEWUI, sDeleteFolder, sizeof(sDeleteFolder));
		*/
		bool BrowseDirectory(HWND hOwner, WCHAR* sInTitle, unsigned long ulFlags, WCHAR* sOutBuf, int iMaxOutBufSz, WCHAR* sInitialDirectory, const WCHAR* sRoot)
		{
			LPMALLOC pMalloc;
			LPITEMIDLIST lpItemIDList;
			BROWSEINFO  browseInfo;
			bool bResult = false;

			if (iMaxOutBufSz < MAX_PATH)
			{
				return false;
			}

			memset(&pMalloc, 0, sizeof(pMalloc));
			memset(&browseInfo, 0, sizeof(browseInfo));
			memset(&lpItemIDList, 0, sizeof(lpItemIDList));

			if (S_OK != SHGetMalloc(&pMalloc))
			{
				return false;
			}

			memset(&browseInfo, 0, sizeof(BROWSEINFO));
			browseInfo.hwndOwner = hOwner;
			browseInfo.pszDisplayName = sOutBuf;
			browseInfo.lpszTitle = sInTitle;
			browseInfo.ulFlags = ulFlags;

			if (!sRoot)
			{
				browseInfo.pidlRoot = ConvertPathToLpItemIdList(sRoot);
			}

			if (sInitialDirectory)
			{
				browseInfo.lParam = (LPARAM)sInitialDirectory;
				browseInfo.lpfn = BrowseDirectoryCallbackProc;
			}

			lpItemIDList = SHBrowseForFolder(&browseInfo);

			if (lpItemIDList != NULL)
			{
				wcscpy_s(sOutBuf, iMaxOutBufSz, _T(""));

				if (SHGetPathFromIDList(lpItemIDList, sOutBuf))
				{
					if (wcslen(sOutBuf) > 0)
					{
						bResult = true;
					}
				}

				pMalloc->Free(lpItemIDList);
			}
			pMalloc->Release();

			return bResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		HFONT MakeFont(WCHAR* sFontName, int iPointSize)
		{
			HDC hDC;
			int CyPixels = 0;
			hDC = GetDC(HWND_DESKTOP);
			CyPixels = GetDeviceCaps(hDC, LOGPIXELSY);
			ReleaseDC(HWND_DESKTOP, hDC);
			int iiPointSize = (iPointSize * CyPixels) / 72;
			return CreateFont(0 - iiPointSize, 0, 0, 0, FW_NORMAL, 0, 0, 0,
				ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, FF_DONTCARE, sFontName);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void SetFont(HWND hWnd, HFONT hFont)
		{
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//This function will wait for the window specified by hWnd to become visible,
		//	it will then set the window to the foreground and return.
		bool WaitOnWindow(HWND& hWnd)
		{
			while (!hWnd) //Wait on valid handle.
			{
				Sleep(1);
			}

			while (!IsWindowVisible(hWnd)) //Wait on window to be visible.
			{
				Sleep(1);
			}

			if (IsWindow(hWnd))
			{
				SetForegroundWindow(hWnd);
				return true;
			}

			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//This function will wait for a specified amount of time for the
		//	window specified by hWnd to become visible, it will then set
		//	the window to the foreground and return.
		bool WaitOnWindow(HWND& hWnd, int iMilliSeconds)
		{
			while (!hWnd) //Wait on valid handle.
			{
				Sleep(1);
			}

			int iWaited = 0;
			while (!IsWindowVisible(hWnd))
			{
				if (iMilliSeconds > 0)
				{
					if (iWaited++ >= iMilliSeconds)
					{
						return false;
					}
				}
				Sleep(1);
			}

			if (IsWindow(hWnd))
			{
				SetForegroundWindow(hWnd);
				return true;
			}

			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void ShowWindowInTaskbar(HWND hWnd, bool bShow)
		{
			DWORD dwCurrentStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
			if (bShow)
			{
				if (!(dwCurrentStyle & WS_EX_APPWINDOW))
				{
					SetWindowLong(hWnd, GWL_EXSTYLE, dwCurrentStyle | WS_EX_APPWINDOW);
				}
			}
			else {
				if ((dwCurrentStyle & WS_EX_APPWINDOW))
				{
					SetWindowLong(hWnd, GWL_EXSTYLE, (GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_APPWINDOW));
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool SelectComboItem(HWND hCombo, const WCHAR* sText, int iMatchLength)
		{
			LRESULT iIndex = -1;

			if (iMatchLength <= 0)
			{
				iIndex = SendMessage(hCombo, (UINT)CB_FINDSTRING, (WPARAM)-1, (LPARAM)sText);
			}
			else {
#ifdef _USE_GLOBAL_MEMPOOL
				WCHAR* sShortText = (WCHAR*)pMem->Allocate(iMatchLength + 1, sizeof(WCHAR));
#else
				WCHAR* sShortText = (WCHAR*)calloc(iMatchLength + 1, sizeof(WCHAR));
#endif

				wcsncpy_s(sShortText, iMatchLength + 1, sText, iMatchLength);
				iIndex = SendMessage(hCombo, (UINT)CB_FINDSTRING, (WPARAM)-1, (LPARAM)sShortText);
#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(sShortText);
#else
				free(sShortText);
#endif
			}

			if (iIndex >= 0)
			{
				return(SendMessage(hCombo, (UINT)CB_SETCURSEL, (WPARAM)iIndex, (LPARAM)NULL) == iIndex);
			}

			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool SelectComboItem(HWND hCombo, const WCHAR* sText)
		{
			return SelectComboItem(hCombo, sText, -1);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function is used to show a validation message box on a Property Sheet control.

			If will move the focus to the complaining tab and stop the apply process.

			the Property sheet callback procedure should return TRUE if you call this function.

		*/
		int PropPageMsg(HWND hWnd, const WCHAR* sText, const WCHAR* sTitle, int iMsgBoxStyle)
		{
			SendMessage(GetParent(hWnd), PSM_SETCURSEL, NULL, (LPARAM)hWnd);
			int iResult = MessageBox(hWnd, sText, sTitle, iMsgBoxStyle);
			SetWindowLongPtr(hWnd, DWLP_MSGRESULT, PSNRET_INVALID_NOCHANGEPAGE);
			return iResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			See [PropPageMsg] overload for notes.
		*/
		int PropPageMsg(HWND hWnd, const WCHAR* sText, const WCHAR* sTitle)
		{
			return PropPageMsg(hWnd, sText, sTitle, MB_ICONINFORMATION);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Windows
} //namespace::NSWFL
#endif
