///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_LISTBOX_CPP_
#define _NSWFL_LISTBOX_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace ListBox {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			MAX Length: 32,767 Items
		*/
		int InsertListBoxItem(HWND hWnd, WCHAR* sInStr, int iMaxItems, int iInsertPos)
		{
			int NumberOfLines = 0;

			if (hWnd)
			{
				//FIXFIX: This is VERY unsafe - it locks thread if the window is unresponsive!!!!
				SendMessage(hWnd, LB_INSERTSTRING, iInsertPos, (LPARAM)(WCHAR*)sInStr);

				NumberOfLines = (int)SendMessage(hWnd, LB_GETCOUNT, 0, 0);

				while (NumberOfLines > iMaxItems)
				{
					NumberOfLines = (int)SendMessage(hWnd, LB_GETCOUNT, 0, 0);
					SendMessage(hWnd, LB_DELETESTRING, 0, FALSE);
				}

				NumberOfLines = (int)SendMessage(hWnd, LB_GETCOUNT, 0, 0);
				SendMessage(hWnd, LB_SETCURSEL, NumberOfLines - 1, 0);
				SendMessage(hWnd, LB_SETCARETINDEX, NumberOfLines - 1, FALSE);
			}
			return NumberOfLines;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InsertListBoxItem(HWND hWnd, WCHAR* sInStr, int iInsertPos)
		{
			return InsertListBoxItem(hWnd, sInStr, 10240, iInsertPos);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InsertListBoxItem(HWND hWnd, WCHAR* sInStr)
		{
			return InsertListBoxItem(hWnd, sInStr, 10240, -1);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InsertListBoxItemNoDuplicate(HWND hWnd, WCHAR* sInStr, int iInsertPos)
		{
			if (NSWFL::ListBox::FindExactListBoxString(hWnd, sInStr) < 0)
			{
				return NSWFL::ListBox::InsertListBoxItem(hWnd, sInStr);
			}
			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InsertListBoxItemNoDuplicate(HWND hWnd, WCHAR* sInStr)
		{
			return NSWFL::ListBox::InsertListBoxItemNoDuplicate(hWnd, sInStr, -1);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ClearListBox(HWND hWnd)
		{
			DWORD NumberOfLines = 0;

			NumberOfLines = (int)SendMessage(hWnd, LB_GETCOUNT, 0, 0);

			SendMessage(hWnd, LB_RESETCONTENT, 0, 0);

			return NumberOfLines;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int DeleteListBoxItem(HWND hWnd)
		{
			int iIndex = NSWFL::ListBox::GetCurrentListBoxItem(hWnd);
			return NSWFL::ListBox::DeleteListBoxItem(hWnd, iIndex);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int DeleteListBoxItem(HWND hWnd, int iItemPos)
		{
			SendMessage(hWnd, LB_DELETESTRING, iItemPos, 0);
			return (int)SendMessage(hWnd, LB_GETCOUNT, 0, 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CountListBoxItems(HWND hWnd)
		{
			return (int)SendMessage(hWnd, LB_GETCOUNT, 0, 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Use this Function only with multiple-selection list boxes.
		int SelectMultipleListBoxItem(HWND hWnd, int iItemPos, bool bSelect)
		{
			SendMessage(hWnd, LB_SETSEL, (WPARAM)bSelect, (LPARAM)iItemPos);
			return (int)SendMessage(hWnd, LB_GETCOUNT, 0, 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int SelectListBoxItem(HWND hWnd, int iItemPos)
		{
			SendMessage(hWnd, LB_SETCURSEL, (WPARAM)iItemPos, 0);
			return (int)SendMessage(hWnd, LB_GETCOUNT, 0, 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int GetCurrentListBoxItem(HWND hWnd)
		{
			return (int)SendMessage(hWnd, LB_GETCURSEL, 0, 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int GetListBoxItemText(WCHAR* sOutVal, int iMaxSz, HWND hWnd)
		{
			int iItemPos = NSWFL::ListBox::GetCurrentListBoxItem(hWnd);
			if (iItemPos >= 0)
			{
				Assert(NSWFL::ListBox::GetListBoxItemLength(hWnd, iItemPos) >= iMaxSz, "Buffer too small.");
				return (int)SendMessage(hWnd, LB_GETTEXT, (WPARAM)iItemPos, (LPARAM)sOutVal);
			}
			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int GetListBoxItemText(WCHAR* sOutVal, int iMaxSz, HWND hWnd, int iItemPos)
		{
			Assert(NSWFL::ListBox::GetListBoxItemLength(hWnd, iItemPos) >= iMaxSz, "Buffer too small.");
			return (int)SendMessage(hWnd, LB_GETTEXT, (WPARAM)iItemPos, (LPARAM)sOutVal);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int GetListBoxItemLength(HWND hWnd, int iItemPos)
		{
			return (int)SendMessage(hWnd, LB_GETTEXTLEN, iItemPos, 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int FindListBoxString(HWND hWnd, const WCHAR* sString, int iStartIndex)
		{
			return (int)SendMessage(hWnd, (UINT)LB_FINDSTRING, (WPARAM)iStartIndex, (LPARAM)sString);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int FindListBoxString(HWND hWnd, const WCHAR* sString)
		{
			return (int)FindListBoxString(hWnd, sString, -1);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int FindExactListBoxString(HWND hWnd, const WCHAR* sString, int iStartIndex)
		{
			return (int)SendMessage(hWnd, (UINT)LB_FINDSTRINGEXACT, (WPARAM)iStartIndex, (LPARAM)sString);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int FindExactListBoxString(HWND hWnd, const WCHAR* sString)
		{
			return (int)FindListBoxString(hWnd, sString, -1);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int SetHorizontalListBoxExtent(HWND hWnd, int iHorzExtent)
		{
			SendMessage(hWnd, (UINT)LB_SETHORIZONTALEXTENT, (WPARAM)iHorzExtent, (LPARAM)0);
			return (int)SendMessage(hWnd, LB_GETCOUNT, 0, 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int PopListBoxFromIntegerArray(HWND hWnd, int* iItemList, int iItems)
		{
			WCHAR sText[64];

			for (int iItem = 0; iItem < iItems; iItem++)
			{
				swprintf_s(sText, sizeof(sText), _T("%d"), iItemList[iItem]);
				InsertListBoxItem(hWnd, sText);
			}
			return iItems;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int PopListBoxFromStringArray(HWND hWnd, WCHAR** sItemList, int iItems)
		{
			for (int iItem = 0; iItem < iItems; iItem++)
			{
				InsertListBoxItem(hWnd, sItemList[iItem]);
			}
			return iItems;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//This functions return value must be free externally! Use: FreeArray(sArray, iItems) in the String.Cpp
		WCHAR** ListBoxToCharacterArray(HWND hWnd, int* iOutItemCount)
		{
			int iItems = CountListBoxItems(hWnd);
			int iItem = 0;
			int iAdded = 0;
			int iLength = 0;

#ifdef _USE_GLOBAL_MEMPOOL
			WCHAR** sArray = (WCHAR**)pMem->Allocate(sizeof(WCHAR*), iItems);
#else
			WCHAR** sArray = (WCHAR**)calloc(sizeof(WCHAR*), iItems);
#endif

			while (iItem < iItems)
			{
				if ((iLength = GetListBoxItemLength(hWnd, iItem)) > 0)
				{
#ifdef _USE_GLOBAL_MEMPOOL
					sArray[iAdded] = (WCHAR*)pMem->Allocate(sizeof(WCHAR), iLength + 1);
#else
					sArray[iAdded] = (WCHAR*)calloc(sizeof(WCHAR), iLength + 1);
#endif

					if (GetListBoxItemText(sArray[iAdded], iLength + 1, hWnd, iItem) > 0)
					{
						sArray[iAdded][iLength] = '\0';
						iAdded++;
					}
					else {
#ifdef _USE_GLOBAL_MEMPOOL
						pMem->Free(sArray[iAdded]);
#else
						free(sArray[iAdded]);
#endif
					}
				}
				iItem++;
			}

			*iOutItemCount = iItem;

			return sArray;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//This functions return value must be free externally!
		int* ListBoxToIntegerArray(HWND hWnd, int* iOutItemCount)
		{
			int iItems = CountListBoxItems(hWnd);
			int iItem = 0;
			int iAdded = 0;
			int iLength = 0;
			WCHAR sBuffer[64];

#ifdef _USE_GLOBAL_MEMPOOL
			int* iArray = (int*)pMem->Allocate(sizeof(int), iItems);
#else
			int* iArray = (int*)calloc(sizeof(int), iItems);
#endif

			while (iItem < iItems)
			{
				if ((iLength = GetListBoxItemLength(hWnd, iItem)) > 0)
				{
					if (GetListBoxItemText(sBuffer, sizeof(sBuffer), hWnd, iItem) > 0)
					{
						iArray[iAdded] = _wtol(sBuffer);
						iAdded++;
					}
				}
				iItem++;
			}

			*iOutItemCount = iItem;

			return iArray;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int DeleteSingleListBoxItemFriendly(HWND hWnd)
		{
			int iCurrent = GetCurrentListBoxItem(hWnd);
			int iItems = CountListBoxItems(hWnd);

			int iDeleted = DeleteListBoxItem(hWnd);

			if ((iCurrent + 1) == iItems)
			{
				SelectListBoxItem(hWnd, iCurrent - 1);
			}
			else {
				SelectListBoxItem(hWnd, iCurrent);
			}

			return iDeleted;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::ListBox
} //namespace::NSWFL
#endif
