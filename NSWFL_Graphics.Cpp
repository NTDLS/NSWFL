///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_GRAPHICS_CPP_
#define _NSWFL_GRAPHICS_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace Graphics {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Replaces all occurrences of the first pixil's color with the suppled color.

			//Example:
			MakeBitmapTransparent(hBitmap, GetSysColor(COLOR_MENU))

		*/
		HBITMAP MakeBitmapTransparent(HBITMAP hbmSrc, COLORREF backGroundColor)
		{

			HDC hdcSrc, hdcDst;
			HBITMAP hbmOld, hbmNew;
			BITMAP bm;
			COLORREF clrTP;

			if ((hdcSrc = CreateCompatibleDC(NULL)) != NULL)
			{
				if ((hdcDst = CreateCompatibleDC(NULL)) != NULL)
				{
					int nRow, nCol;
					GetObject(hbmSrc, sizeof(bm), &bm);
					hbmOld = (HBITMAP)SelectObject(hdcSrc, hbmSrc);
					hbmNew = CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes, bm.bmBitsPixel, NULL);
					SelectObject(hdcDst, hbmNew);

					BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);

					clrTP = GetPixel(hdcDst, 0, 0);// Get color of first pixel at 0,0

					for (nRow = 0; nRow < bm.bmHeight; nRow++)// work our way through all the pixels changing their color
					{
						for (nCol = 0; nCol < bm.bmWidth; nCol++)// when we hit our set transparency color.
						{
							if (GetPixel(hdcDst, nCol, nRow) == clrTP)
							{
								SetPixel(hdcDst, nCol, nRow, backGroundColor);
							}
						}
					}

					DeleteDC(hdcDst);
				}

				DeleteDC(hdcSrc);
			}

			return hbmNew;// return our transformed bitmap.
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Graphics
} //namespace::NSWFL
#endif
