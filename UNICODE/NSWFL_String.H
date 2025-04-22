///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_STRING_H_
#define _NSWFL_STRING_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace String {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsUpper(WCHAR cIn);
		bool IsLower(WCHAR cIn);
		WCHAR ToUpper(WCHAR cIn);
		WCHAR ToLower(WCHAR cIn);
		WCHAR* UCase(WCHAR* sBuf, int iBufSz);
		WCHAR* LCase(WCHAR* sBuf, int iBufSz);
		WCHAR* UCase(WCHAR* sBuf);
		WCHAR* LCase(WCHAR* sBuf);

		WCHAR* Space(size_t iSpaces);
		WCHAR* Space(WCHAR* sBuffer, size_t iSpaces);

		int IndexOf(const WCHAR* sBuf, const WCHAR cCharacter);
		int IndexOf(const WCHAR* sBuf, int iBufSz, const WCHAR cCharacter);
		int LastIndexOf(const WCHAR* sBuf, const WCHAR cCharacter);
		int LastIndexOf(const WCHAR* sBuf, int iBufSz, const WCHAR cCharacter);

		int GetToken(const WCHAR* sBuf, int iBufSz, WCHAR* sOutToken, int iMaxTokSz);

		int GetNextToken(const WCHAR* sBuf, int iBufSz, WCHAR* sOutToken,
			int iMaxTokSz, int* iInOutRPos);

		int GetNextToken(const WCHAR* sBuf, int iBufSz, WCHAR* sOutToken,
			int iMaxTokSz, int iStartPos);

		int GetToken(const WCHAR* sBuf, int iBufSz, WCHAR* sOutToken, int iMaxTokSz,
			const WCHAR* sInvalidChars, int iInvalidCount);

		int GetNextToken(const WCHAR* sBuf, int iBufSz, WCHAR* sOutToken, int iMaxTokSz,
			int* iInOutRPos, const WCHAR* sInvalidChars, int iInvalidCount);

		int GetNextToken(const WCHAR* sBuf, int iBufSz, WCHAR* sOutToken, int iMaxTokSz,
			int iStartPos, const WCHAR* sInvalidChars, int iInvalidCount);

		int ReplaceStrings(const WCHAR* sInBuf, const WCHAR* sReplace, const WCHAR* sWith, WCHAR* sOutBuf, int iMaxSz);
		int ReplaceStrings(const WCHAR* sInBuf, const WCHAR* sReplace, int iReplaceLen,
			const WCHAR* sWith, WCHAR* sOutBuf, int iMaxSz);

		int ReplaceStringsI(const WCHAR* sInBuf, const WCHAR* sReplace, const WCHAR* sWith, WCHAR* sOutBuf, int iMaxSz);
		int ReplaceStringsI(const WCHAR* sInBuf, const WCHAR* sReplace, int iReplaceLen,
			const WCHAR* sWith, WCHAR* sOutBuf, int iMaxSz);

		int ReplaceCharacter(WCHAR* sInBuf, int iBufferLength, const WCHAR cReplace, const WCHAR cWith);
		int ReplaceCharacter(const WCHAR* sInBuf, WCHAR* sOutBuf, const WCHAR cReplace, const WCHAR cWith);
		int ReplaceCharacter(WCHAR* sInOutBuf, const WCHAR cReplace, const WCHAR cWith);
		bool ReverseString(WCHAR* sBuf, int iBufSz);
		int InStr(const WCHAR* sSearchFor, int sSearchForSz, const WCHAR* sInBuf, const int iBufSz, const int iStartPos);
		int InStr(const WCHAR* sSearchFor, const WCHAR* sInBuf, const int iBufSz, const int iStartPos);
		int InStr(const WCHAR* sSearchFor, const WCHAR* sInBuf);
		int InStrI(const WCHAR* sSearchFor, int sSearchForSz, const WCHAR* sInBuf, const int iBufSz, const int iStartPos);
		int InStrI(const WCHAR* sSearchFor, const WCHAR* sInBuf, const int iBufSz, const int iStartPos);
		int InStrI(const WCHAR* sSearchFor, const WCHAR* sInBuf);
		bool CompareBinString(const WCHAR* sBuf1, int iStrSz1, const WCHAR* sBuf2, int iStrSz2);
		int StringScan(const WCHAR* sInBuf, int iStartPos, int iLength, WCHAR* sOutVal, int iMaxSz);
		int AppendStrings(const WCHAR* sBuf1, const WCHAR* sBuf2, int iBuf1Sz, int iBuf2Sz, WCHAR* sOutBuf, int iMaxSz);
		int CompareFlagedString(const WCHAR* sBuf, const WCHAR* sFlag);
		int GetFlagedValue(const WCHAR* sBuf, int iSkpLen, int iBufSz, WCHAR* sOutBuf, int iMaxSz);
		WCHAR* SimpleCipherString(WCHAR* sBuf, int iBufSz);
		int CopyString(WCHAR* sOutBuf, int iMaxSz, const WCHAR* sInBuf, int iInBufSz);

		bool StrEqlI(const WCHAR* sOne, const WCHAR* sTwo);
		bool StrEql(const WCHAR* sOne, const WCHAR* sTwo);

		bool StrEql(const WCHAR* sOne, int iOneSz, const WCHAR* sTwo, int iTwoSz);
		bool StrEqlI(const WCHAR* sOne, int iOneSz, const WCHAR* sTwo, int iTwoSz);

		WCHAR** Split(const WCHAR* sBuf, int iBufSz, int* iOutItemCount, const WCHAR* sSplitOn, int iSplitOnSz);
		WCHAR** Split(const WCHAR* sBuf, int iBufSz, int* iOutItemCount, const WCHAR* sSplitOn);
		WCHAR** Split(const WCHAR* sBuf, const WCHAR* sSplitOn);
		WCHAR** Split(const WCHAR* sBuf, int iBufSz, const WCHAR* sSplitOn);
		void FreeArray(WCHAR** sArray, int iItems);
		void FreeArray(WCHAR** sArray);
		int UBound(WCHAR** sArray);

		bool IsNumeric(const WCHAR cIn);
		bool IsNumeric(const WCHAR* sText);
		bool IsNumeric(const WCHAR* sText, int iLength);

		int GetFirstCharPos(const WCHAR* sInBuf, int iInBufSz);
		int GetFirstCharPos(const WCHAR* sInBuf);
		int GetLastCharPos(const WCHAR* sInBuf, int iInBufSz);
		int GetLastCharPos(const WCHAR* sInBuf);

		bool IsWhiteSpace(WCHAR cChar);

		int SkipWhiteSpaces(const WCHAR* sBuf, int iBufSz, int* iRPos);
		int SkipWhiteSpaces(const WCHAR* sBuf, int iBufSz, int iRPos);
		int CleanWhiteSpaces(WCHAR* sInOutBuf, int iLen);
		int CleanWhiteSpaces(WCHAR* sInOutBuf);

		int Mid(const WCHAR* sInBuf, int iInBufSz, int iStartPos, int iRChars, WCHAR* sOutBuf, int iMaxSz);
		int StrCat(WCHAR* sDest, int iMaxSz, int iDestLen, const WCHAR* sAppend, int iAppendLen);
		int Right(WCHAR* sOutBuf, int iMaxSz, const WCHAR* sInBuf, int iInLen, int iRight);
		int Left(WCHAR* sOutBuf, int iMaxSz, const WCHAR* sInBuf, int iInLen, int iLeft);

		int Trim(WCHAR* sInOutBuf, int iLen);
		int Trim(WCHAR* sInOutBuf);
		int LTrim(WCHAR* sInOutBuf, int iLen);
		int RTrim(WCHAR* sInOutBuf, int iLen);
		int LTrim(WCHAR* sInOutBuf);
		int RTrim(WCHAR* sInOutBuf);

		int CountOccurrences(const WCHAR cLookFor, const WCHAR* sInStr);
		int CountOccurrences(const WCHAR cLookFor, const WCHAR* sInStr, int iInStrSz);
		int CountOccurrences(const WCHAR* sLookFor, const WCHAR* sInStr, int iInStrSz);
		int CountOccurrences(const WCHAR* sLookFor, const WCHAR* sInStr);

		int CharIndex(const WCHAR* sBuf, const WCHAR cChar);
		int CharIndex(const WCHAR* sBuf, int iBufSz, const WCHAR cChar);

		bool Abbreviate(const WCHAR* sString, WCHAR* Abbr, int iMaxSz);

		bool IsMathChar(const WCHAR cChar);
		bool IsAlNum(const WCHAR cChar);
		bool IsAlpha(const WCHAR* sString, int iLength);
		bool IsAlpha(const WCHAR cChar);
		bool IsAlNum(const WCHAR* sString, int iLength);
		bool IsHexDigit(const WCHAR cChar);

		bool IsCharInArray(const WCHAR cTokChar, const WCHAR* sChars, unsigned int iChars);

		WCHAR* StrnDup(const WCHAR* sString, size_t iLength);

		bool NewGUID(WCHAR* sOut, int iMaxOutSz);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::String
} //namespace::NSWFL
#endif
