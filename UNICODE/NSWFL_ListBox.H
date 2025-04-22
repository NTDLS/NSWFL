///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_LISTBOX_H_
#define _NSWFL_LISTBOX_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace ListBox {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InsertListBoxItem(HWND hWnd, WCHAR* sInStr, int iMaxItems, int iInsertPos);
		int InsertListBoxItem(HWND hWnd, WCHAR* sInStr, int iInsertPos);
		int ClearListBox(HWND hWnd);
		int DeleteListBoxItem(HWND hWnd, int iItemPos);
		int CountListBoxItems(HWND hWnd);
		int SelectMultipleListBoxItem(HWND hWnd, int iItemPos, bool bSelect);
		int SelectListBoxItem(HWND hWnd, int iItemPos);
		int GetCurrentListBoxItem(HWND hWnd);
		int GetListBoxItemText(WCHAR* sOutVal, int iMaxSz, HWND hWnd, int iItemPos);
		int GetListBoxItemText(WCHAR* sOutVal, int iMaxSz, HWND hWnd);
		int GetListBoxItemLength(HWND hWnd, int iItemPos);
		int SetHorizontalListBoxExtent(HWND hWnd, int iHorzExtent);
		int InsertListBoxItem(HWND hWnd, WCHAR* sInStr);
		WCHAR** ListBoxToCharacterArray(HWND hWnd, int* iOutItemCount);
		int* ListBoxToIntegerArray(HWND hWnd, int* iOutItemCount);
		int DeleteListBoxItem(HWND hWnd);
		int PopListBoxFromIntegerArray(HWND hWnd, int* iItemList, int iItems);
		int PopListBoxFromStringArray(HWND hWnd, WCHAR** sItemList, int iItems);
		int FindListBoxString(HWND hWnd, const WCHAR* sString, int iStartIndex);
		int FindListBoxString(HWND hWnd, const WCHAR* sString);
		int FindExactListBoxString(HWND hWnd, const WCHAR* sString, int iStartIndex);
		int FindExactListBoxString(HWND hWnd, const WCHAR* sString);
		int DeleteSingleListBoxItemFriendly(HWND hWnd);
		int InsertListBoxItemNoDuplicate(HWND hWnd, WCHAR* sInStr);
		int InsertListBoxItemNoDuplicate(HWND hWnd, WCHAR* sInStr, int iInsertPos);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::ListBox
} //namespace::NSWFL
#endif
