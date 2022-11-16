///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_IsolationLock_H_
#define _NSWFL_IsolationLock_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Isolation {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class IsolationLock {

		private:
			CRITICAL_SECTION CS;
			int iSharedLockCount;
			int iExclusiveLockCount;

		public:
			~IsolationLock(void);
			IsolationLock(void);

			int ExclusiveLocks(void); //Count of exclusive locks.
			int SharedLocks(void); //Count of shared locks.
			int Locks(void); //Count of all locks.

			void LockShared(void); //Obtains a shared lock.
			void LockExclusive(void); //Obtains an exclusive lock.

			__int64 UnlockShared(__int64 i64Value);
			unsigned __int64 UnlockShared(unsigned __int64 i64Value);

			bool UnlockShared(bool bValue);
			bool UnlockExclusive(bool bValue);

			char *UnlockShared(char *sValue);
			char *UnlockExclusive(char *sValue);

			void *UnlockShared(void *pValue);
			void *UnlockExclusive(void *pValue);

			double UnlockShared(double dValue);
			double UnlockExclusive(double dValue);

			float UnlockShared(float bValue);
			float UnlockExclusive(float bValue);

			int UnlockShared(int iValue);
			int UnlockExclusive(int iValue);

			unsigned int UnlockShared(unsigned int iValue);
			unsigned int UnlockExclusive(unsigned int iValue);

			void UnlockShared(void);
			void UnlockExclusive(void);
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Isolation
} //namespace::NSWFL
#endif
