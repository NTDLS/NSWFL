///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_STRINGBUILDER_H_
#define _NSWFL_STRINGBUILDER_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace String {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define StringBuilder_GRANDULARITY 1024

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class StringBuilder {

		public:
			operator const WCHAR* (void);
			operator WCHAR* (void);
			operator int(void);
			StringBuilder operator+ (StringBuilder& pStr);
			StringBuilder operator+= (StringBuilder& pStr);
			bool operator== (StringBuilder& pStr);
			bool operator> (StringBuilder& pStr);
			bool operator< (StringBuilder& pStr);
			bool operator>= (StringBuilder& pStr);
			bool operator<= (StringBuilder& pStr);
			bool operator!= (StringBuilder& pStr);
			WCHAR& operator[] (unsigned int i);
			WCHAR& operator[] (int i);

			bool Initialize(const int iInAllocAhead);
			bool Initialize(void);
			void Destroy(void);

			StringBuilder(StringBuilder* pSource);
			StringBuilder(int InitialSize);
			StringBuilder(const WCHAR* String, int Size);
			StringBuilder(const WCHAR* String);
			StringBuilder(const WCHAR* String, int Size, int InitialSize);
			StringBuilder(void);
			~StringBuilder(void);

			bool WriteToFileByHandle(FILE* hTarget);
			bool WriteToFile(const WCHAR* sFileName);

			bool AppendFromTextBox(HWND hTextBox);
			bool AppendFromFile(const WCHAR* sFileName);
			bool AppendFromFileByHandle(FILE* hSource);

			bool Prepend(const WCHAR* sDat2Append, const int iDat2AppendSz);
			bool Prepend(const WCHAR* sDat2Append);
			bool Prepend(StringBuilder* pBuffer);
			int PrependF(const WCHAR* sFormat, ...);

			bool Append(StringBuilder* pBuffer);

			int StrCmp(StringBuilder* pStr);
			int StrCmpi(StringBuilder* pStr);
			int StrnCmp(StringBuilder* pStr, int iChars);
			int StrnCmpi(StringBuilder* pStr, int iChars);

			bool Append(const int iNum);
			bool Append(const unsigned int iNum);
			bool Append(const long iNum);
			bool Append(const unsigned long iNum);

			bool Append(const __int64 i64Dat2Append);
			bool Append(const unsigned __int64 i64Dat2Append);

			int AppendF(const WCHAR* sFormat, ...);
			bool Append(const WCHAR* sDat2Append);
			bool Append(const WCHAR* sDat2Append, const int iDat2AppendSz);

			bool Append(const WCHAR cCharacter2Append);

			bool Append(const float fDat2Append, int iDecimalPlaces);
			bool Append(const float fDat2Append);
			bool Append(const double dDat2Append, int iDecimalPlaces);
			bool Append(const double dDat2Append);

			int Set(const WCHAR* sString, int iLength);
			int Set(const WCHAR* sString);
			int Set(StringBuilder* pBuilder);
			int SetF(const WCHAR* sFormat, ...);

			WCHAR* ToString(void);

			int InStr(const WCHAR* sSearchFor, int sSearchForSz, const int iStartPos);
			int InStr(const WCHAR* sSearchFor, const int iStartPos);
			int InStr(const WCHAR* sSearchFor);
			int InStrI(const WCHAR* sSearchFor, int sSearchForSz, const int iStartPos);
			int InStrI(const WCHAR* sSearchFor, const int iStartPos);
			int InStrI(const WCHAR* sSearchFor);

			bool IncreaseSizeBy(int iAdditionalBytes);
			bool Resize(int iAtLeast);

			void Clear(void);
			bool Terminate(void);
			bool Terminate(int iPosition);

			int RemoveWhiteSpaces(void);
			int Trim(void);

			int Replace(const WCHAR* sReplace, int iReplaceLen, const WCHAR* sWith, int iWithLen, bool bCaseSensitive);
			int Replace(const WCHAR* sReplace, const WCHAR* sWith, bool bCaseSensitive);
			int Replace(const WCHAR* sReplace, const WCHAR* sWith);
			int ReplaceRange(int iBegin, int iEnd, const WCHAR* sWith, int iWithSz);

			int FirstCharPos(void);
			int LastCharPos(void);

			int Right(WCHAR* sOutBuf, int iMaxSz, int iRight);
			int Left(WCHAR* sOutBuf, int iMaxSz, int iLeft);

			WCHAR* Buffer;

			int Alloc;
			int AllocAhead;
			int Length;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::String
} //namespace::NSWFL
#endif
