///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_LISTVIEW_H_
#define _NSWFL_LISTVIEW_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace ListView {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool LVChecked(HWND hList, int iRow);
		bool LVDeleteItem(HWND hList);
		bool LVDeleteItem(HWND hList, int iItem);
		bool LVDeleteItemFriendly(HWND hList);
		bool LVDeleteItemFriendly(HWND hList, int iItem);
		bool LVGetItemText(HWND hList, int iRow, int iColumn, char *sText, int iMaxOutSz);
		LPARAM LVGetItemParam(HWND hList, int iRow, int iColumn);
		bool LVGetSelectedItemText(HWND hList, int iColumn, char *sText, int iMaxOutSz);
		bool LVMoveItemDown(HWND hList);
		bool LVMoveItemUp(HWND hList);
		bool LVMoveItemDown(HWND hList, int iItemIndex);
		bool LVMoveItemUp(HWND hList, int iItemIndex);
		int LVDeleteSelectedItems(HWND hList);
		int LVUnCheckAllRows(HWND hList);
		int LVCheckAllRows(HWND hList);

		int LVSelectAllRows(HWND hList);
		int LVDeSelectAllRows(HWND hList);

		int LVColumnCount(HWND hList);
		int LVColumnIndex(HWND hList, const char *sColumnName);
		int LVGetFirstSelectedItemIndex(HWND hList);
		int LVGetText(HWND hList, int iItem, int iColumn, char *sText, int iMaxText);
		int LVItemCount(HWND hList);
		int LVCheckedItemCount(HWND hList);

		void LVFocusItem(HWND hList, int iItem, bool bSelected);
		void LVSelectItem(HWND hList, int iItem, bool bSelected);

		void LVAutoSizeColumns(HWND hList);
		void LVAutoSizeColumns(HWND hList, int iMaxWidth);
		void LVCheck(HWND hList, int iRow, bool bChecked);
		void LVClear(HWND hList);
		void LVSetColumnWidth(HWND hList, int iPos, int iType, int iMax);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::ListView
} //namespace::NSWFL
#endif
