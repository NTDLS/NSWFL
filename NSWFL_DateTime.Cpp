///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_DATETIME_CPP_
#define _NSWFL_DATETIME_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER //_MSC_VER is defined when compiling with Microsoft Visual Studio (2003, 2005, 2008+)
#include <ATLTime.H>
#endif

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace DateTime {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		const unsigned short usDaysInMonths[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		const bool bWeekDays[7] = { 0,1,1,1,1,1,0 };
		const char *sDayNames[8] = {
			"Sunday",
			"Monday",
			"Tuesday",
			"Wednesday",
			"Thursday",
			"Friday",
			"Saturday",
			NULL
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Returns:
			-1 First file time is earlier than second file time.
			0 First file time is equal to second file time.
			1 First file time is later than second file time.
		*/
		int CompareDateTime(SYSTEMTIME *t1, SYSTEMTIME *t2)
		{
			FILETIME f1;
			FILETIME f2;

			memset(&f1, 0, sizeof(f1));
			memset(&f2, 0, sizeof(f2));

			if (SystemTimeToFileTime(t1, &f1))
			{
				if (SystemTimeToFileTime(t2, &f2))
				{
					return CompareFileTime(&f1, &f2);
				}
			}
			return -255;
		}

		//You can use SystemTimeToFileTime to convert the SYSTEMTIME values to
		//FILETIME and then use CompareFileTime.

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int GetWeekdayNumber(const char *name)
		{
			const char *sDays[6][8] =
			{
				{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", NULL},
				{"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa", NULL},
				{"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", NULL},
				{"", "", "Tues", "", "Thur", "", "", NULL},
				{"", "", "", "", "Thurs", "", "", NULL},
				{NULL}
			};

			for (int set = 0; sDays[set][0]; set++)
			{
				for (int day = 0; sDays[set][day]; day++)
				{
					if (_strcmpi(sDays[set][day], name) == 0)
					{
						return day + 1;
					}
				}
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int GetMonthNumber(const char *name)
		{
			const char *sMonths[3][13] =
			{
				{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL},
				{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December", NULL},
				{NULL}
			};

			for (int set = 0; sMonths[set][0]; set++)
			{
				for (int month = 0; sMonths[set][month]; month++)
				{
					if (_strcmpi(sMonths[set][month], name) == 0)
					{
						return month + 1;
					}
				}
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Get_Time(char *sOutVal, int iMaxSz, DWORD dwFlags)
		{
			SYSTEMTIME ST;
			GetLocalTime(&ST);
			return GetTimeFormat(LOCALE_USER_DEFAULT, dwFlags, &ST, NULL, sOutVal, iMaxSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Get_Time(char *sOutVal, int iMaxSz)
		{
			SYSTEMTIME ST;
			GetLocalTime(&ST);
			return GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &ST, NULL, sOutVal, iMaxSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Get_LongDate(char *sOutVal, int iMaxSz)
		{
			SYSTEMTIME ST;
			GetLocalTime(&ST);
			return GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &ST, NULL, sOutVal, iMaxSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Get_ShortDate(char *sOutVal, int iMaxSz)
		{
			SYSTEMTIME ST;
			GetLocalTime(&ST);
			return GetDateFormat((LCID)NULL, (DWORD)NULL, &ST, "MM-dd-yyyy", sOutVal, iMaxSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Get_LocalDateTime(DWORD *dwDate, DWORD *dwTime)
		{
			SYSTEMTIME ST;
			GetLocalTime(&ST);

			*dwDate = NSWFL::DateTime::TimeToLong(ST.wHour, ST.wMinute, ST.wSecond);
			*dwTime = NSWFL::DateTime::DateToLong(ST.wYear, ST.wMonth, ST.wDay);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Get_SystemDateTime(DWORD *dwDate, DWORD *dwTime)
		{
			SYSTEMTIME ST;
			GetSystemTime(&ST);

			*dwDate = NSWFL::DateTime::TimeToLong(ST.wHour, ST.wMinute, ST.wSecond);;
			*dwTime = NSWFL::DateTime::DateToLong(ST.wYear, ST.wMonth, ST.wDay);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Converts a day number into a day name. Example" DayNumberToName(0) return "Sunday"
		*/
		const char *DayNumberToName(const int iNumber)
		{
			if (iNumber >= 0 && iNumber <= 7)
			{
				return sDayNames[iNumber];
			}
			else {
				return NULL;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Converts a day name into a day number. Example" DayNameToNumber("Monday") return 1
		*/
		int DayNameToNumber(const char *sDayName)
		{
			for (int i = 0; sDayNames[i]; i++)
			{
				if (NSWFL::String::StrEqlI(sDayNames[i], sDayName))
				{
					return i;
				}
			}
			return -1;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Convert Hour, Minute & Second To A DWORD Value

		DWORD LocalTimeToLong(void)
		{
			SYSTEMTIME ST;
			GetLocalTime(&ST);
			return NSWFL::DateTime::TimeToLong(&ST);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Convert Hour, Minute & Second To A DWORD Value

		DWORD SystemTimeToLong(void)
		{
			SYSTEMTIME ST;
			GetSystemTime(&ST);
			return NSWFL::DateTime::TimeToLong(&ST);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Convert Hour, Minute & Second To A DWORD Value

		DWORD TimeToLong(SYSTEMTIME *ST)
		{
			return NSWFL::DateTime::TimeToLong(ST->wHour, ST->wMinute, ST->wSecond);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Convert Hour, Minute & Second To A DWORD Value

		DWORD TimeToLong(int iHour, int iMin, int iSec)
		{
			return (iHour * 60l + iMin) * 60l + iSec;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Convert A DWORD Value To Hour, Minute & Second

		void LongToTime(DWORD dwTime, int *piHour, int *piMin, int *piSec)
		{
			*piSec = (int)dwTime % 60l;
			dwTime /= 60l;
			*piMin = (int)dwTime % 60l;
			*piHour = (int)dwTime / 60l;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Convert Year, Month & Day To A DWORD Value

		DWORD LocalDateToLong(void)
		{
			SYSTEMTIME ST;
			GetLocalTime(&ST);
			return NSWFL::DateTime::DateToLong(&ST);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Convert Year, Month & Day To A DWORD Value

		DWORD SystemDateToLong(void)
		{
			SYSTEMTIME ST;
			GetSystemTime(&ST);
			return NSWFL::DateTime::DateToLong(&ST);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Convert Year, Month & Day To A DWORD Value

		DWORD DateToLong(SYSTEMTIME *ST)
		{
			return NSWFL::DateTime::DateToLong(ST->wYear, ST->wMonth, ST->wDay);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Convert Year, Month & Day To A DWORD Value

		DWORD DateToLong(int iYear, int iMonth, int iDay)
		{
			int a = 0;
			int b = 0;

			int year = iYear;
			int month = iMonth;
			int day = iDay;

			float year_corr = 0;

			if (year < 0) year++;

			year_corr = (float)(year > 0 ? 0.0 : 0.75);

			if (month <= 2)
			{
				year--;
				month += 12;
			}

			b = 0;

			if (year * 10000.0 + month * 100.0 + day >= 15821015.0)
			{
				a = year / 100;
				b = 2 - a + a / 4;
			}

			return (DWORD)(365.25 * year - year_corr) + (DWORD)(30.6001 * (month + 1)) + day + 1720995L + b;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Convert A DWORD Value To Year, Month & Day

		void LongToDate(DWORD dwDate, int *piYear, int *piMonth, int *piDay)
		{
			DWORD a = 0;
			DWORD b = 0;
			DWORD c = 0;
			DWORD d = 0;
			DWORD e = 0;
			DWORD z = 0;
			DWORD alpha = 0;

			z = dwDate;

			if (z < 2299161L) a = z;
			else {
				alpha = (DWORD)((z - 1867216.25) / 36524.25);
				a = z + 1 + alpha - alpha / 4;
			}

			b = a + 1524;
			c = (DWORD)((b - 122.1) / 365.25);
			d = (DWORD)(365.25 * c);
			e = (DWORD)((b - d) / 30.6001);

			*piDay = (int)(b - d - (DWORD)(30.6001 * e));
			*piMonth = (int)(e < 13.5) ? e - 1 : e - 13;
			*piYear = (int)(*piMonth > 2.5) ? (c - 4716) : c - 4715;

			if (*piYear <= 0) *piYear -= 1;

			return;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _TIME_T_DEFINED
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* __int64 <--> FILETIME */
		__int64 FileTimeToInt64(FILETIME f)
		{
			__int64 t;

			t = f.dwHighDateTime;
			t <<= 32;
			t |= f.dwLowDateTime;
			return t;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		FILETIME Int64ToFileTime(__int64 t)
		{
			FILETIME f;
			f.dwHighDateTime = (DWORD)((t >> 32) & 0x00000000FFFFFFFF);
			f.dwLowDateTime = (DWORD)(t & 0x00000000FFFFFFFF);
			return f;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/* FILETIME utility */
		FILETIME GetFileTimeFromYear(WORD year)
		{
			SYSTEMTIME s = { 0 };
			FILETIME f;

			s.wYear = year;
			s.wMonth = 1;
			s.wDayOfWeek = 1;
			s.wDay = 1;

			SystemTimeToFileTime(&s, &f);
			return f;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		__time64_t GetYDayFromSystemTime(const SYSTEMTIME* s)
		{
			__int64 t;
			FILETIME f1, f2;

			f1 = GetFileTimeFromYear(s->wYear);
			SystemTimeToFileTime(s, &f2);

			t = NSWFL::DateTime::FileTimeToInt64(f2) - FileTimeToInt64(f1);

			return (__time64_t)((t / (__int64)10000000) / (60 * 60 * 24));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
/* tm <--> SYSTEMTIME */
		SYSTEMTIME TmToSystemTime(struct tm *t)
		{
			SYSTEMTIME s;

			s.wYear = t->tm_year + 1900;
			s.wMonth = t->tm_mon + 1;
			s.wDayOfWeek = t->tm_wday;
			s.wDay = t->tm_mday;
			s.wHour = t->tm_hour;
			s.wMinute = t->tm_min;
			s.wSecond = t->tm_sec;
			s.wMilliseconds = 0;

			return s;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
		struct tm SystemTimeToTm(SYSTEMTIME *s)
		{
			struct tm t;

			t.tm_year = s->wYear - 1900;
			t.tm_mon = s->wMonth - 1;
			t.tm_wday = s->wDayOfWeek;
			t.tm_mday = s->wDay;
			t.tm_yday = (int)GetYDayFromSystemTime(s);
			t.tm_hour = s->wHour;
			t.tm_min = s->wMinute;
			t.tm_sec = s->wSecond;
			t.tm_isdst = 0;

			return t;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/* FILETIME <--> time_t */
		__time64_t FileTimeToUnixTime(const FILETIME* f)
		{
			FILETIME f1601, f1970;
			__int64 t, offset;

			f1601 = GetFileTimeFromYear(1601);
			f1970 = GetFileTimeFromYear(1970);

			offset = NSWFL::DateTime::FileTimeToInt64(f1970) - FileTimeToInt64(f1601);

			t = NSWFL::DateTime::FileTimeToInt64(*f);

			t -= offset;
			return (__time64_t)(t / (__int64)10000000);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		FILETIME UnitTimeToFileTime(const __time64_t t)
		{
			FILETIME f, f1970;
			__int64 time;

			f1970 = GetFileTimeFromYear(1970);

			time = t;
			time *= (__int64)10000000;
			time += NSWFL::DateTime::FileTimeToInt64(f1970);

			f = NSWFL::DateTime::Int64ToFileTime(time);

			return f;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
		FILETIME UnixTimeToFileTime(__time64_t t)
		{
			FILETIME retf;
			__int64 i64 = Int32x32To64(t, 10000000) + 116444736000000000;
			retf.dwLowDateTime = (DWORD)i64;
			retf.dwHighDateTime = (DWORD)(i64 >> 32);
			return retf;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
		bool UnixTimeToSystemTime(__time64_t ttTime, LPSYSTEMTIME pST)
		{
			FILETIME pft = NSWFL::DateTime::UnixTimeToFileTime(ttTime);

			if (!FileTimeToSystemTime(&pft, pST))
				return false;

			return true;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
		bool UnixTimeToLocalTime(__time64_t ttTime, LPSYSTEMTIME pLT)
		{
			SYSTEMTIME pST;
			TIME_ZONE_INFORMATION TZI;

			if (!NSWFL::DateTime::UnixTimeToSystemTime(ttTime, &pST))
				return false;
			if (GetTimeZoneInformation(&TZI) == 0xFFFFFFFF)
				return false;
			if (!SystemTimeToTzSpecificLocalTime(&TZI, &pST, pLT))
				return false;

			return true;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
		__time64_t FileTimeToUnixTime(FILETIME FileTime)
		{
			static const __int64 SECS_BETWEEN_EPOCHS = __int64(11644473600);
			static const __int64 SECS_TO_100NS = 10000000; /* 10^7 */

			/* get the full win32 value, in 100ns */
			__int64 i64UnixTime = ((__int64)FileTime.dwHighDateTime << 32) + FileTime.dwLowDateTime;

			/* convert to the Unix epoch */
			i64UnixTime -= (SECS_BETWEEN_EPOCHS * SECS_TO_100NS);

			//*nsec = (UnixTime % SECS_TO_100NS) * 100; //NANO seconds.

			i64UnixTime /= SECS_TO_100NS; /* now convert to seconds */

			return (__time64_t)i64UnixTime;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
		bool DateAddSystem(const char *sInterval, int iNumber, SYSTEMTIME *lpST)
		{
			tm tmTimeStamp;
			__time64_t tUnixTime;

			FILETIME FileTime;

			if (SystemTimeToFileTime(lpST, &FileTime))
			{
				tUnixTime = NSWFL::DateTime::FileTimeToUnixTime(FileTime);
			}

			_gmtime64_s(&tmTimeStamp, &tUnixTime);

			NSWFL::DateTime::DateAdd(sInterval, iNumber, &tmTimeStamp);

			if (_mktime64(&tmTimeStamp) != (__time64_t)-1)
			{
				*lpST = NSWFL::DateTime::TmToSystemTime(&tmTimeStamp);
			}
			else {
				perror("mktime failed");
				return false;
			}

			return true;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
		bool DateAddLocal(const char *sInterval, int iNumber, SYSTEMTIME *lpST)
		{
			tm tmTimeStamp;
			__time64_t tUnixTime;

			FILETIME FileTime;
			FILETIME LocalFileTime;

			if (SystemTimeToFileTime(lpST, &FileTime))
			{
				if (LocalFileTimeToFileTime(&FileTime, &LocalFileTime))
				{
					tUnixTime = NSWFL::DateTime::FileTimeToUnixTime(LocalFileTime);
				}
			}

			_localtime64_s(&tmTimeStamp, &tUnixTime);

			NSWFL::DateTime::DateAdd(sInterval, iNumber, &tmTimeStamp);

			if (_mktime64(&tmTimeStamp) != (__time64_t)-1)
			{
				*lpST = NSWFL::DateTime::TmToSystemTime(&tmTimeStamp);
			}
			else {
				perror("mktime failed");
				return false;
			}

			return true;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
		bool DateAddLocal(const char *sInterval, int iNumber, __time64_t *tUnixTime)
		{
			tm tmTimeStamp;
			_localtime64_s(&tmTimeStamp, tUnixTime);
			return NSWFL::DateTime::DateAdd(sInterval, iNumber, &tmTimeStamp);
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
		bool DateAddSystem(const char *sInterval, int iNumber, __time64_t *tUnixTime)
		{
			tm tmTimeStamp;
			_gmtime64_s(&tmTimeStamp, tUnixTime);
			return NSWFL::DateTime::DateAdd(sInterval, iNumber, &tmTimeStamp);
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
		bool DateAdd(const char *sInterval, int iNumber, tm *tmTimeStamp)
		{
			if (NSWFL::String::StrEqlI(sInterval, "s")) //Second
				tmTimeStamp->tm_sec += iNumber;
			else if (NSWFL::String::StrEqlI(sInterval, "n")) //Minute
				tmTimeStamp->tm_min += iNumber;
			else if (NSWFL::String::StrEqlI(sInterval, "h")) //Hour
				tmTimeStamp->tm_hour += iNumber;
			else if (NSWFL::String::StrEqlI(sInterval, "d")) //Day
				tmTimeStamp->tm_mday += iNumber;
			else if (NSWFL::String::StrEqlI(sInterval, "m")) //Month
				tmTimeStamp->tm_mon += iNumber;
			else if (NSWFL::String::StrEqlI(sInterval, "y")) //Year
				tmTimeStamp->tm_year += iNumber;

			if (!(_mktime64(tmTimeStamp) != (__time64_t)-1))
			{
				return false;
			}

			return true;
		}
#endif
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::DateTime
} //namespace::NSWFL
#endif
