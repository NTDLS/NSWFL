///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
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
			operator const char*(void);
			operator char*(void);
			operator int(void);
			StringBuilder operator+ (StringBuilder &pStr);
			StringBuilder operator+= (StringBuilder &pStr);
			bool operator== (StringBuilder &pStr);
			bool operator> (StringBuilder &pStr);
			bool operator< (StringBuilder &pStr);
			bool operator>= (StringBuilder &pStr);
			bool operator<= (StringBuilder &pStr);
			bool operator!= (StringBuilder &pStr);
			char &operator[] (unsigned int i);
			char &operator[] (int i);

			bool Initialize(const int iInAllocAhead);
			bool Initialize(void);
			void Destroy(void);

			StringBuilder(StringBuilder *pSource);
			StringBuilder(int InitialSize);
			StringBuilder(const char *String, int Size);
			StringBuilder(const char *String);
			StringBuilder(const char *String, int Size, int InitialSize);
			StringBuilder(void);
			~StringBuilder(void);

			bool WriteToFileByHandle(FILE *hTarget);
			bool WriteToFile(const char *sFileName);

			bool AppendFromTextBox(HWND hTextBox);
			bool AppendFromFile(const char *sFileName);
			bool AppendFromFileByHandle(FILE *hSource);

			bool Prepend(const char *sDat2Append, const int iDat2AppendSz);
			bool Prepend(const char *sDat2Append);
			bool Prepend(StringBuilder *pBuffer);
			int PrependF(const char *sFormat, ...);

			bool Append(StringBuilder *pBuffer);

			int StrCmp(StringBuilder *pStr);
			int StrCmpi(StringBuilder *pStr);
			int StrnCmp(StringBuilder *pStr, int iChars);
			int StrnCmpi(StringBuilder *pStr, int iChars);

			bool Append(const int iNum);
			bool Append(const unsigned int iNum);
			bool Append(const long iNum);
			bool Append(const unsigned long iNum);

			bool Append(const __int64 i64Dat2Append);
			bool Append(const unsigned __int64 i64Dat2Append);

			int AppendF(const char *sFormat, ...);
			bool Append(const char *sDat2Append);
			bool Append(const unsigned char *sDat2Append);
			bool Append(const char *sDat2Append, const int iDat2AppendSz);
			bool Append(const unsigned char *sDat2Append, const int iDat2AppendSz);

			bool Append(const char cCharacter2Append);

			bool Append(const float fDat2Append, int iDecimalPlaces);
			bool Append(const float fDat2Append);
			bool Append(const double dDat2Append, int iDecimalPlaces);
			bool Append(const double dDat2Append);

			int Set(const char *sString, int iLength);
			int Set(const char *sString);
			int Set(StringBuilder *pBuilder);
			int SetF(const char *sFormat, ...);

			char *ToString(void);

			int InStr(const char *sSearchFor, int sSearchForSz, const int iStartPos);
			int InStr(const char *sSearchFor, const int iStartPos);
			int InStr(const char *sSearchFor);
			int InStrI(const char *sSearchFor, int sSearchForSz, const int iStartPos);
			int InStrI(const char *sSearchFor, const int iStartPos);
			int InStrI(const char *sSearchFor);

			bool IncreaseSizeBy(int iAdditionalBytes);
			bool Resize(int iAtLeast);

			void Clear(void);
			bool Terminate(void);
			bool Terminate(int iPosition);

			int RemoveWhiteSpaces(void);
			int Trim(void);

			int Replace(const char *sReplace, int iReplaceLen, const char *sWith, int iWithLen, bool bCaseSensitive);
			int Replace(const char *sReplace, const char *sWith, bool bCaseSensitive);
			int Replace(const char *sReplace, const char *sWith);
			int ReplaceRange(int iBegin, int iEnd, const char *sWith, int iWithSz);

			int FirstCharPos(void);
			int LastCharPos(void);

			int Right(char *sOutBuf, int iMaxSz, int iRight);
			int Left(char *sOutBuf, int iMaxSz, int iLeft);

			char *Buffer;

			int Alloc;
			int AllocAhead;
			int Length;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::String
} //namespace::NSWFL
#endif
