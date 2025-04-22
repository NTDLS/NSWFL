///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_STRING_CPP_
#define _NSWFL_STRING_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace String {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//The returned pointer must be freed externally.
		WCHAR* Space(size_t iSpaces)
		{
#ifdef _USE_GLOBAL_MEMPOOL
			WCHAR* sSpaces = (WCHAR*)pMem->Allocate(iSpaces + 1, 1);
#else
			WCHAR* sSpaces = (WCHAR*)malloc(iSpaces + 1);
#endif

			if (sSpaces)
			{
				memset(sSpaces, ' ', iSpaces);
				sSpaces[iSpaces] = '\0';
				return sSpaces;
			}
			return NULL;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* Space(WCHAR* sBuffer, size_t iSpaces)
		{
			memset(sBuffer, ' ', iSpaces);
			sBuffer[iSpaces] = '\0';
			return sBuffer;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsUpper(WCHAR cIn)
		{
			return (cIn >= 65 && cIn <= 90);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsLower(WCHAR cIn)
		{
			return (cIn >= 97 && cIn <= 122);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR ToUpper(WCHAR cIn)
		{
			if (IsLower(cIn))
			{
				return (cIn - 32);
			}
			return cIn;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR ToLower(WCHAR cIn)
		{
			if (IsUpper(cIn))
			{
				return (cIn + 32);
			}
			return cIn;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int IndexOf(const WCHAR* sBuf, const WCHAR cCharacter)
		{
			return NSWFL::String::IndexOf(sBuf, (int)wcslen(sBuf), cCharacter);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int IndexOf(const WCHAR* sBuf, int iBufSz, const WCHAR cCharacter)
		{
			for (int iPos = 0; iPos < iBufSz; iPos++)
			{
				if ((WCHAR)sBuf[iPos] == (WCHAR)cCharacter)
				{
					return iPos;
				}
			}
			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LastIndexOf(const WCHAR* sBuf, const WCHAR cCharacter)
		{
			return NSWFL::String::LastIndexOf(sBuf, (int)wcslen(sBuf), cCharacter);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LastIndexOf(const WCHAR* sBuf, int iBufSz, const WCHAR cCharacter)
		{
			if (iBufSz > 0)
			{
				for (int iPos = iBufSz - 1; iPos > -1; iPos--)
				{
					if ((WCHAR)sBuf[iPos] == (WCHAR)cCharacter)
					{
						return iPos;
					}
				}
			}
			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* UCase(WCHAR* sBuf, int iBufSz)
		{
			for (int iPos = 0; iPos < iBufSz; iPos++)
			{
				sBuf[iPos] = NSWFL::String::ToUpper(sBuf[iPos]);
			}
			return sBuf;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* LCase(WCHAR* sBuf, int iBufSz)
		{
			for (int iPos = 0; iPos < iBufSz; iPos++)
			{
				sBuf[iPos] = NSWFL::String::ToLower(sBuf[iPos]);
			}
			return sBuf;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* UCase(WCHAR* sBuf)
		{
			return NSWFL::String::UCase(sBuf, (int)wcslen(sBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* LCase(WCHAR* sBuf)
		{
			return NSWFL::String::LCase(sBuf, (int)wcslen(sBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Trim(WCHAR* sInOutBuf, int iLen)
		{
			int iLength = NSWFL::String::LTrim(sInOutBuf, iLen);
			return NSWFL::String::RTrim(sInOutBuf, iLen);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Trim(WCHAR* sInOutBuf)
		{
			return NSWFL::String::Trim(sInOutBuf, (int)wcslen(sInOutBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LTrim(WCHAR* sInOutBuf, int iLen)
		{
			if (iLen > 0)
			{
				NSWFL::String::ReverseString(sInOutBuf, iLen);
				iLen = NSWFL::String::RTrim(sInOutBuf, iLen);
				NSWFL::String::ReverseString(sInOutBuf, iLen);
			}

			return iLen;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LTrim(WCHAR* sInOutBuf)
		{
			return NSWFL::String::LTrim(sInOutBuf, (int)wcslen(sInOutBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int RTrim(WCHAR* sInOutBuf, int iLen)
		{
			if (iLen > 0)
			{
				iLen = (NSWFL::String::GetLastCharPos(sInOutBuf, iLen) + 1);
				sInOutBuf[iLen] = '\0'; //replace the NULL terminator.
			}
			return iLen;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int RTrim(WCHAR* sInOutBuf)
		{
			return NSWFL::String::RTrim(sInOutBuf, (int)wcslen(sInOutBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//sInOutBuf returns the string passed into it with all white spaces
		//	trimmed off the beginning and end of it.
		int CleanWhiteSpaces(WCHAR* sInOutBuf, int iLen)
		{
			if (iLen > 0)
			{
				iLen = NSWFL::String::RTrim(sInOutBuf, iLen);
				NSWFL::String::ReverseString(sInOutBuf, iLen);
				iLen = NSWFL::String::RTrim(sInOutBuf, iLen);
				NSWFL::String::ReverseString(sInOutBuf, iLen);
			}
			return iLen;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CleanWhiteSpaces(WCHAR* sInOutBuf)
		{
			return NSWFL::String::CleanWhiteSpaces(sInOutBuf, (int)wcslen(sInOutBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ReplaceStringsI(const WCHAR* sInBuf, const WCHAR* sReplace, const WCHAR* sWith, WCHAR* sOutBuf, int iMaxSz)
		{
			return NSWFL::String::ReplaceStringsI(sInBuf, sReplace, (int)wcslen(sReplace), sWith, sOutBuf, iMaxSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ReplaceStringsI(const WCHAR* sInBuf, const WCHAR* sReplace, int iReplaceLen,
			const WCHAR* sWith, WCHAR* sOutBuf, int iMaxSz)
		{
			const WCHAR* sNULL = NULL;

			int iReplaces = 0;

			if (sInBuf == NULL || sOutBuf == NULL)
			{
				return -1;
			}

			wcscpy_s(sOutBuf, iMaxSz, _T(""));

			for (sNULL = sInBuf; *sNULL; ++sNULL)
			{
				if (_wcsnicmp(sNULL, sReplace, iReplaceLen) == 0)
				{
					wcscat_s(sOutBuf, iMaxSz, sWith);
					sNULL += (iReplaceLen - 1);
					iReplaces++;
				}
				else wcsncat_s(sOutBuf, iMaxSz, sNULL, 1);
			}

			return iReplaces;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ReplaceStrings(const WCHAR* sInBuf, const WCHAR* sReplace, const WCHAR* sWith, WCHAR* sOutBuf, int iMaxSz)
		{
			return NSWFL::String::ReplaceStrings(sInBuf, sReplace, (int)wcslen(sReplace), sWith, sOutBuf, iMaxSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ReplaceStrings(const WCHAR* sInBuf, const WCHAR* sReplace, int iReplaceLen,
			const WCHAR* sWith, WCHAR* sOutBuf, int iMaxSz)
		{
			const WCHAR* sNULL = NULL;

			int iReplaces = 0;

			if (sInBuf == NULL || sOutBuf == NULL)
			{
				return -1;
			}

			wcscpy_s(sOutBuf, iMaxSz, _T(""));

			for (sNULL = sInBuf; *sNULL; ++sNULL)
			{
				if (wcsncmp(sNULL, sReplace, iReplaceLen) == 0)
				{
					wcscat_s(sOutBuf, iMaxSz, sWith);
					sNULL += (iReplaceLen - 1);
					iReplaces++;
				}
				else wcsncat_s(sOutBuf, iMaxSz, sNULL, 1);
			}

			return iReplaces;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ReplaceCharacter(WCHAR* sInBuf, int iBufferLength, const WCHAR cReplace, const WCHAR cWith)
		{
			int iOccur = 0;

			for (int i = 0; i < iBufferLength; i++)
			{
				if (sInBuf[i] == '+')
				{
					sInBuf[i] = ' ';
					iOccur++;
				}
			}

			return iOccur;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ReplaceCharacter(const WCHAR* sInBuf, WCHAR* sOutBuf, const WCHAR cReplace, const WCHAR cWith)
		{
			int iPos = 0;
			int iOccur = 0;
			int iLength = (int)wcslen(sInBuf);

			while (iPos < iLength)
			{
				if (sInBuf[iPos] != cReplace)
				{
					sOutBuf[iPos] = sInBuf[iPos];
					iOccur++;
				}
				else sOutBuf[iPos] = cWith;

				iPos++;
			}

			return iOccur;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ReplaceCharacter(WCHAR* sInOutBuf, const WCHAR cReplace, const WCHAR cWith)
		{
			return NSWFL::String::ReplaceCharacter(sInOutBuf, sInOutBuf, cReplace, cWith);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool ReverseString(WCHAR* sBuf, int iBufSz)
		{
			WCHAR* String1 = NULL;
			WCHAR* String2 = NULL;

			if (!sBuf || !*sBuf)
			{
				return false;
			}

			for (String1 = sBuf, String2 = sBuf + iBufSz - 1; String2 > String1; ++String1, --String2)
			{
				*String1 ^= *String2;
				*String2 ^= *String1;
				*String1 ^= *String2;
			}

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InStrI(const WCHAR* sSearchFor, int sSearchForSz, const WCHAR* sInBuf, const int iBufSz, const int iStartPos)
		{
			int iLookingLoop = 0;

			if (iStartPos > iBufSz)
			{
				return -2;
			}
			if (sSearchForSz > iBufSz)
			{
				return -2;
			}

			for (int iControlLoop = iStartPos; iControlLoop <= (iBufSz - sSearchForSz); iControlLoop++)
			{
				if (ToLower(sInBuf[iControlLoop]) == ToLower(sSearchFor[iLookingLoop]))
				{
					while (iLookingLoop <= sSearchForSz)
					{
						if (ToLower(sSearchFor[iLookingLoop]) == ToLower(sInBuf[iLookingLoop + iControlLoop]))
						{
							iLookingLoop++;
						}
						else iLookingLoop = sSearchForSz + 1;

						if (iLookingLoop == sSearchForSz)
						{
							return iControlLoop;
						}
					}
				}
				iLookingLoop = 0;
			}

			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InStr(const WCHAR* sSearchFor, int sSearchForSz, const WCHAR* sInBuf, const int iBufSz, const int iStartPos)
		{
			int iLookingLoop = 0;

			if (iStartPos > iBufSz)
			{
				return -2;
			}
			if (sSearchForSz > iBufSz)
			{
				return -2;
			}

			for (int iControlLoop = iStartPos; iControlLoop <= (iBufSz - sSearchForSz); iControlLoop++)
			{
				if (sInBuf[iControlLoop] == sSearchFor[iLookingLoop])
				{
					while (iLookingLoop <= sSearchForSz)
					{
						if (sSearchFor[iLookingLoop] == sInBuf[iLookingLoop + iControlLoop])
						{
							iLookingLoop++;
						}
						else iLookingLoop = sSearchForSz + 1;

						if (iLookingLoop == sSearchForSz)
						{
							return iControlLoop;
						}
					}
				}
				iLookingLoop = 0;
			}

			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InStr(const WCHAR* sSearchFor, const WCHAR* sInBuf, const int iBufSz, const int iStartPos)
		{
			return InStr(sSearchFor, (int)wcslen(sSearchFor), sInBuf, iBufSz, iStartPos);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InStr(const WCHAR* sSearchFor, const WCHAR* sInBuf)
		{
			return InStr(sSearchFor, sInBuf, (int)wcslen(sInBuf), 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InStrI(const WCHAR* sSearchFor, const WCHAR* sInBuf, const int iBufSz, const int iStartPos)
		{
			return InStrI(sSearchFor, (int)wcslen(sSearchFor), sInBuf, iBufSz, iStartPos);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InStrI(const WCHAR* sSearchFor, const WCHAR* sInBuf)
		{
			return InStrI(sSearchFor, sInBuf, (int)wcslen(sInBuf), 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CompareBinString(const WCHAR* sBuf1, int iStrSz1, const WCHAR* sBuf2, int iStrSz2)
		{
			if (iStrSz1 > iStrSz2) //Use the shortest string.
			{
				iStrSz1 = iStrSz2;
			}

			return (memcmp(sBuf1, sBuf2, iStrSz1) == 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringScan(const WCHAR* sInBuf, int iStartPos, int iLength, WCHAR* sOutVal, int iMaxSz)
		{
			wcsncpy_s(sOutVal, iMaxSz, &sInBuf[iStartPos], iLength);
			return iLength;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int AppendStrings(const WCHAR* sBuf1, const WCHAR* sBuf2, int iBuf1Sz, int iBuf2Sz, WCHAR* sOutBuf, int iMaxSz)
		{
			int iWPos = iBuf1Sz;
			int iRPos = 0;

			if (((iBuf1Sz + iBuf2Sz) + 1) > iMaxSz)
			{
				return -1;
			}

			memcpy_s(sOutBuf, iMaxSz, sBuf1, iBuf1Sz);

			while (iRPos < iBuf2Sz)
			{
				sOutBuf[iWPos++] = sBuf2[iRPos++];
			}

			sOutBuf[iWPos] = '\0';

			return iWPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CompareFlagedString(const WCHAR* sBuf, const WCHAR* sFlag)
		{
			int iFlagLength = (int)wcslen(sFlag);

			if (wcsncmp(sBuf, sFlag, iFlagLength) == 0)
			{
				return iFlagLength;
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int GetFlagedValue(const WCHAR* sBuf, int iSkpLen, int iBufSz, WCHAR* sOutBuf, int iMaxSz)
		{
			int iWPos = 0;
			int iRPos = iSkpLen;

			if (((iBufSz - iSkpLen) + 1) > iMaxSz)
			{
				return (0 - (iBufSz - iSkpLen)); // Returns the number of bytes required as a negitive number.
			}

			while (iRPos < iBufSz)
			{
				sOutBuf[iWPos++] = sBuf[iRPos++];
			}

			sOutBuf[iWPos] = '\0';

			return iWPos; //The length of the string returned in sOutBuf.
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* SimpleCipherString(WCHAR* sBuf, int iBufSz)
		{
			for (int iPos = 0; iPos < iBufSz; iPos++)
			{
				((WCHAR*)sBuf)[iPos] += (WCHAR)128;
			}

			return sBuf;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CopyString(WCHAR* sOutBuf, int iMaxSz, const WCHAR* sInBuf, int iInBufSz)
		{
			int iPos = 0;

			if ((iInBufSz + 1) > iMaxSz)
			{
				return -1;
			}

			while (iPos < iInBufSz)
			{
				sOutBuf[iPos] = sInBuf[iPos];
				iPos++;
			}

			sOutBuf[iPos] = '\0';

			return iPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Returns the count of the Occurrences of "cLookFor" that are found in "sInStr".

		int CountOccurrences(const WCHAR cLookFor, const WCHAR* sInStr, int iInStrSz)
		{
			int iOccurrences = 0;
			for (int iPos = 0; iPos < iInStrSz; iPos++)
			{
				if ((WCHAR)sInStr[iPos] == (WCHAR)cLookFor)
				{
					iOccurrences++;
				}
			}

			return iOccurrences;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Returns the count of the Occurrences of "cLookFor" that are found in "sInStr".

		int CountOccurrences(const WCHAR cLookFor, const WCHAR* sInStr)
		{
			return CountOccurrences(cLookFor, sInStr, (int)wcslen(sInStr));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Returns the count of the Occurrences of "sLookFor" that are found in "sInStr".

		int CountOccurrences(const WCHAR* sLookFor, const WCHAR* sInStr, int iInStrSz)
		{
			int iPos = 0;
			int iOccurrences = 0;
			while ((iPos = InStr(sLookFor, sInStr, iInStrSz, iPos)) >= 0)
			{
				iPos++;
				iOccurrences++;
			}

			return iOccurrences;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Returns the count of the Occurrences of "sLookFor" that are found in "sInStr".

		int CountOccurrences(const WCHAR* sLookFor, const WCHAR* sInStr)
		{
			return CountOccurrences(sLookFor, sInStr, (int)wcslen(sInStr));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns a zero-based, one-dimensional array containing a specified number of substrings.

			Example:

				WCHAR **sArray = NULL;
				WCHAR *sBuf = "Hello\tCruel\tWorld\tFrom\tC\tPlus\tPlus\t";
				int iItems = 0;
				int iItem = 0;

				sArray = SplitEx(sBuf, wcslen(sBuf), sArray, &iItems, wcslen("\t"), 1);

				wprintf(_T("Array Items: %d\n"), UBound(sArray));

				while(iItem < iItems)
				{
					wprintf(_T(Item[%d]: %s\n"), iItem, sArray[iItem]);
					iItem++;
				}

				FreeSplitStringEx(sArray, iItems);
		*/

		WCHAR** Split(const WCHAR* sBuf, int iBufSz, int* iOutItemCount, const WCHAR* sSplitOn, int iSplitOnSz)
		{
			int iStartPos = 0;

			int iRPos = 0;
			int iWPos = 0;

			int iItems = 0;

			int iItemCount = 0; // The number of sub-strings that are going to be parsed from the string.
			int iLastPos = 0;

			//Get the number of items that we are going to split.
			while ((iStartPos = InStr(sSplitOn, iSplitOnSz, sBuf, iBufSz, iStartPos)) >= 0)
			{
				iItemCount++;
				iStartPos = (iStartPos + iSplitOnSz);
				iLastPos = iStartPos;
			}

			//The string is not terminated by a delimiter.
			if (iBufSz > iLastPos)
			{
				iItemCount++;
			}

			iStartPos = 0;

			//Allocate enough RAM to hold all the sub-strings and the last NULL array item.

#ifdef _USE_GLOBAL_MEMPOOL
			WCHAR** sArray = (WCHAR**)pMem->Allocate(sizeof(WCHAR*), iItemCount + 1);
#else
			WCHAR** sArray = (WCHAR**)calloc(sizeof(WCHAR*), iItemCount + 1);
#endif

			while ((iStartPos = InStr(sSplitOn, iSplitOnSz, sBuf, iBufSz, iRPos)) >= 0)
			{
				//Allocate enough RAM to hold the sub-string.
#ifdef _USE_GLOBAL_MEMPOOL
				sArray[iItems] = (WCHAR*)pMem->Allocate(sizeof(WCHAR), (iStartPos - iRPos) + 1);
#else
				sArray[iItems] = (WCHAR*)calloc(sizeof(WCHAR), (iStartPos - iRPos) + 1);
#endif

				iWPos = 0;
				while (iRPos < iStartPos)
				{
					sArray[iItems][iWPos++] = sBuf[iRPos++];
				}

				sArray[iItems][iWPos] = '\0';

				iRPos = (iRPos + iSplitOnSz);

				iItems++;
			}

			//The string is not terminated by a delimiter.
			if (iBufSz > iRPos)
			{
				//Allocate enough RAM to hold the sub-string.
#ifdef _USE_GLOBAL_MEMPOOL
				sArray[iItems] = (WCHAR*)pMem->Allocate(sizeof(WCHAR), (iBufSz - iRPos) + 1);
#else
				sArray[iItems] = (WCHAR*)calloc(sizeof(WCHAR), (iBufSz - iRPos) + 1);
#endif

				iWPos = 0;

				while (iRPos < iBufSz)
				{
					sArray[iItems][iWPos++] = sBuf[iRPos++];
				}

				sArray[iItems][iWPos] = '\0';
				iRPos = (iRPos + iSplitOnSz);

				iItems++;
			}

			//Insert a null at the end of the array.
			sArray[iItems] = NULL;

			if (iOutItemCount)
			{
				*iOutItemCount = iItems;
			}

			return sArray;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR** Split(const WCHAR* sBuf, int iBufSz, int* iOutItemCount, const WCHAR* sSplitOn)
		{
			return Split(sBuf, iBufSz, iOutItemCount, sSplitOn, (int)wcslen(sSplitOn));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns a zero-based, one-dimensional array containing a specified number of substrings.

			Example:

				WCHAR **sArray = NULL;
				WCHAR *sBuf = "Hello\tCruel\tWorld\tFrom\tC\tPlus\tPlus\t";
				int iItem = 0;

				sArray = Split(sBuf, "\t");

				wprintf(_T(Array Items: %d\n"), UBound(sArray));

				while(sArray[iItem] != NULL)
				{
					wprintf(_T(Item[%d]: %s\n"), iItem, sArray[iItem]);
					iItem++;
				}

				FreeSplitString(sArray);
		*/

		WCHAR** Split(const WCHAR* sBuf, const WCHAR* sSplitOn)
		{
			return Split(sBuf, (int)wcslen(sBuf), NULL, sSplitOn, (int)wcslen(sSplitOn));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR** Split(const WCHAR* sBuf, int iBufSz, const WCHAR* sSplitOn)
		{
			return Split(sBuf, iBufSz, NULL, sSplitOn, (int)wcslen(sSplitOn));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function will free all the RAM allocated by an array.

			All array items, and the array its self.

			Intended for use with the Split() and SplitEx functions.
		*/
		void FreeArray(WCHAR** sArray, int iItems)
		{
			if (sArray)
			{
				for (int iItem = 0; iItem < iItems; iItem++)
				{
#ifdef _USE_GLOBAL_MEMPOOL
					pMem->Free(sArray[iItem]);
#else
					free(sArray[iItem]);
#endif
				}
#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(sArray);
#else
				free(sArray);
#endif
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function will free all the RAM allocated by an array.

			All array items, and the array its self.

			Intended for use with the Split() and SplitEx functions.
		*/
		void FreeArray(WCHAR** sArray)
		{
			NSWFL::String::FreeArray(sArray, NSWFL::String::UBound(sArray));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function will return the number of items in a WCHAR *Array[].
			This function will malfunction if the last item in the array is not NULL.

			Intended for use with the Split() and SplitEx functions.
		*/
		int UBound(WCHAR** sArray)
		{
			int iItem = 0;

			while (sArray[iItem])
			{
				iItem++;
			}

			return iItem;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Returns true if the passed character is 0-9.
		bool IsNumeric(const WCHAR cIn)
		{
			return (cIn >= 48 && cIn <= 57);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Returns true if the passed string is in the following formats:
			//0, -0.0, 0.0, -0, -0.0
		bool IsNumeric(const WCHAR* sText, int iLength)
		{
			int iRPos = 0;
			bool bHasDecimal = false;

			if (iLength == 0)
			{
				return false;
			}

			if (sText[iRPos] == '-' || sText[iRPos] == '+')
			{
				iRPos++;
			}

			for (; iRPos < iLength; iRPos++)
			{
				if (!IsNumeric(sText[iRPos]))
				{
					if (sText[iRPos] == '.')
					{
						if (iRPos == iLength - 1) //Decimal cannot be the last character.
						{
							return false;
						}
						if (iRPos == 0 || (iRPos == 1 && sText[0] == '-')) //Decimal cannot be the first character.
						{
							return false;
						}


						if (bHasDecimal)
						{
							return false;
						}
						bHasDecimal = true;
					}
					else {
						return false;
					}
				}
			}

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsNumeric(const WCHAR* sText)
		{
			return IsNumeric(sText, (int)wcslen(sText));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Mid(const WCHAR* sInBuf, int iInBufSz, int iStartPos, int iRChars, WCHAR* sOutBuf, int iMaxSz)
		{
			int iRPos = iStartPos;
			int iWPos = 0;
			int iChars = 0;

			if (iRChars <= 0 || iRChars > (iInBufSz - iStartPos))
			{
				iChars = iInBufSz;
			}
			else iChars = (iStartPos + iRChars);

			if (((iChars - iStartPos) + 1) > iMaxSz)
			{
				return -1;
			}

			while (iRPos < iChars)
			{
				sOutBuf[iWPos++] = sInBuf[iRPos++];
			}

			sOutBuf[iWPos] = '\0';

			return iWPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StrCat(WCHAR* sDest, int iMaxSz, int iDestLen, const WCHAR* sAppend, int iAppendLen)
		{
			int iRPos = 0;
			int iWPos = iDestLen;

			if ((iAppendLen + iDestLen) > iMaxSz)
			{
				return -1;
			}

			while (iRPos < iAppendLen)
			{
				sDest[iWPos++] = sAppend[iRPos++];
			}

			return iWPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Right(WCHAR* sOutBuf, int iMaxSz, const WCHAR* sInBuf, int iInLen, int iRight)
		{
			int iLen = iInLen;
			int iRPos = iLen - iRight;
			int iWPos = 0;

			if ((iRight + 1) > iMaxSz)
			{
				return -1;
			}

			if (iRight > iLen)
			{
				wcscpy_s(sOutBuf, iMaxSz, _T(""));
				return 0;
			}

			while (iRPos < iLen)
			{
				sOutBuf[iWPos++] = sInBuf[iRPos++];
			}

			sOutBuf[iWPos] = '\0';

			return iWPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Left(WCHAR* sOutBuf, int iMaxSz, const WCHAR* sInBuf, int iInLen, int iLeft)
		{
			int iLen = iInLen;
			int iRPos = 0;

			if ((iLeft + 1) > iMaxSz)
			{
				return -1;
			}

			if (iLeft > iInLen)
			{
				wcscpy_s(sOutBuf, iMaxSz, _T(""));
				return 0;
			}

			while (iRPos < iLeft)
			{
				sOutBuf[iRPos] = sInBuf[iRPos];
				iRPos++;
			}

			sOutBuf[iRPos] = '\0';

			return iRPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Returns true if the character passed into cChar is a whitespace, otherwise it returns false.
		bool IsWhiteSpace(WCHAR cChar)
		{
			if (cChar == ' ' || cChar == '\r' || cChar == '\n' || cChar == '\t' || cChar == '\0')
			{
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Returns the position of the first valid character in the string passed into sInBuf.
		//iInBufSz should be the length of the string being passed into sInBuf
		int GetFirstCharPos(const WCHAR* sInBuf, int iInBufSz)
		{
			for (int iRPos = 0; iRPos < iInBufSz; iRPos++)
			{
				if (!IsWhiteSpace(sInBuf[iRPos]))
				{
					return iRPos;
				}
			}
			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int GetFirstCharPos(const WCHAR* sInBuf)
		{
			return GetFirstCharPos(sInBuf, (int)wcslen(sInBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Returns the position of the last valid character in the string passed into sInBuf.
		//iInBufSz should be the length of the string being passed into sInBuf
		int GetLastCharPos(const WCHAR* sInBuf, int iInBufSz)
		{
			int iRPos = iInBufSz - 1;
			while (iRPos != -1)
			{
				if (!IsWhiteSpace(sInBuf[iRPos]))
				{
					return iRPos;
				}
				iRPos--;
			}
			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int GetLastCharPos(const WCHAR* sInBuf)
		{
			return GetLastCharPos(sInBuf, (int)wcslen(sInBuf));
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CharIndex(const WCHAR* sBuf, const WCHAR cChar)
		{
			return NSWFL::String::CharIndex(sBuf, (int)wcslen(sBuf), cChar);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CharIndex(const WCHAR* sBuf, int iBufSz, const WCHAR cChar)
		{
			for (int iPos = 0; iPos < iBufSz; iPos++)
			{
				if (sBuf[iPos] == cChar)
				{
					return iPos;
				}
			}

			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int SkipWhiteSpaces(const WCHAR* sBuf, int iBufSz, int iRPos)
		{
			while (NSWFL::String::IsWhiteSpace(sBuf[iRPos]) && iRPos < iBufSz)
			{
				iRPos++;
			}

			return iRPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int SkipWhiteSpaces(const WCHAR* sBuf, int iBufSz, int* iRPos)
		{
			while (NSWFL::String::IsWhiteSpace(sBuf[((int)*iRPos)]) && *iRPos < iBufSz)
			{
				((int)*iRPos)++;
			}

			return *iRPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsCharInArray(const WCHAR cTokChar, const WCHAR* sChars, unsigned int iChars)
		{
			for (unsigned int iChar = 0; iChar < iChars; iChar++)
			{
				if ((const WCHAR)cTokChar == (const WCHAR)(sChars[iChar]))
				{
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the size in characters of the token.
		*/
		int GetToken(const WCHAR* sBuf, int iBufSz, WCHAR* sOutToken, int iMaxTokSz,
			const WCHAR* sInvalidChars, int iInvalidCount)
		{
			return NSWFL::String::GetNextToken(sBuf, iBufSz, sOutToken,
				iMaxTokSz, (int)0, sInvalidChars, iInvalidCount);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the size in characters of the token.
		*/
		int GetToken(const WCHAR* sBuf, int iBufSz, WCHAR* sOutToken, int iMaxTokSz)
		{
			return NSWFL::String::GetNextToken(sBuf, iBufSz, sOutToken, iMaxTokSz, (int)0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the size in characters of the token.
		*/
		int GetNextToken(const WCHAR* sBuf, int iBufSz, WCHAR* sOutToken,
			int iMaxTokSz, int* iInOutRPos,
			const WCHAR* sInvalidChars, int iInvalidCount)
		{
			int iRPos = *iInOutRPos;
			int iWPos = 0;

			NSWFL::String::SkipWhiteSpaces(sBuf, iBufSz, &iRPos);

			if (sOutToken)
			{
				while (!NSWFL::String::IsCharInArray(sBuf[iRPos], (WCHAR*)sInvalidChars, iInvalidCount) && iRPos < iBufSz)
				{
					if (iWPos < iMaxTokSz)
					{
						sOutToken[iWPos++] = sBuf[iRPos++];
					}
					else {
						//We're going to continue to figure out how large this token is for informational purposes.
						iWPos++;
						iRPos++;
					}
				}
				if (iWPos < iMaxTokSz)
				{
					sOutToken[iWPos] = '\0';
				}
				else {
					sOutToken[iMaxTokSz - 1] = '\0'; //Truncate token.
				}
			}

			NSWFL::String::SkipWhiteSpaces(sBuf, iBufSz, &iRPos);

			*iInOutRPos = iRPos;

			return iWPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the size in characters of the token.
		*/
		int GetNextToken(const WCHAR* sBuf, int iBufSz, WCHAR* sOutToken,
			int iMaxTokSz, int* iInOutRPos)
		{
			int iRPos = *iInOutRPos;
			int iWPos = 0;

			NSWFL::String::SkipWhiteSpaces(sBuf, iBufSz, &iRPos);

			if (sOutToken)
			{
				while (!NSWFL::String::IsWhiteSpace(sBuf[iRPos]) && iRPos < iBufSz)
				{
					if (iWPos < iMaxTokSz)
					{
						sOutToken[iWPos++] = sBuf[iRPos++];
					}
					else {
						iWPos++;
						iRPos++;
					}
				}
				if (iWPos < iMaxTokSz)
				{
					sOutToken[iWPos] = '\0';
				}
				else {
					sOutToken[iMaxTokSz - 1] = '\0';
				}
			}

			NSWFL::String::SkipWhiteSpaces(sBuf, iBufSz, &iRPos);

			*iInOutRPos = iRPos;

			return iWPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the size in characters of the token.
		*/
		int GetNextToken(const WCHAR* sBuf, int iBufSz, WCHAR* sOutToken,
			int iMaxTokSz, int iStartPos,
			const WCHAR* sInvalidChars, int iInvalidCount)
		{
			int iInOutRPos = iStartPos;
			return NSWFL::String::GetNextToken(sBuf, iBufSz, sOutToken, iMaxTokSz, &iInOutRPos, sInvalidChars, iInvalidCount);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the size in characters of the token.
		*/
		int GetNextToken(const WCHAR* sBuf, int iBufSz, WCHAR* sOutToken, int iMaxTokSz, int iStartPos)
		{
			int iInOutRPos = iStartPos;
			return NSWFL::String::GetNextToken(sBuf, iBufSz, sOutToken, iMaxTokSz, &iInOutRPos);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Abbreviate(const WCHAR* sString, WCHAR* Abbr, int iMaxSz)
		{
			int iLen = (int)wcslen(sString);
			int iRPos = 0;
			int iWPos = 0;
			int iSpaces = 0;

			if (iLen > 0)
			{
				Abbr[iWPos++] = sString[iRPos];

				while (iRPos < iLen)
				{
					if (sString[iRPos] == ' ')
					{
						if (iLen > iRPos)
						{
							Abbr[iWPos++] = sString[iRPos + 1];
						}
						iSpaces++;
					}

					iRPos++;
				}
			}

			if (iSpaces == 0)
			{
				wcscpy_s(Abbr, iMaxSz, sString);
				return true;
			}

			Abbr[iWPos] = '\0';

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsMathChar(const WCHAR cChar)
		{
			if (cChar == '*' || cChar == '/' || cChar == '~' || cChar == '-'
				|| cChar == '+' || cChar == '>' || cChar == '<' || cChar == '!'
				|| cChar == '&' || cChar == '|' || cChar == '^' || cChar == '%')
			{
				return true;
			}

			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsAlNum(const WCHAR cChar)
		{
			return ((cChar >= 'A' && cChar <= 'Z') ||
				(cChar >= 'a' && cChar <= 'z') ||
				(cChar >= '0' && cChar <= '9'));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsAlNum(const WCHAR* sString, int iLength)
		{
			for (int i = 0; i < iLength; i++)
			{
				if (!IsAlNum(sString[i]))
				{
					return false;
				}
			}
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsAlpha(const WCHAR cChar)
		{
			return ((cChar >= 'A' && cChar <= 'Z') || (cChar >= 'a' && cChar <= 'z'));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsAlpha(const WCHAR* sString, int iLength)
		{
			for (int i = 0; i < iLength; i++)
			{
				if (!IsAlpha(sString[i]))
				{
					return false;
				}
			}
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsHexDigit(const WCHAR cChar)
		{
			return ((cChar >= 'A' && cChar <= 'F') ||
				(cChar >= 'a' && cChar <= 'f') ||
				(cChar >= '0' && cChar <= '9'));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Compare case-sensitive string.
		*/
		bool StrEql(const WCHAR* sOne, const WCHAR* sTwo)
		{
			while (*sOne == *sTwo && *sOne && *sTwo)
			{
				*sOne++;
				*sTwo++;
			}

			if (!*sOne && !*sTwo)
			{
				return true;
			}
			else return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Compare case-insensitive string.
		*/
		bool StrEqlI(const WCHAR* sOne, const WCHAR* sTwo)
		{
			while (*sOne && *sTwo && (*sOne <= 65 || *sOne <= 90 ? (*sOne) + 32 : (*sOne)) == (*sTwo <= 65 || *sTwo <= 90 ? (*sTwo) + 32 : (*sTwo)))
			{
				*sOne++;
				*sTwo++;
			}

			if (!*sOne && !*sTwo)
			{
				return true;
			}
			else return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Compare case-sensitive string.
		*/
		bool StrEql(const WCHAR* sOne, int iOneSz, const WCHAR* sTwo, int iTwoSz)
		{
			if (iOneSz != iTwoSz)
			{
				return false;
			}

			for (iOneSz = 0; iOneSz < iTwoSz; iOneSz++)
			{
				if (sOne[iOneSz] != sTwo[iOneSz])
				{
					return false;
				}
			}

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Compare case-insensitive string.
		*/
		bool StrEqlI(const WCHAR* sOne, int iOneSz, const WCHAR* sTwo, int iTwoSz)
		{
			if (iOneSz != iTwoSz)
			{
				return false;
			}

			for (iOneSz = 0; iOneSz < iTwoSz; iOneSz++)
			{
				if (!((sOne[iOneSz] <= 65 || sOne[iOneSz] <= 90 ? (sOne[iOneSz]) + 32 : (sOne[iOneSz]))
					== (sTwo[iOneSz] <= 65 || sTwo[iOneSz] <= 90 ? (sTwo[iOneSz]) + 32 : (sTwo[iOneSz]))))
				{
					return false;
				}
			}

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* StrnDup(const WCHAR* sString, size_t iLength)
		{
			WCHAR* sNew = (WCHAR*)malloc(iLength + 1);
			if (sNew)
			{
				memcpy(sNew, sString, iLength);
				sNew[iLength] = '\0';
			}
			return sNew;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool NewGUID(WCHAR* sOut, int iMaxOutSz)
		{
			GUID lGUID;
			memset(&lGUID, 0, sizeof(GUID));
			if (CoCreateGuid(&lGUID) == S_OK)
			{
				if (NSWFL::Conversion::Bin2Hex(sOut, iMaxOutSz, &lGUID, sizeof(GUID), 2, false) == 32)
				{
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::String
} //namespace::NSWFL
#endif
