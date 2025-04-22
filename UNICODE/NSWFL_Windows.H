///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_WINDOWS_H_
#define _NSWFL_WINDOWS_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <shtypes.h>

namespace NSWFL {
	namespace Windows {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		enum OpenOrSaveDialogType { //For OSFileDialog(...)
			OFD = 1, //Open File Dialog
			SFD = 2  //Save File Dialog
		};

#define SHELL_FOLDERS_KEY _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders")

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CopyToClipboard(WCHAR* str);

		bool EnableDialogSystemMenu(HWND hWnd, bool bState);
		bool EnableDialogCloseButton(HWND hWnd, bool bState);

		void TextBoxInsert(HWND hWnd, const WCHAR* sInBuf);
		void TextBoxWrite(HWND hWnd, const WCHAR* sInBuf);
		void CopyTextBoxToClipboard(HWND hWnd);

		void MsgBox(const int iValue);
		void MsgBox(const float fValue);
		void MsgBox(const double dValue);
		void MsgBox(const WCHAR* sValue);
		void MsgBox(const __int64 i64Value);

		int MsgBoxF(HWND hWnd, const WCHAR* sCaption, DWORD dwFlags, const WCHAR* sFormat, ...);
		int MsgBoxF(HWND hWnd, const WCHAR* sCaption, const WCHAR* sFormat, ...);
		int MsgBoxF(HWND hWnd, const WCHAR* sFormat, ...);
		int MsgBoxF(const WCHAR* sFormat, ...);

		int PropPageMsg(HWND hWnd, const WCHAR* sText, const WCHAR* sTitle);
		int PropPageMsg(HWND hWnd, const WCHAR* sText, const WCHAR* sTitle, int iMsgBoxStyle);

		void ErrorMessage(HWND hWnd, const WCHAR* sInBuf);
		void AlertMessage(HWND hWnd, const WCHAR* sInBuf, const WCHAR* sTitle);

		int Get_TextLength(HWND hWnd);
		long Get_Long(HWND hWnd);
		__int64 Get_Int64(HWND hWnd);
		float Get_Float(HWND hWnd);
		double Get_Double(HWND hWnd);
		int Get_Int(HWND hWnd);

		void Set_Long(HWND hWnd, long lInput);
		void Set_LongFormatted(HWND hWnd, long lInput);
		void Set_Double(HWND hWnd, double dInput, int iDecimals);
		void Set_Float(HWND hWnd, float fInput, int iDecimals);
		void Set_DoubleFormatted(HWND hWnd, double dInput, int iDecimals);
		void Set_FloatFormatted(HWND hWnd, float fInput, int iDecimals);

		void Set_Int(HWND hWnd, int iInput);
		void Set_Text(HWND hWnd, const WCHAR* sInBuf);
		int Set_TextF(HWND hWnd, const WCHAR* sFormat, ...);
		int Get_Text(HWND hWnd, WCHAR* sOutBuf, int iMaxSize);
		int Get_Text(HWND hWnd, WCHAR*& sOutBuf);
		void CenterWindow(HWND hWnd);
		void CenterOverWindow(HWND hWnd, HWND hOtherWindow);
		void CenterOverOwner(HWND hWnd);
		LRESULT CALLBACK Set_Color(int iFGColor, int iBKColor, WPARAM wParam);
		bool IsMouseOverHwnd(HWND hWnd);
		int OSFileDialog(HWND hOwner, WCHAR* sInOutFileName, int iMaxOutBufSz, WCHAR* sInDfltExt,
			WCHAR* sInTitle, WCHAR* sInFilters, WCHAR* sInDfltDir,
			NSWFL::Windows::OpenOrSaveDialogType iDlgType, DWORD dwFlags);
		bool BrowseDirectory(HWND hOwner, WCHAR* sInTitle, unsigned long ulFlags, WCHAR* sOutBuf, int iMaxOutBufSz);
		bool BrowseDirectory(HWND hOwner, WCHAR* sInTitle, unsigned long ulFlags, WCHAR* sOutBuf, int iMaxOutBufSz, WCHAR* sInitialDirectory);
		bool BrowseDirectory(HWND hOwner, WCHAR* sInTitle, unsigned long ulFlags, WCHAR* sOutBuf, int iMaxOutBufSz, WCHAR* sInitialDirectory, const WCHAR* sRoot);

		LPITEMIDLIST ConvertPathToLpItemIdList(const WCHAR* pszPath);
		HFONT MakeFont(WCHAR* sFontName, int iPointSize);
		void SetFont(HWND hWnd, HFONT hFont);
		bool WaitOnWindow(HWND& hWnd);
		bool WaitOnWindow(HWND& hWnd, int iMilliSeconds);
		bool IsMouseOverWindow(HWND hWnd);
		HWND GetWindowUnderPoint(void);
		void ShowWindowInTaskbar(HWND hWnd, bool bShow);

		bool SelectComboItem(HWND hCombo, const WCHAR* sText, int iMatchLength);
		bool SelectComboItem(HWND hCombo, const WCHAR* sText);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Windows
} //namespace::NSWFL
#endif
