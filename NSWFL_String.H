///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
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

		bool IsUpper(char cIn);
		bool IsLower(char cIn);
		char ToUpper(char cIn);
		char ToLower(char cIn);
		char * UCase(char *sBuf, int iBufSz);
		char * LCase(char *sBuf, int iBufSz);
		char * UCase(char *sBuf);
		char * LCase(char *sBuf);

		char *Space(size_t iSpaces);
		char *Space(char *sBuffer, size_t iSpaces);

		int IndexOf(const char *sBuf, const char cCharacter);
		int IndexOf(const char *sBuf, int iBufSz, const char cCharacter);
		int LastIndexOf(const char *sBuf, const char cCharacter);
		int LastIndexOf(const char *sBuf, int iBufSz, const char cCharacter);

		int GetToken(const char *sBuf, int iBufSz, char *sOutToken, int iMaxTokSz);

		int GetNextToken(const char *sBuf, int iBufSz, char *sOutToken,
			int iMaxTokSz, int *iInOutRPos);

		int GetNextToken(const char *sBuf, int iBufSz, char *sOutToken,
			int iMaxTokSz, int iStartPos);

		int GetToken(const char *sBuf, int iBufSz, char *sOutToken, int iMaxTokSz,
			const char *sInvalidChars, int iInvalidCount);

		int GetNextToken(const char *sBuf, int iBufSz, char *sOutToken, int iMaxTokSz,
			int *iInOutRPos, const char *sInvalidChars, int iInvalidCount);

		int GetNextToken(const char *sBuf, int iBufSz, char *sOutToken, int iMaxTokSz,
			int iStartPos, const char *sInvalidChars, int iInvalidCount);

		int ReplaceStrings(const char *sInBuf, const char *sReplace, const char *sWith, char *sOutBuf, int iMaxSz);
		int ReplaceStrings(const char *sInBuf, const char *sReplace, int iReplaceLen,
			const char *sWith, char *sOutBuf, int iMaxSz);

		int ReplaceStringsI(const char *sInBuf, const char *sReplace, const char *sWith, char *sOutBuf, int iMaxSz);
		int ReplaceStringsI(const char *sInBuf, const char *sReplace, int iReplaceLen,
			const char *sWith, char *sOutBuf, int iMaxSz);

		int ReplaceCharacter(char *sInBuf, int iBufferLength, const char cReplace, const char cWith);
		int ReplaceCharacter(const char *sInBuf, char *sOutBuf, const char cReplace, const char cWith);
		int ReplaceCharacter(char *sInOutBuf, const char cReplace, const char cWith);
		bool ReverseString(char *sBuf, int iBufSz);
		int InStr(const char *sSearchFor, int sSearchForSz, const char *sInBuf, const int iBufSz, const int iStartPos);
		int InStr(const char *sSearchFor, const char *sInBuf, const int iBufSz, const int iStartPos);
		int InStr(const char *sSearchFor, const char *sInBuf);
		int InStrI(const char *sSearchFor, int sSearchForSz, const char *sInBuf, const int iBufSz, const int iStartPos);
		int InStrI(const char *sSearchFor, const char *sInBuf, const int iBufSz, const int iStartPos);
		int InStrI(const char *sSearchFor, const char *sInBuf);
		bool CompareBinString(const char *sBuf1, int iStrSz1, const char *sBuf2, int iStrSz2);
		int StringScan(const char *sInBuf, int iStartPos, int iLength, char *sOutVal, int iMaxSz);
		int AppendStrings(const char *sBuf1, const char *sBuf2, int iBuf1Sz, int iBuf2Sz, char *sOutBuf, int iMaxSz);
		int CompareFlagedString(const char *sBuf, const char *sFlag);
		int GetFlagedValue(const char *sBuf, int iSkpLen, int iBufSz, char *sOutBuf, int iMaxSz);
		char *SimpleCipherString(char *sBuf, int iBufSz);
		int CopyString(char *sOutBuf, int iMaxSz, const char *sInBuf, int iInBufSz);

		bool StrEqlI(const char *sOne, const char *sTwo);
		bool StrEql(const char *sOne, const char *sTwo);

		bool StrEql(const char *sOne, int iOneSz, const char *sTwo, int iTwoSz);
		bool StrEqlI(const char *sOne, int iOneSz, const char *sTwo, int iTwoSz);

		char **Split(const char *sBuf, int iBufSz, int *iOutItemCount, const char *sSplitOn, int iSplitOnSz);
		char **Split(const char *sBuf, int iBufSz, int *iOutItemCount, const char *sSplitOn);
		char **Split(const char *sBuf, const char *sSplitOn);
		char **Split(const char *sBuf, int iBufSz, const char *sSplitOn);
		void FreeArray(char **sArray, int iItems);
		void FreeArray(char **sArray);
		int UBound(char **sArray);

		bool IsNumeric(const char cIn);
		bool IsNumeric(const char *sText);
		bool IsNumeric(const char *sText, int iLength);

		int GetFirstCharPos(const char *sInBuf, int iInBufSz);
		int GetFirstCharPos(const char *sInBuf);
		int GetLastCharPos(const char *sInBuf, int iInBufSz);
		int GetLastCharPos(const char *sInBuf);

		bool IsWhiteSpace(char cChar);
		bool IsWhiteSpace(unsigned char cChar);

		int SkipWhiteSpaces(const char *sBuf, int iBufSz, int *iRPos);
		int SkipWhiteSpaces(const char *sBuf, int iBufSz, int iRPos);
		int CleanWhiteSpaces(char *sInOutBuf, int iLen);
		int CleanWhiteSpaces(char *sInOutBuf);

		int Mid(const char *sInBuf, int iInBufSz, int iStartPos, int iRChars, char *sOutBuf, int iMaxSz);
		int StrCat(char *sDest, int iMaxSz, int iDestLen, const char *sAppend, int iAppendLen);
		int Right(char *sOutBuf, int iMaxSz, const char *sInBuf, int iInLen, int iRight);
		int Left(char *sOutBuf, int iMaxSz, const char *sInBuf, int iInLen, int iLeft);

		int Trim(char *sInOutBuf, int iLen);
		int Trim(char *sInOutBuf);
		int LTrim(char *sInOutBuf, int iLen);
		int RTrim(char *sInOutBuf, int iLen);
		int LTrim(char *sInOutBuf);
		int RTrim(char *sInOutBuf);

		int CountOccurrences(const char cLookFor, const char *sInStr);
		int CountOccurrences(const char cLookFor, const char *sInStr, int iInStrSz);
		int CountOccurrences(const char *sLookFor, const char *sInStr, int iInStrSz);
		int CountOccurrences(const char *sLookFor, const char *sInStr);

		int CharIndex(const char *sBuf, const char cChar);
		int CharIndex(const char *sBuf, int iBufSz, const char cChar);

		bool Abbreviate(const char *sString, char *Abbr, int iMaxSz);

		bool IsMathChar(const char cChar);
		bool IsAlNum(const char cChar);
		bool IsAlpha(const char *sString, int iLength);
		bool IsAlpha(const char cChar);
		bool IsAlNum(const char *sString, int iLength);
		bool IsHexDigit(const char cChar);

		bool IsCharInArrayU(const unsigned char cTokChar, const unsigned char *sChars, unsigned int iChars);
		bool IsCharInArrayS(const char cTokChar, const char *sChars, unsigned int iChars);

		char *StrnDup(const char* sString, size_t iLength);

		bool NewGUID(char *sOut, int iMaxOutSz);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::String
} //namespace::NSWFL
#endif
