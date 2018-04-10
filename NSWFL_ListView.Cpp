///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_LISTVIEW_CPP_
#define _NSWFL_LISTVIEW_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <CommCtrl.H>

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
using namespace NSWFL::Memory;
extern MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace ListView {

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LVUnCheckAllRows(HWND hList)
		{
			int iItems = ListView_GetItemCount(hList);

			for (int i = 0; i < iItems; i++)
			{
				ListView_SetCheckState(hList, i, false);
			}

			return iItems;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		int LVCheckAllRows(HWND hList)
		{
			int iItems = ListView_GetItemCount(hList);

			for (int i = 0; i < iItems; i++)
			{
				ListView_SetCheckState(hList, i, true);
			}

			return iItems;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LVDeSelectAllRows(HWND hList)
		{
			int iItems = ListView_GetItemCount(hList);

			for (int i = 0; i < iItems; i++)
			{
				ListView_SetItemState(hList, i, 0, LVIS_SELECTED);
			}

			return iItems;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LVSelectAllRows(HWND hList)
		{
			int iItems = ListView_GetItemCount(hList);

			for(int i = 0; i < iItems; i++)
			{
				ListView_SetItemState(hList, i, LVIS_SELECTED, LVIS_SELECTED);
			}

			return iItems;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LVGetText(HWND hList, int iItem, int iColumn, char *sText, int iMaxText)
		{
			LVITEM Item;
			memset(&Item, 0, sizeof(Item));
			Item.iItem = iItem;
			Item.mask = LVIF_TEXT;
			Item.iSubItem = iColumn;
			Item.cchTextMax = iMaxText;
			Item.pszText = sText;

			return (int)SendMessage(hList, LVM_GETITEMTEXT, (WPARAM)iItem, (LPARAM)&Item);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void LVCheck(HWND hList, int iRow, bool bChecked)
		{
			ListView_SetCheckState(hList, iRow, bChecked);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool LVChecked(HWND hList, int iRow)
		{
			return ListView_GetCheckState(hList, iRow) > 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		LPARAM LVGetItemParam(HWND hList, int iRow, int iColumn)
		{
			LV_ITEM lvItem;
			memset(&lvItem, 0, sizeof(lvItem));

			lvItem.iItem = iRow;
			lvItem.iSubItem = iColumn;
			lvItem.mask = LVIF_PARAM;

			ListView_GetItem(hList, &lvItem);

			return lvItem.lParam;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool LVGetItemText(HWND hList, int iRow, int iColumn, char *sText, int iMaxOutSz)
		{
			LV_ITEM lvItem;
			memset(&lvItem, 0, sizeof(lvItem));

			lvItem.iItem = iRow;
			lvItem.iSubItem = iColumn;
			lvItem.mask = LVIF_TEXT;
			lvItem.cchTextMax = iMaxOutSz;
			lvItem.pszText = sText;

			return ListView_GetItem(hList, &lvItem) > 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool LVDeleteItemFriendly(HWND hList, int iItem)
		{
			if (ListView_DeleteItem(hList, iItem) > 0)
			{
				if (iItem == NSWFL::ListView::LVItemCount(hList))
				{
					iItem--;
				}
				LVSelectItem(hList, iItem, true);
				LVFocusItem(hList, iItem, true);
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LVDeleteSelectedItems(HWND hList)
		{
			int iDeletedCount = 0;
			while (true)
			{
				int iIndex = ListView_GetNextItem(hList, -1, LVNI_SELECTED);
				if (iIndex >= 0)
				{
					iDeletedCount++;
					ListView_DeleteItem(hList, iIndex);
				}
				else
				{
					break;
				}
			}
			return iDeletedCount;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool LVDeleteItemFriendly(HWND hList)
		{
			int iItem = NSWFL::ListView::LVGetFirstSelectedItemIndex(hList);
			if (iItem >= 0)
			{
				return NSWFL::ListView::LVDeleteItemFriendly(hList, iItem);
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			What the hell does this do? (other than  :|
		*/
		bool LVDeleteItem(HWND hList, int iItem)
		{
			if (ListView_DeleteItem(hList, iItem) > 0)
			{
				if (iItem == NSWFL::ListView::LVItemCount(hList))
				{
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function is designed to only work with single selection list views.
		*/
		bool LVDeleteItem(HWND hList)
		{
			int iItem = NSWFL::ListView::LVGetFirstSelectedItemIndex(hList);
			if (iItem >= 0)
			{
				return NSWFL::ListView::LVDeleteItem(hList, iItem);
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LVGetFirstSelectedItemIndex(HWND hList)
		{
			return ListView_GetNextItem(hList, -1, LVNI_SELECTED);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void LVFocusItem(HWND hList, int iItem, bool bSelected)
		{
			if (bSelected)
			{
				ListView_SetItemState(hList, iItem, LVIS_FOCUSED, LVIS_FOCUSED);
			}
			else
			{
				ListView_SetItemState(hList, iItem, 0, LVIS_FOCUSED);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Passing iItem as -1 will apply selection to all items.
		*/
		void LVSelectItem(HWND hList, int iItem, bool bSelected)
		{
			if (bSelected)
			{
				ListView_SetItemState(hList, iItem, LVIS_SELECTED, LVIS_SELECTED);
			}
			else
			{
				ListView_SetItemState(hList, iItem, 0, LVIS_SELECTED);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function is designed to only work with single selection list views.
		*/
		bool LVGetSelectedItemText(HWND hList, int iColumn, char *sText, int iMaxOutSz)
		{
			if (ListView_GetSelectedCount(hList) == 1)
			{
				int iRow = NSWFL::ListView::LVGetFirstSelectedItemIndex(hList);
				if (iRow >= 0)
				{
					return NSWFL::ListView::LVGetItemText(hList, iRow, iColumn, sText, iMaxOutSz);
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LVColumnCount(HWND hList)
		{
			int iCount = 0;
			LVCOLUMN lColumn; //Dummy stuff.
			memset(&lColumn, 0, sizeof(lColumn));
			lColumn.mask = LVCF_WIDTH;

			while (true)
			{
				if (ListView_GetColumn(hList, iCount, &lColumn))
				{
					iCount++;
				}
				else {
					break;
				}
			}
			return iCount;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LVItemCount(HWND hList)
		{
			return ListView_GetItemCount(hList);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LVCheckedItemCount(HWND hList)
		{
			int iCheckedCount = 0;
			int iItemCount = NSWFL::ListView::LVItemCount(hList);

			for (int iIndex = 0; iIndex < iItemCount; iIndex++)
			{
				if (ListView_GetCheckState(hList, iIndex) > 0)
				{
					iCheckedCount++;

				}
			}

			return iCheckedCount;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool LVMoveItemDown(HWND hList)
		{
			return NSWFL::ListView::LVMoveItemDown(hList, NSWFL::ListView::LVGetFirstSelectedItemIndex(hList));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool LVMoveItemDown(HWND hList, int iItemIndex)
		{
			int iColumns = NSWFL::ListView::LVColumnCount(hList);
			if (iColumns <= 0)
			{
				return false;
			}

			if (iItemIndex + 1 >= NSWFL::ListView::LVItemCount(hList) || iItemIndex < 0)
			{
				return false;
			}

			int iNewItem = 0;
			int iMaxText = 1024 * 10;

#ifdef _USE_GLOBAL_MEMPOOL
			char *sText = (char *)pMem->Allocate(iMaxText, sizeof(char));
#else
			char *sText = (char *)calloc(iMaxText, sizeof(char));
#endif

			bool bChecked = (ListView_GetCheckState(hList, iItemIndex) > 0);

			for (int iSubItem = 0; iSubItem < iColumns; iSubItem++)
			{
				LVITEM Item;
				memset(&Item, 0, sizeof(Item));
				if (iSubItem == 0)
				{
					Item.mask = LVIF_COLFMT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE | LVIF_TEXT;
				}
				else {
					Item.mask = LVIF_TEXT;
				}
				Item.iItem = iItemIndex;
				Item.iSubItem = iSubItem;
				Item.cchTextMax = iMaxText;
				Item.pszText = sText;
				if (ListView_GetItem(hList, &Item))
				{
					if (iSubItem == 0)
					{
						Item.state |= LVIS_SELECTED;
						Item.iItem = iItemIndex + 2;
						iNewItem = ListView_InsertItem(hList, &Item);
						ListView_SetCheckState(hList, Item.iItem, bChecked);
					}
					else {
						Item.iItem = iNewItem;
						ListView_SetItem(hList, &Item);
					}
				}
			}

			NSWFL::ListView::LVDeleteItem(hList, iItemIndex);
			ListView_EnsureVisible(hList, iNewItem, FALSE);

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sText);
#else
			free(sText);
#endif

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool LVMoveItemUp(HWND hList)
		{
			return NSWFL::ListView::LVMoveItemUp(hList, NSWFL::ListView::LVGetFirstSelectedItemIndex(hList));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool LVMoveItemUp(HWND hList, int iItemIndex)
		{
			int iColumns = LVColumnCount(hList);
			if (iColumns <= 0 || iItemIndex <= 0)
			{
				return false;
			}

			int iNewItem = 0;
			int iMaxText = 1024 * 10;

#ifdef _USE_GLOBAL_MEMPOOL
			char *sText = (char *)pMem->Allocate(iMaxText, sizeof(char));
#else
			char *sText = (char *)calloc(iMaxText, sizeof(char));
#endif

			bool bChecked = (ListView_GetCheckState(hList, iItemIndex) > 0);

			for (int iSubItem = 0; iSubItem < iColumns; iSubItem++)
			{
				LVITEM Item;
				memset(&Item, 0, sizeof(Item));
				if (iSubItem == 0)
				{
					Item.iItem = iItemIndex;
					Item.mask = LVIF_COLFMT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE | LVIF_TEXT;
				}
				else {
					Item.iItem = iItemIndex + 1;
					Item.mask = LVIF_TEXT;
				}
				Item.iSubItem = iSubItem;
				Item.cchTextMax = iMaxText;
				Item.pszText = sText;
				if (ListView_GetItem(hList, &Item))
				{
					if (iSubItem == 0)
					{
						Item.state |= LVIS_SELECTED;
						Item.iItem = iItemIndex - 1;
						iNewItem = ListView_InsertItem(hList, &Item);
						ListView_SetCheckState(hList, Item.iItem, bChecked);
					}
					else {
						Item.iItem = iNewItem;
						ListView_SetItem(hList, &Item);
					}
				}
			}

			NSWFL::ListView::LVDeleteItem(hList, iItemIndex + 1);
			ListView_EnsureVisible(hList, iNewItem, FALSE);

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sText);
#else
			free(sText);
#endif

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void LVClear(HWND hList)
		{
			LVCOLUMN lColumn; //Dummy stuff.
			memset(&lColumn, 0, sizeof(lColumn));
			lColumn.mask = LVCF_WIDTH;

			ListView_DeleteAllItems(hList);

			for (int iIndex = LVColumnCount(hList) - 1; iIndex >= 0; iIndex--)
			{
				ListView_DeleteColumn(hList, iIndex);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LVColumnIndex(HWND hList, const char *sColumnName)
		{
			LVCOLUMN lColumn;

			char sText[255];

			for (int iIndex = 0; true; iIndex++)
			{
				memset(&lColumn, 0, sizeof(lColumn));

				lColumn.mask = LVCF_TEXT;
				lColumn.pszText = sText;
				lColumn.cchTextMax = sizeof(sText);

				if (ListView_GetColumn(hList, iIndex, &lColumn))
				{
					if (NSWFL::String::StrEqlI(sColumnName, sText))
					{
						return iIndex;
					}
				}
				else {
					break;
				}
			}

			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			iType:
				LVSCW_AUTOSIZE = -1
				LVSCW_AUTOSIZE_USEHEADER = -2
		*/
		void LVSetColumnWidth(HWND hList, int iPos, int iType, int iMax)
		{
			LVCOLUMN vColumn;
			int iMin = 0;

			if (ListView_GetColumnWidth(hList, iPos) > 0) //Don't resize 0-width (hidden) columns.
			{
				ListView_SetColumnWidth(hList, iPos, LVSCW_AUTOSIZE_USEHEADER);
				memset(&vColumn, 0, sizeof(LVCOLUMN));
				vColumn.mask = LVCF_WIDTH;
				if (ListView_GetColumn(hList, iPos, &vColumn))
				{
					iMin = vColumn.cx; //Get the size of the column header, we don't want to collapse it.
				}

				ListView_SetColumnWidth(hList, iPos, iType);
				if (iMin > 0 || iMax > 0) //If iMax less than 0 means no Max size.
				{
					memset(&vColumn, 0, sizeof(LVCOLUMN));
					vColumn.mask = LVCF_WIDTH;
					if (ListView_GetColumn(hList, iPos, &vColumn))
					{
						if (iMin > 0 && vColumn.cx < iMin)
						{
							ListView_SetColumnWidth(hList, iPos, iMin);
						}

						if (iMax > 0 && vColumn.cx > iMax)
						{
							ListView_SetColumnWidth(hList, iPos, iMax);
						}
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void LVAutoSizeColumns(HWND hList, int iMaxWidth)
		{
			for (int iCol = 0; iCol < LVColumnCount(hList); iCol++)
			{
				LVSetColumnWidth(hList, iCol, LVSCW_AUTOSIZE, iMaxWidth);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void LVAutoSizeColumns(HWND hList)
		{
			LVAutoSizeColumns(hList, 250);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::ListView
} //namespace::NSWFL
#endif
