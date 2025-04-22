///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_DATETIME_H_
#define _NSWFL_DATETIME_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace DateTime {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		extern const unsigned short usDaysInMonths[];
		extern const bool bWeekDays[7];
		extern const char* sDayNames[8];

#ifdef _TIME_T_DEFINED
		__int64 FileTimeToInt64(FILETIME f);
		FILETIME Int64ToFileTime(__int64 t);
		FILETIME GetFileTimeFromYear(WORD year);
		__time64_t GetYDayFromSystemTime(const SYSTEMTIME* s);
		SYSTEMTIME TmToSystemTime(struct tm* t);
		struct tm SystemTimeToTm(SYSTEMTIME* s);
		__time64_t FileTimeToUnixTime(const FILETIME* f);
		FILETIME UnitTimeToFileTime(const __time64_t t);
		FILETIME UnixTimeToFileTime(__time64_t t);
		bool UnixTimeToSystemTime(__time64_t ttTime, LPSYSTEMTIME pST);
		bool UnixTimeToLocalTime(__time64_t ttTime, LPSYSTEMTIME pLT);
		__time64_t FileTimeToUnixTime(FILETIME FileTime);
		bool DateAddSystem(const char* sInterval, int iNumber, __time64_t* tUnixTime);
		bool DateAddLocal(const char* sInterval, int iNumber, __time64_t* tUnixTime);
		bool DateAddSystem(const char* sInterval, int iNumber, SYSTEMTIME* lpST);
		bool DateAddLocal(const char* sInterval, int iNumber, SYSTEMTIME* lpST);
		bool DateAdd(const char* sInterval, int iNumber, tm* tmTimeStamp);
#endif

		int DayNameToNumber(const char* sDayName);
		const char* DayNumberToName(const int iNumber);
		DWORD TimeToLong(int iHour, int iMin, int iSec);
		void LongToTime(DWORD dwTime, int* piHour, int* piMin, int* piSec);
		DWORD DateToLong(int iYear, int iMonth, int iDay);
		void LongToDate(DWORD dwDate, int* piYear, int* piMonth, int* piDay);
		DWORD DateToLong(SYSTEMTIME* ST);
		DWORD TimeToLong(SYSTEMTIME* ST);
		DWORD LocalTimeToLong(void);
		DWORD SystemTimeToLong(void);
		DWORD LocalDateToLong(void);
		DWORD SystemDateToLong(void);

		int GetMonthNumber(const char* name);
		int GetWeekdayNumber(const char* name);
		int CompareDateTime(SYSTEMTIME* t1, SYSTEMTIME* t2);

		int Get_Time(char* sOutVal, int iMaxSz);
		int Get_Time(char* sOutVal, int iMaxSz, DWORD dwFlags);
		int Get_LongDate(char* sOutVal, int iMaxSz);
		int Get_ShortDate(char* sOutVal, int iMaxSz);
		void Get_LocalDateTime(DWORD* dwDate, DWORD* dwTime);
		void Get_SystemDateTime(DWORD* dwDate, DWORD* dwTime);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::DateTime
} //namespace::NSWFL
#endif
