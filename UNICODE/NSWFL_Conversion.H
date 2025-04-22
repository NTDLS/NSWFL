///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_CONVERSION_H_
#define _NSWFL_CONVERSION_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Conversion {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FloatChar(WCHAR* sOutVal, int iMaxSz, float fInput);
		WCHAR* DoubleChar(WCHAR* sOutVal, int iMaxSz, double dInput);
		WCHAR* IntChar(WCHAR* sOutVal, int iMaxSz, int iInput);
		WCHAR* LongChar(WCHAR* sOutVal, int iMaxSz, DWORD dwInput);
		WCHAR* FloatChar(WCHAR* sOutVal, int iMaxSz, float fInput, int iDecPlaces);
		long CharLong(const WCHAR* sInput);
		float CharFloat(const WCHAR* sInput);
		double CharDouble(const WCHAR* sInput);
		int CharInt(const WCHAR* sInput);
		int HexToDecI(const WCHAR* sHexString);
		unsigned int HexToDecU(const WCHAR* sHexString);
		__int64 HexToDecI64(const WCHAR* sHexString);
		unsigned __int64 HexToDecU64(const WCHAR* sHexString);
		int Bin2Hex(void* lpTarget, int iMaxSz, const void* lpSrc, int iSz, int iBytes);
		int Bin2Hex(void* lpTarget, int iMaxSz, const void* lpSrc, int iSz, int iBytes, bool bPrefix);
		int Bin2HexFriendly(void* lpTarget, int iMaxSz, const void* lpSrc, int iSz, int iBytes, int iWidth);

		WCHAR* FormatFloatString(WCHAR* sOut, int iMaxSz, const WCHAR* sIn);
		WCHAR* FormatFloat(WCHAR* sOut, int iMaxSz, float fValue, int iDecimalPlaces);
		WCHAR* FormatDouble(WCHAR* sOut, int iMaxSz, double dValue, int iDecimalPlaces);
		WCHAR* FormatDoubleString(WCHAR* sOut, int iMaxSz, const WCHAR* sIn);
		WCHAR* FormatFloatString(WCHAR* sOut, int iMaxSz, const WCHAR* sIn);
		WCHAR* FormatiString(WCHAR* sOut, int iMaxSz, const WCHAR* sIn);
		WCHAR* FormatInteger(WCHAR* sOut, int iMaxSz, const long lIn);
		int iFormattedValue(const WCHAR* sFormatted);
		unsigned int uiFormattedValue(const WCHAR* sFormatted);
		__int64 i64FormattedValue(const WCHAR* sFormatted);
		float fFormattedValue(const WCHAR* sFormatted);
		double dFormattedValue(const WCHAR* sFormatted);
		WCHAR* I64FormatInteger(WCHAR* sOut, int iMaxSz, __int64 i64);

		bool ParseBoolean(const WCHAR* sText);
		bool ParseBoolean(const int iValue);

		double Round(double dValue, int iDecimalPlaces);
		int Round(double dValue, int iDecimalPlaces, WCHAR* sOutput, int iSizeOfOutput);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Conversion
} //namespace::NSWFL
#endif
