///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
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

		char *FloatChar(char *sOutVal, int iMaxSz, float fInput);
		char *DoubleChar(char *sOutVal, int iMaxSz, double dInput);
		char *IntChar(char *sOutVal, int iMaxSz, int iInput);
		char *LongChar(char *sOutVal, int iMaxSz, DWORD dwInput);
		char *FloatChar(char *sOutVal, int iMaxSz, float fInput, int iDecPlaces);
		long CharLong(const char *sInput);
		float CharFloat(const char *sInput);
		double CharDouble(const char *sInput);
		int CharInt(const char *sInput);
		int HexToDecI(const char *sHexString);
		unsigned int HexToDecU(const char *sHexString);
		__int64 HexToDecI64(const char *sHexString);
		unsigned __int64 HexToDecU64(const char *sHexString);
		int Bin2Hex(void *lpTarget, int iMaxSz, const void *lpSrc, int iSz, int iBytes);
		int Bin2Hex(void *lpTarget, int iMaxSz, const void *lpSrc, int iSz, int iBytes, bool bPrefix);
		int Bin2HexFriendly(void *lpTarget, int iMaxSz, const void *lpSrc, int iSz, int iBytes, int iWidth);

		char *FormatFloatString(char *sOut, int iMaxSz, const char *sIn);
		char *FormatFloat(char *sOut, int iMaxSz, float fValue, int iDecimalPlaces);
		char *FormatDouble(char *sOut, int iMaxSz, double dValue, int iDecimalPlaces);
		char *FormatDoubleString(char *sOut, int iMaxSz, const char *sIn);
		char *FormatFloatString(char *sOut, int iMaxSz, const char *sIn);
		char *FormatiString(char *sOut, int iMaxSz, const char *sIn);
		char *FormatInteger(char *sOut, int iMaxSz, const long lIn);
		int iFormattedValue(const char *sFormatted);
		unsigned int uiFormattedValue(const char *sFormatted);
		__int64 i64FormattedValue(const char *sFormatted);
		float fFormattedValue(const char *sFormatted);
		double dFormattedValue(const char *sFormatted);
		char *I64FormatInteger(char *sOut, int iMaxSz, __int64 i64);

		bool ParseBoolean(const char *sText);
		bool ParseBoolean(const int iValue);

		double Round(double dValue, int iDecimalPlaces);
		int Round(double dValue, int iDecimalPlaces, char *sOutput, int iSizeOfOutput);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Conversion
} //namespace::NSWFL
#endif
