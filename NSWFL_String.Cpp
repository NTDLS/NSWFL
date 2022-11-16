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
extern NSWFL::Memory::MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace String {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//The returned pointer must be freed externally.
		char *Space(size_t iSpaces)
		{
#ifdef _USE_GLOBAL_MEMPOOL
			char *sSpaces = (char *)pMem->Allocate(iSpaces + 1, 1);
#else
			char *sSpaces = (char *)malloc(iSpaces + 1);
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

		char *Space(char *sBuffer, size_t iSpaces)
		{
			memset(sBuffer, ' ', iSpaces);
			sBuffer[iSpaces] = '\0';
			return sBuffer;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsUpper(char cIn)
		{
			return (cIn >= 65 && cIn <= 90);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsLower(char cIn)
		{
			return (cIn >= 97 && cIn <= 122);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char ToUpper(char cIn)
		{
			if (IsLower(cIn))
			{
				return (cIn - 32);
			}
			return cIn;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char ToLower(char cIn)
		{
			if (IsUpper(cIn))
			{
				return (cIn + 32);
			}
			return cIn;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int IndexOf(const char *sBuf, const char cCharacter)
		{
			return NSWFL::String::IndexOf(sBuf, (int)strlen(sBuf), cCharacter);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int IndexOf(const char *sBuf, int iBufSz, const char cCharacter)
		{
			for (int iPos = 0; iPos < iBufSz; iPos++)
			{
				if ((unsigned char)sBuf[iPos] == (unsigned char)cCharacter)
				{
					return iPos;
				}
			}
			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LastIndexOf(const char *sBuf, const char cCharacter)
		{
			return NSWFL::String::LastIndexOf(sBuf, (int)strlen(sBuf), cCharacter);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LastIndexOf(const char *sBuf, int iBufSz, const char cCharacter)
		{
			if (iBufSz > 0)
			{
				for (int iPos = iBufSz - 1; iPos > -1; iPos--)
				{
					if ((unsigned char)sBuf[iPos] == (unsigned char)cCharacter)
					{
						return iPos;
					}
				}
			}
			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char *UCase(char *sBuf, int iBufSz)
		{
			for (int iPos = 0; iPos < iBufSz; iPos++)
			{
				sBuf[iPos] = NSWFL::String::ToUpper(sBuf[iPos]);
			}
			return sBuf;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char *LCase(char *sBuf, int iBufSz)
		{
			for (int iPos = 0; iPos < iBufSz; iPos++)
			{
				sBuf[iPos] = NSWFL::String::ToLower(sBuf[iPos]);
			}
			return sBuf;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char *UCase(char *sBuf)
		{
			return NSWFL::String::UCase(sBuf, (int)strlen(sBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char *LCase(char *sBuf)
		{
			return NSWFL::String::LCase(sBuf, (int)strlen(sBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Trim(char *sInOutBuf, int iLen)
		{
			int iLength = NSWFL::String::LTrim(sInOutBuf, iLen);
			return NSWFL::String::RTrim(sInOutBuf, iLen);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Trim(char *sInOutBuf)
		{
			return NSWFL::String::Trim(sInOutBuf, (int)strlen(sInOutBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int LTrim(char *sInOutBuf, int iLen)
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

		int LTrim(char *sInOutBuf)
		{
			return NSWFL::String::LTrim(sInOutBuf, (int)strlen(sInOutBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int RTrim(char *sInOutBuf, int iLen)
		{
			if (iLen > 0)
			{
				iLen = (NSWFL::String::GetLastCharPos(sInOutBuf, iLen) + 1);
				sInOutBuf[iLen] = '\0'; //replace the NULL terminator.
			}
			return iLen;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int RTrim(char *sInOutBuf)
		{
			return NSWFL::String::RTrim(sInOutBuf, (int)strlen(sInOutBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//sInOutBuf returns the string passed into it with all white spaces
		//	trimmed off the beginning and end of it.
		int CleanWhiteSpaces(char *sInOutBuf, int iLen)
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

		int CleanWhiteSpaces(char *sInOutBuf)
		{
			return NSWFL::String::CleanWhiteSpaces(sInOutBuf, (int)strlen(sInOutBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ReplaceStringsI(const char *sInBuf, const char *sReplace, const char *sWith, char *sOutBuf, int iMaxSz)
		{
			return NSWFL::String::ReplaceStringsI(sInBuf, sReplace, (int)strlen(sReplace), sWith, sOutBuf, iMaxSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ReplaceStringsI(const char *sInBuf, const char *sReplace, int iReplaceLen,
			const char *sWith, char *sOutBuf, int iMaxSz)
		{
			const char *sNULL = NULL;

			int iReplaces = 0;

			if (sInBuf == NULL || sOutBuf == NULL)
			{
				return -1;
			}

			strcpy_s(sOutBuf, iMaxSz, "");

			for (sNULL = sInBuf; *sNULL; ++sNULL)
			{
				if (_strnicmp(sNULL, sReplace, iReplaceLen) == 0)
				{
					strcat_s(sOutBuf, iMaxSz, sWith);
					sNULL += (iReplaceLen - 1);
					iReplaces++;
				}
				else strncat_s(sOutBuf, iMaxSz, sNULL, 1);
			}

			return iReplaces;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ReplaceStrings(const char *sInBuf, const char *sReplace, const char *sWith, char *sOutBuf, int iMaxSz)
		{
			return NSWFL::String::ReplaceStrings(sInBuf, sReplace, (int)strlen(sReplace), sWith, sOutBuf, iMaxSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ReplaceStrings(const char *sInBuf, const char *sReplace, int iReplaceLen,
			const char *sWith, char *sOutBuf, int iMaxSz)
		{
			const char *sNULL = NULL;

			int iReplaces = 0;

			if (sInBuf == NULL || sOutBuf == NULL)
			{
				return -1;
			}

			strcpy_s(sOutBuf, iMaxSz, "");

			for (sNULL = sInBuf; *sNULL; ++sNULL)
			{
				if (strncmp(sNULL, sReplace, iReplaceLen) == 0)
				{
					strcat_s(sOutBuf, iMaxSz, sWith);
					sNULL += (iReplaceLen - 1);
					iReplaces++;
				}
				else strncat_s(sOutBuf, iMaxSz, sNULL, 1);
			}

			return iReplaces;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int ReplaceCharacter(char *sInBuf, int iBufferLength, const char cReplace, const char cWith)
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

		int ReplaceCharacter(const char *sInBuf, char *sOutBuf, const char cReplace, const char cWith)
		{
			int iPos = 0;
			int iOccur = 0;
			int iLength = (int)strlen(sInBuf);

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

		int ReplaceCharacter(char *sInOutBuf, const char cReplace, const char cWith)
		{
			return NSWFL::String::ReplaceCharacter(sInOutBuf, sInOutBuf, cReplace, cWith);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool ReverseString(char *sBuf, int iBufSz)
		{
			char *String1 = NULL;
			char *String2 = NULL;

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

		int InStrI(const char *sSearchFor, int sSearchForSz, const char *sInBuf, const int iBufSz, const int iStartPos)
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

		int InStr(const char *sSearchFor, int sSearchForSz, const char *sInBuf, const int iBufSz, const int iStartPos)
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

		int InStr(const char *sSearchFor, const char *sInBuf, const int iBufSz, const int iStartPos)
		{
			return InStr(sSearchFor, (int)strlen(sSearchFor), sInBuf, iBufSz, iStartPos);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InStr(const char *sSearchFor, const char *sInBuf)
		{
			return InStr(sSearchFor, sInBuf, (int)strlen(sInBuf), 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InStrI(const char *sSearchFor, const char *sInBuf, const int iBufSz, const int iStartPos)
		{
			return InStrI(sSearchFor, (int)strlen(sSearchFor), sInBuf, iBufSz, iStartPos);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int InStrI(const char *sSearchFor, const char *sInBuf)
		{
			return InStrI(sSearchFor, sInBuf, (int)strlen(sInBuf), 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CompareBinString(const char *sBuf1, int iStrSz1, const char *sBuf2, int iStrSz2)
		{
			if (iStrSz1 > iStrSz2) //Use the shortest string.
			{
				iStrSz1 = iStrSz2;
			}

			return (memcmp(sBuf1, sBuf2, iStrSz1) == 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringScan(const char *sInBuf, int iStartPos, int iLength, char *sOutVal, int iMaxSz)
		{
			strncpy_s(sOutVal, iMaxSz, &sInBuf[iStartPos], iLength);
			return iLength;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int AppendStrings(const char *sBuf1, const char *sBuf2, int iBuf1Sz, int iBuf2Sz, char *sOutBuf, int iMaxSz)
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

		int CompareFlagedString(const char *sBuf, const char *sFlag)
		{
			int iFlagLength = (int)strlen(sFlag);

			if (strncmp(sBuf, sFlag, iFlagLength) == 0)
			{
				return iFlagLength;
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int GetFlagedValue(const char *sBuf, int iSkpLen, int iBufSz, char *sOutBuf, int iMaxSz)
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

		char *SimpleCipherString(char *sBuf, int iBufSz)
		{
			for (int iPos = 0; iPos < iBufSz; iPos++)
			{
				((unsigned char *)sBuf)[iPos] += (unsigned char)128;
			}

			return sBuf;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CopyString(char *sOutBuf, int iMaxSz, const char *sInBuf, int iInBufSz)
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

		int CountOccurrences(const char cLookFor, const char *sInStr, int iInStrSz)
		{
			int iOccurrences = 0;
			for (int iPos = 0; iPos < iInStrSz; iPos++)
			{
				if ((unsigned char)sInStr[iPos] == (unsigned char)cLookFor)
				{
					iOccurrences++;
				}
			}

			return iOccurrences;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Returns the count of the Occurrences of "cLookFor" that are found in "sInStr".

		int CountOccurrences(const char cLookFor, const char *sInStr)
		{
			return CountOccurrences(cLookFor, sInStr, (int)strlen(sInStr));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Returns the count of the Occurrences of "sLookFor" that are found in "sInStr".

		int CountOccurrences(const char *sLookFor, const char *sInStr, int iInStrSz)
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

		int CountOccurrences(const char *sLookFor, const char *sInStr)
		{
			return CountOccurrences(sLookFor, sInStr, (int)strlen(sInStr));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns a zero-based, one-dimensional array containing a specified number of substrings.

			Example:

				char **sArray = NULL;
				char *sBuf = "Hello\tCruel\tWorld\tFrom\tC\tPlus\tPlus\t";
				int iItems = 0;
				int iItem = 0;

				sArray = SplitEx(sBuf, strlen(sBuf), sArray, &iItems, strlen("\t"), 1);

				printf("Array Items: %d\n", UBound(sArray));

				while(iItem < iItems)
				{
					printf("Item[%d]: %s\n", iItem, sArray[iItem]);
					iItem++;
				}

				FreeSplitStringEx(sArray, iItems);
		*/

		char **Split(const char *sBuf, int iBufSz, int *iOutItemCount, const char *sSplitOn, int iSplitOnSz)
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
			char **sArray = (char **)pMem->Allocate(sizeof(char *), iItemCount + 1);
#else
			char **sArray = (char **)calloc(sizeof(char *), iItemCount + 1);
#endif

			while ((iStartPos = InStr(sSplitOn, iSplitOnSz, sBuf, iBufSz, iRPos)) >= 0)
			{
				//Allocate enough RAM to hold the sub-string.
#ifdef _USE_GLOBAL_MEMPOOL
				sArray[iItems] = (char *)pMem->Allocate(sizeof(char), (iStartPos - iRPos) + 1);
#else
				sArray[iItems] = (char *)calloc(sizeof(char), (iStartPos - iRPos) + 1);
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
				sArray[iItems] = (char *)pMem->Allocate(sizeof(char), (iBufSz - iRPos) + 1);
#else
				sArray[iItems] = (char *)calloc(sizeof(char), (iBufSz - iRPos) + 1);
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

		char **Split(const char *sBuf, int iBufSz, int *iOutItemCount, const char *sSplitOn)
		{
			return Split(sBuf, iBufSz, iOutItemCount, sSplitOn, (int)strlen(sSplitOn));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns a zero-based, one-dimensional array containing a specified number of substrings.

			Example:

				char **sArray = NULL;
				char *sBuf = "Hello\tCruel\tWorld\tFrom\tC\tPlus\tPlus\t";
				int iItem = 0;

				sArray = Split(sBuf, "\t");

				printf("Array Items: %d\n", UBound(sArray));

				while(sArray[iItem] != NULL)
				{
					printf("Item[%d]: %s\n", iItem, sArray[iItem]);
					iItem++;
				}

				FreeSplitString(sArray);
		*/

		char **Split(const char *sBuf, const char *sSplitOn)
		{
			return Split(sBuf, (int)strlen(sBuf), NULL, sSplitOn, (int)strlen(sSplitOn));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char **Split(const char *sBuf, int iBufSz, const char *sSplitOn)
		{
			return Split(sBuf, iBufSz, NULL, sSplitOn, (int)strlen(sSplitOn));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function will free all the RAM allocated by an array.

			All array items, and the array its self.

			Intended for use with the Split() and SplitEx functions.
		*/
		void FreeArray(char **sArray, int iItems)
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
		void FreeArray(char **sArray)
		{
			NSWFL::String::FreeArray(sArray, NSWFL::String::UBound(sArray));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function will return the number of items in a char *Array[].
			This function will malfunction if the last item in the array is not NULL.

			Intended for use with the Split() and SplitEx functions.
		*/
		int UBound(char **sArray)
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
		bool IsNumeric(const char cIn)
		{
			return (cIn >= 48 && cIn <= 57);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Returns true if the passed string is in the following formats:
			//0, -0.0, 0.0, -0, -0.0
		bool IsNumeric(const char *sText, int iLength)
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

		bool IsNumeric(const char *sText)
		{
			return IsNumeric(sText, (int)strlen(sText));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Mid(const char *sInBuf, int iInBufSz, int iStartPos, int iRChars, char *sOutBuf, int iMaxSz)
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

		int StrCat(char *sDest, int iMaxSz, int iDestLen, const char *sAppend, int iAppendLen)
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

		int Right(char *sOutBuf, int iMaxSz, const char *sInBuf, int iInLen, int iRight)
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
				strcpy_s(sOutBuf, iMaxSz, "");
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

		int Left(char *sOutBuf, int iMaxSz, const char *sInBuf, int iInLen, int iLeft)
		{
			int iLen = iInLen;
			int iRPos = 0;

			if ((iLeft + 1) > iMaxSz)
			{
				return -1;
			}

			if (iLeft > iInLen)
			{
				strcpy_s(sOutBuf, iMaxSz, "");
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
		bool IsWhiteSpace(char cChar)
		{
			return NSWFL::String::IsWhiteSpace((unsigned char)cChar);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Returns true if the character passed into cChar is a whitespace, otherwise it returns false.
		bool IsWhiteSpace(unsigned char cChar)
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
		int GetFirstCharPos(const char *sInBuf, int iInBufSz)
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

		int GetFirstCharPos(const char *sInBuf)
		{
			return GetFirstCharPos(sInBuf, (int)strlen(sInBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Returns the position of the last valid character in the string passed into sInBuf.
		//iInBufSz should be the length of the string being passed into sInBuf
		int GetLastCharPos(const char *sInBuf, int iInBufSz)
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

		int GetLastCharPos(const char *sInBuf)
		{
			return GetLastCharPos(sInBuf, (int)strlen(sInBuf));
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CharIndex(const char *sBuf, const char cChar)
		{
			return NSWFL::String::CharIndex(sBuf, (int)strlen(sBuf), cChar);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CharIndex(const char *sBuf, int iBufSz, const char cChar)
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

		int SkipWhiteSpaces(const char *sBuf, int iBufSz, int iRPos)
		{
			while (NSWFL::String::IsWhiteSpace(sBuf[iRPos]) && iRPos < iBufSz)
			{
				iRPos++;
			}

			return iRPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int SkipWhiteSpaces(const char *sBuf, int iBufSz, int *iRPos)
		{
			while (NSWFL::String::IsWhiteSpace(sBuf[((int)*iRPos)]) && *iRPos < iBufSz)
			{
				((int)*iRPos)++;
			}

			return *iRPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsCharInArrayU(const unsigned char cTokChar, const unsigned char *sChars, unsigned int iChars)
		{
			for (unsigned int iChar = 0; iChar < iChars; iChar++)
			{
				if (cTokChar == sChars[iChar])
				{
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsCharInArrayS(const char cTokChar, const char *sChars, unsigned int iChars)
		{
			for (unsigned int iChar = 0; iChar < iChars; iChar++)
			{
				if ((const unsigned char)cTokChar == (const unsigned char)(sChars[iChar]))
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
		int GetToken(const char *sBuf, int iBufSz, char *sOutToken, int iMaxTokSz,
			const char *sInvalidChars, int iInvalidCount)
		{
			return NSWFL::String::GetNextToken(sBuf, iBufSz, sOutToken,
				iMaxTokSz, (int)0, sInvalidChars, iInvalidCount);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the size in characters of the token.
		*/
		int GetToken(const char *sBuf, int iBufSz, char *sOutToken, int iMaxTokSz)
		{
			return NSWFL::String::GetNextToken(sBuf, iBufSz, sOutToken, iMaxTokSz, (int)0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the size in characters of the token.
		*/
		int GetNextToken(const char *sBuf, int iBufSz, char *sOutToken,
			int iMaxTokSz, int *iInOutRPos,
			const char *sInvalidChars, int iInvalidCount)
		{
			int iRPos = *iInOutRPos;
			int iWPos = 0;

			NSWFL::String::SkipWhiteSpaces(sBuf, iBufSz, &iRPos);

			if (sOutToken)
			{
				while (!NSWFL::String::IsCharInArrayU(sBuf[iRPos], (unsigned char *)sInvalidChars, iInvalidCount) && iRPos < iBufSz)
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
		int GetNextToken(const char *sBuf, int iBufSz, char *sOutToken,
			int iMaxTokSz, int *iInOutRPos)
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
		int GetNextToken(const char *sBuf, int iBufSz, char *sOutToken,
			int iMaxTokSz, int iStartPos,
			const char *sInvalidChars, int iInvalidCount)
		{
			int iInOutRPos = iStartPos;
			return NSWFL::String::GetNextToken(sBuf, iBufSz, sOutToken, iMaxTokSz, &iInOutRPos, sInvalidChars, iInvalidCount);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the size in characters of the token.
		*/
		int GetNextToken(const char *sBuf, int iBufSz, char *sOutToken, int iMaxTokSz, int iStartPos)
		{
			int iInOutRPos = iStartPos;
			return NSWFL::String::GetNextToken(sBuf, iBufSz, sOutToken, iMaxTokSz, &iInOutRPos);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Abbreviate(const char *sString, char *Abbr, int iMaxSz)
		{
			int iLen = (int)strlen(sString);
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
				strcpy_s(Abbr, iMaxSz, sString);
				return true;
			}

			Abbr[iWPos] = '\0';

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsMathChar(const char cChar)
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

		bool IsAlNum(const char cChar)
		{
			return ((cChar >= 'A' && cChar <= 'Z') ||
				(cChar >= 'a' && cChar <= 'z') ||
				(cChar >= '0' && cChar <= '9'));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsAlNum(const char *sString, int iLength)
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

		bool IsAlpha(const char cChar)
		{
			return ((cChar >= 'A' && cChar <= 'Z') || (cChar >= 'a' && cChar <= 'z'));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsAlpha(const char *sString, int iLength)
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

		bool IsHexDigit(const char cChar)
		{
			return ((cChar >= 'A' && cChar <= 'F') ||
				(cChar >= 'a' && cChar <= 'f') ||
				(cChar >= '0' && cChar <= '9'));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Compare case-sensitive string.
		*/
		bool StrEql(const char *sOne, const char *sTwo)
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
		bool StrEqlI(const char *sOne, const char *sTwo)
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
		bool StrEql(const char *sOne, int iOneSz, const char *sTwo, int iTwoSz)
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
		bool StrEqlI(const char *sOne, int iOneSz, const char *sTwo, int iTwoSz)
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

		char *StrnDup(const char* sString, size_t iLength)
		{
			char *sNew = (char*)malloc(iLength + 1);
			if (sNew)
			{
				memcpy(sNew, sString, iLength);
				sNew[iLength] = '\0';
			}
			return sNew;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool NewGUID(char *sOut, int iMaxOutSz)
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
