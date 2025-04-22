///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_CONVERSION_CPP_
#define _NSWFL_CONVERSION_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace Conversion {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Bin2HexFriendly(void* lpTarget, int iMaxSz, const void* lpSrc, int iSz, int iBytes, int iWidth)
		{
			int iWPos = 0;
			int iRPos = 0;
			int iByte = 0;
			int iCurrentWidth = 0;

			if (iBytes < 1)
			{
				return 0;
			}

			Assert(((iSz * iBytes) + 1) > iMaxSz, "Buffer too small.");

			while (iRPos < iSz)
			{
				iByte = (iBytes - 1);

				while (iByte >= 0)
				{
					((WCHAR*)lpTarget)[iWPos++] = _T("0123456789ABCDEF")[((((WCHAR*)lpSrc)[iRPos] >> (iByte--) * 4) & 0xF)];
					iCurrentWidth++;
				}

				if (iCurrentWidth >= iWidth)
				{
					((WCHAR*)lpTarget)[iWPos++] = '\r';
					((WCHAR*)lpTarget)[iWPos++] = '\n';
					iCurrentWidth = 0;
				}
				else {
					((WCHAR*)lpTarget)[iWPos++] = ' ';
				}

				iRPos++;
			}

			((WCHAR*)lpTarget)[iWPos] = '\0';

			return iWPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Bin2Hex(void* lpTarget, int iMaxSz, const void* lpSrc, int iSz, int iBytes, bool bPrefix)
		{
			int iWPos = 0;
			int iRPos = 0;
			int iByte = 0;

			if (iBytes < 1)
			{
				return 0;
			}

			Assert(((iSz * iBytes) + 1) > iMaxSz, "Buffer too small.");

			if (bPrefix)
			{
				((WCHAR*)lpTarget)[iWPos++] = '0';
				((WCHAR*)lpTarget)[iWPos++] = 'x';
			}

			while (iRPos < iSz)
			{
				iByte = (iBytes - 1);

				while (iByte >= 0)
				{
					((WCHAR*)lpTarget)[iWPos++] = _T("0123456789ABCDEF")[((((WCHAR*)lpSrc)[iRPos] >> (iByte--) * 4) & 0xF)];
				}
				iRPos++;
			}

			((WCHAR*)lpTarget)[iWPos] = '\0';

			return iWPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Bin2Hex(void* lpTarget, int iMaxSz, const void* lpSrc, int iSz, int iBytes)
		{
			return NSWFL::Conversion::Bin2Hex(lpTarget, iMaxSz, lpSrc, iSz, iBytes, true);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int HexToDecI(const WCHAR* sHexString)
		{
			struct CHexMap
			{
				WCHAR cChar;
				int iValue;
			};

			const int iHexMapSz = 16;

			CHexMap HexMap[iHexMapSz] =
			{
				{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3},
				{'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
				{'8', 8}, {'9', 9}, {'A',10}, {'B',11},
				{'C',12}, {'D',13}, {'E',14}, {'F',15}
			};

			WCHAR sAlloc[64];
			wcscpy_s(sAlloc, _countof(sAlloc), NSWFL::String::UCase((WCHAR*)sHexString));

			WCHAR* sHex = sAlloc;
			int iResult = 0;
			bool bFirstTime = true;

			if (*sHex == '0' && *(sHex + 1) == 'X')
			{
				sHex += 2;
			}

			while (*sHex != '\0')
			{
				bool bFound = false;
				for (int i = 0; i < iHexMapSz; i++)
				{
					if (*sHex == HexMap[i].cChar)
					{
						if (!bFirstTime)
						{
							iResult <<= 4;
						}
						iResult |= HexMap[i].iValue;
						bFound = true;
						break;
					}
				}

				if (!bFound)
				{
					break;
				}

				sHex++;
				bFirstTime = false;
			}

			return iResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned int HexToDecU(const WCHAR* sHexString)
		{
			struct CHexMap
			{
				WCHAR cChar;
				int iValue;
			};

			const int iHexMapSz = 16;

			CHexMap HexMap[iHexMapSz] =
			{
				{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3},
				{'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
				{'8', 8}, {'9', 9}, {'A',10}, {'B',11},
				{'C',12}, {'D',13}, {'E',14}, {'F',15}
			};

			WCHAR sAlloc[64];
			wcscpy_s(sAlloc, _countof(sAlloc), NSWFL::String::UCase((WCHAR*)sHexString));

			WCHAR* sHex = sAlloc;
			unsigned int iResult = 0;
			bool bFirstTime = true;

			if (*sHex == '0' && *(sHex + 1) == 'X')
			{
				sHex += 2;
			}

			while (*sHex != '\0')
			{
				bool bFound = false;
				for (int i = 0; i < iHexMapSz; i++)
				{
					if (*sHex == HexMap[i].cChar)
					{
						if (!bFirstTime)
						{
							iResult <<= 4;
						}
						iResult |= HexMap[i].iValue;
						bFound = true;
						break;
					}
				}

				if (!bFound)
				{
					break;
				}

				sHex++;
				bFirstTime = false;
			}

			return iResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned __int64 HexToDecU64(const WCHAR* sHexString)
		{
			struct CHexMap
			{
				WCHAR cChar;
				int iValue;
			};

			const int iHexMapSz = 16;

			CHexMap HexMap[iHexMapSz] =
			{
				{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3},
				{'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
				{'8', 8}, {'9', 9}, {'A',10}, {'B',11},
				{'C',12}, {'D',13}, {'E',14}, {'F',15}
			};

			WCHAR sAlloc[64];
			wcscpy_s(sAlloc, _countof(sAlloc), NSWFL::String::UCase((WCHAR*)sHexString));

			WCHAR* sHex = sAlloc;
			unsigned __int64 iResult = 0;
			bool bFirstTime = true;

			if (*sHex == '0' && *(sHex + 1) == 'X')
			{
				sHex += 2;
			}

			while (*sHex != '\0')
			{
				bool bFound = false;
				for (int i = 0; i < iHexMapSz; i++)
				{
					if (*sHex == HexMap[i].cChar)
					{
						if (!bFirstTime)
						{
							iResult <<= 4;
						}
						iResult |= HexMap[i].iValue;
						bFound = true;
						break;
					}
				}

				if (!bFound)
				{
					break;
				}

				sHex++;
				bFirstTime = false;
			}

			return iResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		__int64 HexToDecI64(const WCHAR* sHexString)
		{
			struct CHexMap
			{
				WCHAR cChar;
				int iValue;
			};

			const int iHexMapSz = 16;

			CHexMap HexMap[iHexMapSz] =
			{
				{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3},
				{'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
				{'8', 8}, {'9', 9}, {'A',10}, {'B',11},
				{'C',12}, {'D',13}, {'E',14}, {'F',15}
			};

			WCHAR sAlloc[64];
			wcscpy_s(sAlloc, _countof(sAlloc), NSWFL::String::UCase((WCHAR*)sHexString));

			WCHAR* sHex = sAlloc;
			__int64 iResult = 0;
			bool bFirstTime = true;

			if (*sHex == '0' && *(sHex + 1) == 'X')
			{
				sHex += 2;
			}

			while (*sHex != '\0')
			{
				bool bFound = false;
				for (int i = 0; i < iHexMapSz; i++)
				{
					if (*sHex == HexMap[i].cChar)
					{
						if (!bFirstTime)
						{
							iResult <<= 4;
						}
						iResult |= HexMap[i].iValue;
						bFound = true;
						break;
					}
				}

				if (!bFound)
				{
					break;
				}

				sHex++;
				bFirstTime = false;
			}

			return iResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* LongChar(WCHAR* sOutVal, int iMaxSz, DWORD dwInput)
		{
			_ultow_s(dwInput, sOutVal, iMaxSz, 10);
			return sOutVal;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FloatChar(WCHAR* sOutVal, int iMaxSz, float fInput, int iDecPlaces)
		{
			swprintf_s(sOutVal, iMaxSz, L"%.*g", iDecPlaces, fInput);
			return sOutVal;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FloatChar(WCHAR* sOutVal, int iMaxSz, float fInput)
		{
			FloatChar(sOutVal, iMaxSz, fInput, 12);
			return sOutVal;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* DoubleChar(WCHAR* sOutVal, int iMaxSz, double dInput)
		{
			swprintf_s(sOutVal, iMaxSz, _T("%.1f"), dInput);
			return sOutVal;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* IntChar(WCHAR* sOutVal, int iMaxSz, int iInput)
		{
			swprintf_s(sOutVal, iMaxSz, L"%d", iInput);
			return sOutVal;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		long CharLong(const WCHAR* sInput)
		{
			return (long)_wtol(sInput);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		float CharFloat(const WCHAR* sInput)
		{
			return (float)_wtof(sInput);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		double CharDouble(const WCHAR* sInput)
		{
			return (double)_wtof(sInput);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CharInt(const WCHAR* sInput)
		{
			return (int)_wtoi(sInput);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		double Round(double dValue, int iDecimalPlaces)
		{
			WCHAR sFormat[64];
			WCHAR sTemp[64];
			swprintf_s(sFormat, _countof(sFormat), _T("%%.%df"), iDecimalPlaces);
			swprintf_s(sTemp, _countof(sTemp), sFormat, dValue);
			return _wtof(sTemp);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Round(double dValue, int iDecimalPlaces, WCHAR* sOutput, int iSizeOfOutput)
		{
			WCHAR sFormat[64];
			swprintf_s(sFormat, _countof(sFormat), _T("%%.%df"), iDecimalPlaces);
			return swprintf_s(sOutput, iSizeOfOutput, sFormat, dValue);
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FormatDouble(WCHAR* sOut, int iMaxSz, double dValue, int iDecimalPlaces)
		{
			WCHAR sFormat[64];
			WCHAR sTemp[64];
			swprintf_s(sFormat, _countof(sFormat), _T("%%.%df"), iDecimalPlaces);
			swprintf_s(sTemp, _countof(sTemp), sFormat, dValue);
			if (iDecimalPlaces > 0)
			{
				return NSWFL::Conversion::FormatFloatString(sOut, iMaxSz, sTemp);
			}
			else {
				return NSWFL::Conversion::FormatiString(sOut, iMaxSz, sTemp);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FormatFloat(WCHAR* sOut, int iMaxSz, float fValue, int iDecimalPlaces)
		{
			return FormatDouble(sOut, iMaxSz, fValue, iDecimalPlaces);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FormatFloatString(WCHAR* sOut, int iMaxSz, const WCHAR* sIn)
		{
			return NSWFL::Conversion::FormatDoubleString(sOut, iMaxSz, sIn);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FormatDoubleString(WCHAR* sOut, int iMaxSz, const WCHAR* sIn)
		{
			int iRPos = (int)wcslen(sIn) - 1;
			int iWPos = 0;
			int iDPos = 0;

			bool bFoundDecimal = false;

			if (iRPos < 3)
			{
				wcscpy_s(sOut, iMaxSz, sIn);
				return sOut;
			}

			while (iRPos != -1)
			{
				if (sIn[iRPos] == '.')
				{
					bFoundDecimal = true;
					iDPos = -1;
				}

				if (iDPos++ == 3 && bFoundDecimal && sIn[iRPos] != '-')
				{
					Assert(iWPos >= iMaxSz, "Buffer too small.");
					sOut[iWPos++] = ',';
					iDPos = 1;
				}

				if (iWPos >= iMaxSz)
				{
					Assert(iWPos >= iMaxSz, "Buffer too small.");
				}
				sOut[iWPos++] = sIn[iRPos--];
			}

			NSWFL::String::ReverseString(sOut, iWPos);

			sOut[iWPos] = '\0';

			return sOut;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FormatiString(WCHAR* sOut, int iMaxSz, const WCHAR* sIn)
		{
			int iRPos = (int)wcslen(sIn) - 1;
			int iWPos = 0;
			int iDPos = 0;

			if (iRPos < 3)
			{
				wcscpy_s(sOut, iMaxSz, sIn);
				return sOut;
			}

			while (iRPos != -1)
			{
				if (iDPos++ == 3 && sIn[iRPos] != '-')
				{
					Assert(iWPos >= iMaxSz, "Buffer too small.");
					sOut[iWPos++] = ',';
					iDPos = 1;
				}

				Assert(iWPos >= iMaxSz, "Buffer too small.");
				sOut[iWPos++] = sIn[iRPos--];
			}

			NSWFL::String::ReverseString(sOut, iWPos);

			sOut[iWPos] = '\0';

			return sOut;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FormatInteger(WCHAR* sOut, int iMaxSz, const long lIn)
		{
			WCHAR sLong[64];
			_ultow_s(lIn, sLong, _countof(sLong), 10);
			return FormatiString(sOut, iMaxSz, sLong);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* I64FormatInteger(WCHAR* sOut, int iMaxSz, __int64 i64)
		{
			WCHAR sLong[128];
			_ui64tow_s(i64, sLong, _countof(sLong), 10);
			return FormatiString(sOut, iMaxSz, sLong);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		float fFormattedValue(const WCHAR* sFormatted)
		{
			return (float)NSWFL::Conversion::dFormattedValue(sFormatted);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		double dFormattedValue(const WCHAR* sFormatted)
		{
			int iLength = (int)wcslen(sFormatted);
			int iRPos = 0;
			int iWPos = 0;

			WCHAR sVal[32];

			while (iRPos < iLength)
			{
				if (sFormatted[iRPos] == ',')
					iRPos++;

				if ((sFormatted[iRPos] < 48 || sFormatted[iRPos] > 57) && sFormatted[iRPos] != '.')
					break;

				sVal[iWPos] = sFormatted[iRPos];
				iRPos++;
				iWPos++;
			}
			sVal[iWPos] = '\0';

			return _wtof(sVal);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		__int64 i64FormattedValue(const WCHAR* sFormatted)
		{
			int iLength = (int)wcslen(sFormatted);
			int iRPos = 0;
			int iWPos = 0;

			WCHAR sVal[32];

			if (iLength <= 3)
			{
				return _wtol(sFormatted);
			}

			while (iRPos < iLength)
			{
				if (sFormatted[iRPos] == ',')
					iRPos++;

				if (sFormatted[iRPos] < 48 || sFormatted[iRPos] > 57)
					break;

				sVal[iWPos] = sFormatted[iRPos];
				iRPos++;
				iWPos++;
			}
			sVal[iWPos] = '\0';

			return _wtoi64(sVal);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int iFormattedValue(const WCHAR* sFormatted)
		{
			int iLength = (int)wcslen(sFormatted);
			int iRPos = 0;
			int iWPos = 0;
			bool bIsNegative = false;

			WCHAR sVal[32];

			if (iLength <= 3)
			{
				return _wtol(sFormatted);
			}

			if (sFormatted[0] == '-')
			{
				bIsNegative = true;
				iRPos++;
			}

			while (iRPos < iLength)
			{
				if (sFormatted[iRPos] == ',')
					iRPos++;

				if (sFormatted[iRPos] < 48 || sFormatted[iRPos] > 57)
					break;

				sVal[iWPos] = sFormatted[iRPos];
				iRPos++;
				iWPos++;
			}
			sVal[iWPos] = '\0';

			if (bIsNegative)
			{
				return _wtol(sVal) * -1;
			}
			else {
				return _wtol(sVal);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned int uiFormattedValue(const WCHAR* sFormatted)
		{
			return (unsigned int)NSWFL::Conversion::iFormattedValue(sFormatted);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool ParseBoolean(const int iValue)
		{
			return(iValue != 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool ParseBoolean(const WCHAR* sText)
		{
			if (NSWFL::String::StrEqlI(sText, _T("yes")) || NSWFL::String::StrEqlI(sText, _T("true")) || NSWFL::String::StrEqlI(sText, _T("on")))
			{
				return true;
			}

			return(NSWFL::Conversion::ParseBoolean(_wtol(sText)));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Conversion
} //namespace::NSWFL
#endif
