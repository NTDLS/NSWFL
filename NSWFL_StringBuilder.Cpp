///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_STRINGBUILDER_CPP_
#define _NSWFL_STRINGBUILDER_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

namespace NSWFL {
	namespace String {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		StringBuilder StringBuilder::operator+= (StringBuilder& pStr)
		{
			this->Append(&pStr);
			return this;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		StringBuilder StringBuilder::operator+ (StringBuilder& pStr)
		{
			StringBuilder newString(this);
			newString.Append(&pStr);
			return StringBuilder(&newString);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool StringBuilder::operator== (StringBuilder& pStr)
		{
			return(this->StrCmp(&pStr) == 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool StringBuilder::operator> (StringBuilder& pStr)
		{
			return(this->StrCmp(&pStr) > 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool StringBuilder::operator< (StringBuilder& pStr)
		{
			return(this->StrCmp(&pStr) < 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool StringBuilder::operator>= (StringBuilder& pStr)
		{
			return(this->StrCmp(&pStr) >= 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool StringBuilder::operator<= (StringBuilder& pStr)
		{
			return(this->StrCmp(&pStr) <= 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool StringBuilder::operator!= (StringBuilder& pStr)
		{
			return(this->StrCmp(&pStr) != 0);
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char& StringBuilder::operator[] (unsigned int i)
		{
			return this->Buffer[i];
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char& StringBuilder::operator[] (int i)
		{
			return this->Buffer[i];
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Operator: char*
		*/
		StringBuilder::operator const char* (void)
		{
			return this->Buffer;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Operator: char*
		*/
		StringBuilder::operator char* (void)
		{
			return this->Buffer;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Operator: int
		*/
		StringBuilder::operator int(void)
		{
			return this->Length;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Default constructor.
		*/
		StringBuilder::StringBuilder(void)
		{
			this->Initialize(StringBuilder_GRANDULARITY);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Constructor, setting the initial size of the buffer.
		*/
		StringBuilder::StringBuilder(int InitialSize)
		{
			this->Initialize(InitialSize);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Constructor, setting the initial value (and therfor the size) of the buffer.
		*/
		StringBuilder::StringBuilder(const char* String)
		{
			int iLength = (int)strlen(String) + 10;
			this->Initialize(iLength);
			this->Append(String, iLength - 10);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Constructor, setting the initial value of the buffer as well as the initial size.

			[InitialSize] should (but does not have to) be larger than [Size].
		*/
		StringBuilder::StringBuilder(const char* String, int Size, int InitialSize)
		{
			this->Initialize(InitialSize);
			this->Append(String, Size);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Constructor, setting the initial value of the buffer.
		*/
		StringBuilder::StringBuilder(const char* String, int Size)
		{
			this->Initialize(Size);
			this->Append(String, Size);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Constructor, clone another instance of the StringBuilder class.
		*/
		StringBuilder::StringBuilder(StringBuilder* pSource)
		{
			this->Initialize(pSource->Alloc);
			this->Append(pSource);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Destructor.
		*/
		StringBuilder::~StringBuilder()
		{
			this->Destroy();
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Default Initializer.

			DO NOT call if a constructor was called unless you previously Destroy()d.
		*/
		bool StringBuilder::Initialize(void)
		{
			return this->Initialize(StringBuilder_GRANDULARITY);
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Initializer, sets the allocate ahead size.

			DO NOT call if a constructor was called unless you previously Destroy()d.
		*/
		bool StringBuilder::Initialize(const int iInAllocAhead)
		{
			this->AllocAhead = iInAllocAhead;

#ifdef _USE_GLOBAL_MEMPOOL
			if ((this->Buffer = (char*)pMem->Allocate(sizeof(char), this->AllocAhead + 1)) == NULL)
			{
				return false;
			}
#else
			if ((this->Buffer = (char*)calloc(sizeof(char), this->AllocAhead + 1)) == NULL)
			{
				return false;
			}
#endif

			this->Alloc = this->AllocAhead;
			this->Length = 0;

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Destructor/uninitializer.

			Used to destroy the string buffer before the class falls out of scope.
		*/
		void StringBuilder::Destroy(void)
		{
			this->AllocAhead = 0;
			this->Alloc = 0;
			this->Length = 0;

			if (this->Buffer)
			{
#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(this->Buffer);
#else
				free(this->Buffer);
#endif
			}

			this->Buffer = NULL;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the zero-based index of the first non-whitespace character in the string buffer.
		*/
		int StringBuilder::FirstCharPos(void)
		{
			return GetFirstCharPos(this->Buffer, this->Length);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the zero-based index of the last non-whitespace character in the string buffer.
		*/
		int StringBuilder::LastCharPos(void)
		{
			return GetLastCharPos(this->Buffer, this->Length);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::Set(const char* sString, int iLength)
		{
			this->Clear();
			this->Append(sString, iLength);
			this->Terminate();

			return this->Length;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::Set(const char* sString)
		{
			this->Clear();
			this->Append(sString);
			this->Terminate();

			return this->Length;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::Set(StringBuilder* pBuilder)
		{
			this->Clear();
			this->Append(pBuilder);
			this->Terminate();

			return this->Length;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Sets to the string buffer using standard formatting. Exactly like sprintf("", ...)
		*/
		int StringBuilder::SetF(const char* sFormat, ...)
		{
			va_list ArgList;
			va_start(ArgList, sFormat);

			int iMemoryRequired = _vscprintf(sFormat, ArgList);

#ifdef _USE_GLOBAL_MEMPOOL
			char* sBuf = (char*)pMem->Allocate(sizeof(char), iMemoryRequired + 1);
#else
			char* sBuf = (char*)calloc(sizeof(char), iMemoryRequired + 1);
#endif

			int iSz = _vsprintf_s_l(sBuf, iMemoryRequired + 1, sFormat, NULL, ArgList);
			va_end(ArgList);

			this->Clear();
			this->Append(sBuf, iSz);
			this->Terminate();

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sBuf);
#else
			free(sBuf);
#endif

			return this->Length;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends to the string buffer using standard formatting. Exactly like sprintf("", ...)
		*/
		int StringBuilder::AppendF(const char* sFormat, ...)
		{
			va_list ArgList;
			va_start(ArgList, sFormat);

			int iMemoryRequired = _vscprintf(sFormat, ArgList);

#ifdef _USE_GLOBAL_MEMPOOL
			char* sBuf = (char*)pMem->Allocate(sizeof(char), iMemoryRequired + 1);
#else
			char* sBuf = (char*)calloc(sizeof(char), iMemoryRequired + 1);
#endif

			int iSz = _vsprintf_s_l(sBuf, iMemoryRequired + 1, sFormat, NULL, ArgList);
			va_end(ArgList);

			this->Append(sBuf, iSz);
			this->Terminate();

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sBuf);
#else
			free(sBuf);
#endif

			return this->Length;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Replaces a range of bytes with the specified bytes.
		*/
		int StringBuilder::ReplaceRange(int iBegin, int iEnd, const char* sWith, int iWithSz)
		{
			int iNewSz = this->Length + (iWithSz - (iEnd - iBegin));
			int iGapSize = (this->Length - iNewSz);
			int iPos = 0;

			if (iNewSz >= this->Alloc)
			{
				this->IncreaseSizeBy(iNewSz + 1);
			}

			if (iGapSize < 0)
			{
				iGapSize = abs(iGapSize);

				for (iPos = iNewSz; iPos > iEnd && (iPos - iGapSize) >= 0; iPos--)
				{
					this->Buffer[iPos] = this->Buffer[iPos - iGapSize];
				}
				memcpy(this->Buffer + iBegin, sWith, iWithSz);
			}
			else if (iGapSize > 0)
			{
				memcpy(this->Buffer + iBegin, sWith, iWithSz);

				for (iPos = iBegin + iWithSz; iPos < iNewSz; iPos++)
				{
					this->Buffer[iPos] = this->Buffer[iPos + iGapSize];
				}

				this->Buffer[iPos] = '\0';
			}
			else {
				memcpy(this->Buffer + iBegin, sWith, iWithSz);
			}

			this->Length = iNewSz;

			return this->Length;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends data to the data buffer from a windows textbox.
		*/
		bool StringBuilder::AppendFromTextBox(HWND hTextBox)
		{
			int iTextLen = (GetWindowTextLength(hTextBox));

#ifdef _USE_GLOBAL_MEMPOOL
			char* sBuf = (char*)pMem->Allocate(iTextLen + 1, sizeof(char));
#else
			char* sBuf = (char*)calloc(iTextLen + 1, sizeof(char));
#endif

			if (sBuf != NULL)
			{
				iTextLen = GetWindowText(hTextBox, sBuf, iTextLen + 1);
				this->Append(sBuf, iTextLen);

#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(sBuf);
#else
				free(sBuf);
#endif

				return true;
			}

			return false;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Removes all white-spaces from the buffer.
		*/
		int StringBuilder::RemoveWhiteSpaces(void)
		{
			int iWPos = 0;

			for (int iRPos = 0; iRPos < this->Length; iRPos++)
			{
				if (!IsWhiteSpace(this->Buffer[iRPos]))
				{
					this->Buffer[iWPos++] = this->Buffer[iRPos];
				}
			}

			this->Buffer[iWPos] = '\0';

			this->Length = iWPos;

			return iWPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Removes white-spaces from the beginning and end of the buffer.
		*/
		int StringBuilder::Trim(void)
		{
			return(this->Length = NSWFL::String::CleanWhiteSpaces(this->Buffer, this->Length));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the "Right Most" (x) number bytes form the buffer.
		*/
		int StringBuilder::Right(char* sOutBuf, int iMaxSz, int iRight)
		{
			return NSWFL::String::Right(sOutBuf, iMaxSz, this->Buffer, this->Length, iRight);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the "Left Most" (x) nuber bytes form the buffer.
		*/
		int StringBuilder::Left(char* sOutBuf, int iMaxSz, int iLeft)
		{
			return NSWFL::String::Left(sOutBuf, iMaxSz, this->Buffer, this->Length, iLeft);
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Replaces a given piece of text with another. (not case-sensitive)
		*/
		int StringBuilder::Replace(const char* sReplace, const char* sWith)
		{
			return Replace(sReplace, sWith, false);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Replaces a given piece of text with another allowing for optional case-sensitivity)
		*/
		int StringBuilder::Replace(const char* sReplace, const char* sWith, bool bCaseSensitive)
		{
			return Replace(sReplace, (int)strlen(sReplace), sWith, (int)strlen(sWith), bCaseSensitive);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Replaces a given piece of text with another allowing for optional case-sensitivity)
		*/
		int StringBuilder::Replace(const char* sReplace, int iReplaceLen, const char* sWith, int iWithLen, bool bCaseSensitive)
		{
			int iOccurrences = 0;
			int iPos = 0;
			int iMemDiff = (iWithLen - iReplaceLen);

			while (true)
			{
				if (bCaseSensitive)
				{
					iPos = this->InStr(sReplace, iPos);
				}
				else {
					iPos = InStrI(sReplace, iPos);
				}

				if (iPos >= 0)
				{
					if (iMemDiff > 0)
					{
						//Do we need to allocate additional RAM?
						if ((this->Length + iMemDiff) >= this->Alloc)
						{
							if (!this->IncreaseSizeBy(iMemDiff))
							{
								//Failed to allocate needed additional RAM.
								return false;
							}
						}

						int iCopyFrom = iPos + iMemDiff + iReplaceLen;
						for (int iWPos = (this->Length + iMemDiff) - 1; iWPos >= iCopyFrom; iWPos--)
						{
							this->Buffer[iWPos] = this->Buffer[iWPos - iMemDiff];
						}
					}
					else if (iMemDiff < 0) {
						int iCopyFrom = (this->Length - iReplaceLen) + 1;
						for (int iWPos = iPos; iWPos < iCopyFrom; iWPos++)
						{
							this->Buffer[iWPos + iWithLen] = this->Buffer[iWPos + iReplaceLen];
						}
					}

					memcpy(this->Buffer + iPos, sWith, iWithLen);

					this->Length += iMemDiff;

					iPos++;
					iOccurrences++;
				}
				else {
					break;
				}
			}

			if (iOccurrences > 0)
			{
				this->Terminate();
			}

			return iOccurrences;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Resizes the size of the data buffer.

			Note: Despite the name of the function, the additional bytes can be larger or smaller.
		*/
		bool StringBuilder::IncreaseSizeBy(int iAdditionalBytes)
		{
			this->Alloc += (iAdditionalBytes + 1);

#ifdef _USE_GLOBAL_MEMPOOL
			//Allocate more RAM.
			if ((this->Buffer = (char*)pMem->ReAllocate(this->Buffer, sizeof(char), this->Alloc)) != NULL)
			{
				return true;
			}
#else
			//Allocate more RAM.
			if ((this->Buffer = (char*)realloc(this->Buffer, this->Alloc)) != NULL)
			{
				return true;
			}
#endif

			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Sets the size of the data buffer.

			Note: Reducing the size of the buffer will truncate any data past the point of truncation.
		*/
		bool StringBuilder::Resize(int iSize)
		{
			if (iSize != this->Alloc)
			{
				this->Alloc = iSize;

				if (this->Alloc < this->Length)
				{
					this->Length = this->Alloc; //Truncate if necessary.
				}

#ifdef _USE_GLOBAL_MEMPOOL
				//Allocate more RAM.
				if ((this->Buffer = (char*)pMem->ReAllocate(this->Buffer, sizeof(char), this->Alloc)) != NULL)
				{
					return true;
				}
#else
				//Allocate more RAM.
				if ((this->Buffer = (char*)realloc(this->Buffer, this->Alloc)) != NULL)
				{
					return true;
				}
#endif

				return false;
			}

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Truncates the buffer, resetting it back to zero length.

			Note: This does not **necessarily** destroy all of the data.
		*/
		void StringBuilder::Clear(void)
		{
			if (!this->Buffer)
			{
				this->IncreaseSizeBy(10);
			}

			this->Length = 0;
			this->Buffer[0] = '\0';
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			NULL terminates the data buffer at a specified location.
		*/
		bool StringBuilder::Terminate(int iPosition)
		{
			if (iPosition < this->Alloc)
			{
				this->Buffer[iPosition] = '\0';
				this->Length = iPosition;
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			NULL terminates the data buffer.
		*/
		bool StringBuilder::Terminate(void)
		{
			bool bResult = this->Append("\0", 1);
			this->Length--; //We don't count the NULL terminator.
			return bResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends (char *) bytes of data of a specified length to the **beginning** of the string.
		*/
		bool StringBuilder::Prepend(const char* sDat2Append, const int iDat2AppendSz)
		{
			StringBuilder temp(this);
			this->Set(sDat2Append, iDat2AppendSz);
			return this->Append(&temp);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends (char *) bytes of data to the **beginning** of the string.
		*/
		bool StringBuilder::Prepend(const char* sDat2Append)
		{
			StringBuilder temp(this);
			this->Set(sDat2Append, (int)strlen(sDat2Append));
			return this->Append(&temp);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends bytes of data to the **beginning** of the string.
		*/
		bool StringBuilder::Prepend(StringBuilder* pBuffer)
		{
			StringBuilder temp(this);
			this->Set(pBuffer->Buffer, pBuffer->Length);
			return this->Append(&temp);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends (char *) bytes of data to the **beginning** of the string.
		*/
		int StringBuilder::PrependF(const char* sFormat, ...)
		{
			va_list ArgList;
			va_start(ArgList, sFormat);

			int iMemoryRequired = _vscprintf(sFormat, ArgList);

#ifdef _USE_GLOBAL_MEMPOOL
			char* sBuf = (char*)pMem->Allocate(sizeof(char), iMemoryRequired + 1);
#else
			char* sBuf = (char*)calloc(sizeof(char), iMemoryRequired + 1);
#endif

			int iSz = _vsprintf_s_l(sBuf, iMemoryRequired + 1, sFormat, NULL, ArgList);
			va_end(ArgList);

			this->Prepend(sBuf, iSz);

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sBuf);
#else
			free(sBuf);
#endif

			return this->Length;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends (char *) bytes of data of a specified length.
		*/
		bool StringBuilder::Append(const char* sDat2Append, const int iDat2AppendSz)
		{
			if (sDat2Append && iDat2AppendSz > 0)
			{
				int iRPos = 0;

				//Do we have enough RAM allocated for the newly read data?
				if ((this->Length + iDat2AppendSz) + 1 > this->Alloc)
				{
					//Figure out how much more RAM we will need (And add a few more to it).
					if (!this->IncreaseSizeBy(iDat2AppendSz + this->AllocAhead + 1))
					{
						return false;
					}
				}

				while (iRPos < iDat2AppendSz)
				{
					this->Buffer[this->Length++] = sDat2Append[iRPos++];
				}

				this->Buffer[this->Length] = '\0';
			}

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends (unsigned char *) bytes of data of a specified length.
		*/
		bool StringBuilder::Append(const unsigned char* sDat2Append, const int iDat2AppendSz)
		{
			return this->Append((const char*)sDat2Append, iDat2AppendSz);
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends (unsigned char *) text. (must be NULL terminated)
		*/
		bool StringBuilder::Append(const unsigned char* sDat2Append)
		{
			if (sDat2Append)
			{
				return this->Append(sDat2Append, (int)strlen((const char*)sDat2Append));
			}
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends (char *) text. (must be NULL terminated)
		*/
		bool StringBuilder::Append(const char* sDat2Append)
		{
			if (sDat2Append)
			{
				return this->Append(sDat2Append, (int)strlen(sDat2Append));
			}
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends a single (char) byte.
		*/
		bool StringBuilder::Append(const char cCharacter2Append)
		{
			const char sText[1] = { cCharacter2Append };
			return this->Append(sText, 1);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends an unsigned C long to the data buffer.
		*/
		bool StringBuilder::Append(const unsigned long iNum)
		{
			char sNum[32];
			sprintf_s(sNum, sizeof(sNum), "%d", iNum);
			return this->Append(sNum);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends a C long to the data buffer.
		*/
		bool StringBuilder::Append(const long iNum)
		{
			char sNum[32];
			sprintf_s(sNum, sizeof(sNum), "%d", iNum);
			return Append(sNum);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends an unsigned C long to the data buffer.
		*/
		bool StringBuilder::Append(const unsigned int iNum)
		{
			char sNum[32];
			sprintf_s(sNum, sizeof(sNum), "%u", iNum);
			return this->Append(sNum);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends a C long to the data buffer.
		*/
		bool StringBuilder::Append(const int iNum)
		{
			char sNum[32];
			sprintf_s(sNum, sizeof(sNum), "%d", iNum);
			return this->Append(sNum);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends data to the data buffer from another instance of the class.
		*/
		bool StringBuilder::Append(StringBuilder* pBuffer)
		{
			return this->Append(pBuffer->Buffer, pBuffer->Length);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Writes the string to a file (by handle).
		*/
		bool StringBuilder::WriteToFileByHandle(FILE* hTarget)
		{
			int iBytesWritten = 0;
			int iOffset = 0;
			int iBufferSize = 0;

			do {
				if (iOffset + StringBuilder_GRANDULARITY > this->Length)
				{
					iBufferSize = this->Length - iOffset;
				}
				else {
					iBufferSize = StringBuilder_GRANDULARITY;
				}

				iBytesWritten = (int)fwrite(this->Buffer + iOffset, sizeof(char), iBufferSize, hTarget);
				iOffset += iBytesWritten;
			} while (iBytesWritten == StringBuilder_GRANDULARITY);

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Writes the string to a file (by name).
		*/
		bool StringBuilder::WriteToFile(const char* sFileName)
		{
			FILE* hTarget = NULL;

			if (fopen_s(&hTarget, sFileName, "wb") != 0)
			{
				return false;
			}

			bool bResult = this->WriteToFileByHandle(hTarget);

			fclose(hTarget);

			return bResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends data to the data buffer from a file (by handle).
		*/
		bool StringBuilder::AppendFromFileByHandle(FILE* hSource)
		{
			int iBytesRead = 0;

			char sBuffer[StringBuilder_GRANDULARITY + 1];

			do {
				iBytesRead = (int)fread(sBuffer, sizeof(char), StringBuilder_GRANDULARITY, hSource);
				this->Append(sBuffer, iBytesRead);
			} while (iBytesRead == StringBuilder_GRANDULARITY);

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends data to the data buffer from a file (by name).
		*/
		bool StringBuilder::AppendFromFile(const char* sFileName)
		{
			FILE* hSource = NULL;

			if (fopen_s(&hSource, sFileName, "rb") != 0)
			{
				return false;
			}

			bool bResult = this->AppendFromFileByHandle(hSource);

			fclose(hSource);

			return bResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends a 64 bit integer to the data buffer.
		*/
		bool StringBuilder::Append(const __int64 i64Dat2Append)
		{
			char si64[128];
			sprintf_s(si64, sizeof(si64), "%I64d", i64Dat2Append);
			return this->Append(si64, (int)strlen(si64));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends an unsigned 64 bit integer to the data buffer.
		*/
		bool StringBuilder::Append(const unsigned __int64 i64Dat2Append)
		{
			char si64[128];
			sprintf_s(si64, sizeof(si64), "%I64d", i64Dat2Append);
			return this->Append(si64, (int)strlen(si64));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends a floating point number to the data buffer (2 decimal places).
		*/
		bool StringBuilder::Append(const float fDat2Append)
		{
			return this->Append(fDat2Append, 2);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends a floating point number to the data buffer using with specified number of decimal places.
		*/
		bool StringBuilder::Append(const float fDat2Append, int iDecimalPlaces)
		{
			return this->Append((double)fDat2Append, iDecimalPlaces);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends a (double) floating point number to the data buffer (2 decimal places).
		*/
		bool StringBuilder::Append(const double dDat2Append)
		{
			return Append(dDat2Append, 2);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Appends a (double) floating point number to the data buffer using with specified number of decimal places.
		*/
		bool StringBuilder::Append(const double dDat2Append, int iDecimalPlaces)
		{
			char sFloat[64];
			char sFormat[10];

			sprintf_s(sFormat, sizeof(sFormat), "%%.%df", iDecimalPlaces);
			sprintf_s(sFloat, sizeof(sFloat), sFormat, dDat2Append);

			int iLength = (int)strlen(sFloat);

			while (sFloat[iLength - 1] == '0' && sFloat[iLength - 2] != '.' && iLength > 3)
			{
				iLength--;
			}

			sFloat[iLength] = '\0';

			return this->Append(sFloat, iLength);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char* StringBuilder::ToString(void)
		{
			if (this->Terminate())
			{
				return this->Buffer;
			}
			return NULL;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::InStr(const char* sSearchFor, int sSearchForSz, const int iStartPos)
		{
			return NSWFL::String::InStr(sSearchFor, sSearchForSz, this->Buffer, this->Length, iStartPos);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::InStr(const char* sSearchFor, const int iStartPos)
		{
			return NSWFL::String::InStr(sSearchFor, this->Buffer, this->Length, iStartPos);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::InStr(const char* sSearchFor)
		{
			return NSWFL::String::InStr(sSearchFor, this->Buffer, this->Length, 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::InStrI(const char* sSearchFor, int sSearchForSz, const int iStartPos)
		{
			return NSWFL::String::InStrI(sSearchFor, sSearchForSz, this->Buffer, this->Length, iStartPos);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::InStrI(const char* sSearchFor, const int iStartPos)
		{
			return NSWFL::String::InStrI(sSearchFor, this->Buffer, this->Length, iStartPos);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::InStrI(const char* sSearchFor)
		{
			return NSWFL::String::InStrI(sSearchFor, this->Buffer, this->Length, 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::StrCmp(StringBuilder* pStr)
		{
			return strcmp(this->Buffer, pStr->Buffer);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::StrCmpi(StringBuilder* pStr)
		{
			return _strcmpi(this->Buffer, pStr->Buffer);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::StrnCmp(StringBuilder* pStr, int iChars)
		{
			return strncmp(this->Buffer, pStr->Buffer, iChars);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int StringBuilder::StrnCmpi(StringBuilder* pStr, int iChars)
		{
			return _strnicmp(this->Buffer, pStr->Buffer, iChars);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::String
} //namespace::NSWFL
#endif
