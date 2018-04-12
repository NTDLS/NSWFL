///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_HIGHPRECISIONTIMER_H_
#define _NSWFL_HIGHPRECISIONTIMER_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace DateTime {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class HighPrecisionTimer
		{
		protected:
			LARGE_INTEGER _liStart;
			LARGE_INTEGER _liStop;
			LARGE_INTEGER _liFrequency;
			bool _bIsHighPrecision;

		public:
			HighPrecisionTimer(void);

			bool IsHighPrecision(void); //Returns true if the timer is highly precise.

			void Start(void); //Starts the timer.
			void Stop(void);  //Stops the timer.
			double Calculate(void); //Calculates the difference between timer Start() and Stop() events in milliseconds.
			double Delta(void); //Returns the difference between timer Start() and now in milliseconds.

			__int64 Frequency(void); // Timer frequency (for the current CPU executing the current thread).
			__int64 TickCount(void); // Gets the tickcount for manual duration calculations.
			double Calculate(__int64 i64StartTime, __int64 i64StopTime); //Calculates the difference between given times (tickcounts) in milliseconds.
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::DateTime
} //namespace::NSWFL
#endif
