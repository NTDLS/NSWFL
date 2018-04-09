///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_MENU_CPP_
#define _NSWFL_MENU_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>

#include "NSWFL_Menu.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace Menu {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsMenuItemChecked(HMENU hMenu, int iCommand)
		{
			MENUITEMINFO MII;
			memset(&MII, 0, sizeof(MENUITEMINFO));
			MII.cbSize = sizeof(MENUITEMINFO);
			MII.fMask = MIIM_STATE;

			GetMenuItemInfo(hMenu, iCommand, FALSE, &MII);

			/*CheckMenuItem(
			_In_ HMENU hmenu,
				_In_ UINT  uIDCheckItem,
				_In_ UINT  uCheck //MF_CHECKED / MF_UNCHECKED
				);
			*/

			return ((MII.fState &MF_CHECKED) > 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsMouseOverMenu(HWND hWnd, HMENU hMenu, int iMenuItem)
		{
			RECT Rectangle;
			POINT Pointer;
			memset(&Rectangle, 0, sizeof(Rectangle));
			memset(&Pointer, 0, sizeof(Pointer));

			if (GetMenuItemRect(hWnd, hMenu, iMenuItem, &Rectangle))
			{
				GetCursorPos(&Pointer);
				return(PtInRect(&Rectangle, Pointer) > 0);
			}

			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Menu
} //namespace::NSWFL
#endif
