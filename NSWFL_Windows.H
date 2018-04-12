///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
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

#define SHELL_FOLDERS_KEY "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CopyToClipboard(char *str);

		bool EnableDialogSystemMenu(HWND hWnd, bool bState);
		bool EnableDialogCloseButton(HWND hWnd, bool bState);

		void TextBoxInsert(HWND hWnd, const char *sInBuf);
		void TextBoxWrite(HWND hWnd, const char *sInBuf);
		void CopyTextBoxToClipboard(HWND hWnd);

		void MsgBox(const int iValue);
		void MsgBox(const float fValue);
		void MsgBox(const double dValue);
		void MsgBox(const char *sValue);
		void MsgBox(const __int64 i64Value);

		int MsgBoxF(HWND hWnd, const char *sCaption, DWORD dwFlags, const char *sFormat, ...);
		int MsgBoxF(HWND hWnd, const char *sCaption, const char *sFormat, ...);
		int MsgBoxF(HWND hWnd, const char *sFormat, ...);
		int MsgBoxF(const char *sFormat, ...);

		int PropPageMsg(HWND hWnd, const char *sText, const char *sTitle);
		int PropPageMsg(HWND hWnd, const char *sText, const char *sTitle, int iMsgBoxStyle);

		void ErrorMessage(HWND hWnd, const char *sInBuf);
		void AlertMessage(HWND hWnd, const char *sInBuf, const char *sTitle);

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
		void Set_Text(HWND hWnd, const char *sInBuf);
		int Set_TextF(HWND hWnd, const char *sFormat, ...);
		int Get_Text(HWND hWnd, char *sOutBuf, int iMaxSize);
		int Get_Text(HWND hWnd, char *&sOutBuf);
		void CenterWindow(HWND hWnd);
		void CenterOverWindow(HWND hWnd, HWND hOtherWindow);
		void CenterOverOwner(HWND hWnd);
		LRESULT CALLBACK Set_Color(int iFGColor, int iBKColor, WPARAM wParam);
		bool IsMouseOverHwnd(HWND hWnd);
		int OSFileDialog(HWND hOwner, char *sInOutFileName, int iMaxOutBufSz, char *sInDfltExt,
			char *sInTitle, char *sInFilters, char *sInDfltDir,
			NSWFL::Windows::OpenOrSaveDialogType iDlgType, DWORD dwFlags);
		bool BrowseDirectory(HWND hOwner, char *sInTitle, unsigned long ulFlags, char *sOutBuf, int iMaxOutBufSz);
		bool BrowseDirectory(HWND hOwner, char *sInTitle, unsigned long ulFlags, char *sOutBuf, int iMaxOutBufSz, char *sInitialDirectory);
		bool BrowseDirectory(HWND hOwner, char *sInTitle, unsigned long ulFlags, char *sOutBuf, int iMaxOutBufSz, char *sInitialDirectory, const char *sRoot);

		LPITEMIDLIST ConvertPathToLpItemIdList(const char *pszPath);
		HFONT MakeFont(char *sFontName, int iPointSize);
		void SetFont(HWND hWnd, HFONT hFont);
		bool WaitOnWindow(HWND &hWnd);
		bool WaitOnWindow(HWND &hWnd, int iMilliSeconds);
		bool IsMouseOverWindow(HWND hWnd);
		HWND GetWindowUnderPoint(void);
		void ShowWindowInTaskbar(HWND hWnd, bool bShow);

		bool SelectComboItem(HWND hCombo, const char *sText, int iMatchLength);
		bool SelectComboItem(HWND hCombo, const char *sText);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Windows
} //namespace::NSWFL
#endif
