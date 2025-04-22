///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_IsolationLock_CPP_
#define _NSWFL_IsolationLock_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Isolation {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		This class is designed to allow your code to passively or exclusively
			lock a segment of code. Shared (passive) locks provide a simple
			reference count of locked segments while exclusive locks disallow
			other exclusive or shared locks.
	*/

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define IsolationLockAssert(sExpression, sMsg) \
	if (sExpression) \
	{ \
		TCHAR sAssert[2048]; \
		_sntprintf_s(sAssert, _countof(sAssert), _TRUNCATE, \
			_T("%s\r\n\r\n") \
			_T("Procedure: %s\r\n") \
			_T("Source File: %s\r\n") \
			_T("Build Date/Time: %s (%s)\r\n") \
			_T("Expression: \"%s\"\r\n") \
			_T("Line: %d\r\n\r\n"), \
			_T(sMsg), _T(__FUNCTION__), _T(__FILE__), _T(__DATE__), _T(__TIME__), _T(#sExpression), __LINE__); \
		MessageBox(NULL, sAssert, _T("MemoryPool Exception"), MB_ICONSTOP | MB_SYSTEMMODAL); \
		__debugbreak(); \
	}
#else
#define IsolationLockAssert(sExpression, sMsg)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		IsolationLock::IsolationLock(void)
		{
			InitializeCriticalSection(&CS);
			this->iSharedLockCount = 0;
			this->iExclusiveLockCount = 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		IsolationLock::~IsolationLock(void)
		{
			DeleteCriticalSection(&CS);
			this->iSharedLockCount = 0;
			this->iExclusiveLockCount = 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int IsolationLock::SharedLocks(void)
		{
			return this->iSharedLockCount;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int IsolationLock::ExclusiveLocks(void)
		{
			return this->iExclusiveLockCount;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int IsolationLock::Locks(void)
		{
			return this->iSharedLockCount + this->iExclusiveLockCount;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void IsolationLock::LockShared(void)
		{
			EnterCriticalSection(&this->CS);
			IsolationLockAssert(this->iSharedLockCount < 0, "Shared lock count must be greater than zero.");
			this->iSharedLockCount++;
			LeaveCriticalSection(&this->CS);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void* IsolationLock::UnlockShared(void* pValue)
		{
			EnterCriticalSection(&this->CS);
			IsolationLockAssert(this->iSharedLockCount <= 0, "Shared lock not held.");
			this->iSharedLockCount--;
			LeaveCriticalSection(&this->CS);
			return pValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* IsolationLock::UnlockShared(WCHAR* sValue)
		{
			EnterCriticalSection(&this->CS);
			IsolationLockAssert(this->iSharedLockCount <= 0, "Shared lock not held.");
			this->iSharedLockCount--;
			LeaveCriticalSection(&this->CS);
			return sValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		double IsolationLock::UnlockShared(double dValue)
		{
			EnterCriticalSection(&this->CS);
			IsolationLockAssert(this->iSharedLockCount <= 0, "Shared lock not held.");
			this->iSharedLockCount--;
			LeaveCriticalSection(&this->CS);
			return dValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		float IsolationLock::UnlockShared(float fValue)
		{
			EnterCriticalSection(&this->CS);
			IsolationLockAssert(this->iSharedLockCount <= 0, "Shared lock not held.");
			this->iSharedLockCount--;
			LeaveCriticalSection(&this->CS);
			return fValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		__int64 IsolationLock::UnlockShared(__int64 i64Value)
		{
			EnterCriticalSection(&this->CS);
			IsolationLockAssert(this->iSharedLockCount <= 0, "Shared lock not held.");
			this->iSharedLockCount--;
			LeaveCriticalSection(&this->CS);
			return i64Value;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned __int64 IsolationLock::UnlockShared(unsigned __int64 i64Value)
		{
			EnterCriticalSection(&this->CS);
			IsolationLockAssert(this->iSharedLockCount <= 0, "Shared lock not held.");
			this->iSharedLockCount--;
			LeaveCriticalSection(&this->CS);
			return i64Value;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned int IsolationLock::UnlockShared(unsigned int iValue)
		{
			EnterCriticalSection(&this->CS);
			IsolationLockAssert(this->iSharedLockCount <= 0, "Shared lock not held.");
			this->iSharedLockCount--;
			LeaveCriticalSection(&this->CS);
			return iValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int IsolationLock::UnlockShared(int iValue)
		{
			EnterCriticalSection(&this->CS);
			IsolationLockAssert(this->iSharedLockCount <= 0, "Shared lock not held.");
			this->iSharedLockCount--;
			LeaveCriticalSection(&this->CS);
			return iValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsolationLock::UnlockShared(bool bValue)
		{
			EnterCriticalSection(&this->CS);
			IsolationLockAssert(this->iSharedLockCount <= 0, "Shared lock not held.");
			this->iSharedLockCount--;
			LeaveCriticalSection(&this->CS);
			return bValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void IsolationLock::UnlockShared(void)
		{
			EnterCriticalSection(&this->CS);
			IsolationLockAssert(this->iSharedLockCount <= 0, "Shared lock not held.");
			this->iSharedLockCount--;
			LeaveCriticalSection(&this->CS);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void IsolationLock::LockExclusive(void)
		{
			while (true) //Wait for all other locks to clear.
			{
				EnterCriticalSection(&this->CS); //Lock the resources.
				IsolationLockAssert(this->iExclusiveLockCount < 0, "Exclusive lock count must be greater than zero.");

				//If we already have an exclusive lock, then we're good. Otherwise we
				//	need to wait for the shared lock count to become zero.
				//Keep in mind that this class allows shared locks by the same thread
				//	that owns the exclusive lock.
				if (this->iSharedLockCount == 0 || this->iExclusiveLockCount > 0)
				{
					this->iExclusiveLockCount++;
					return; //Exit the function, leaving the critical section open (entered).
				}
				LeaveCriticalSection(&this->CS); //Unlock the resources.

				Sleep(1); //Give other threads time to do their jobs.
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* IsolationLock::UnlockExclusive(WCHAR* sValue)
		{
			IsolationLockAssert(this->iSharedLockCount != 0, "Shared locks still held.");
			IsolationLockAssert(this->iExclusiveLockCount <= 0, "Exclusive lock not held.");
			this->iExclusiveLockCount--;
			LeaveCriticalSection(&this->CS);
			return sValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		double IsolationLock::UnlockExclusive(double dValue)
		{
			IsolationLockAssert(this->iSharedLockCount != 0, "Shared locks still held.");
			IsolationLockAssert(this->iExclusiveLockCount <= 0, "Exclusive lock not held.");
			this->iExclusiveLockCount--;
			LeaveCriticalSection(&this->CS);
			return dValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		float IsolationLock::UnlockExclusive(float fValue)
		{
			IsolationLockAssert(this->iSharedLockCount != 0, "Shared locks still held.");
			IsolationLockAssert(this->iExclusiveLockCount <= 0, "Exclusive lock not held.");
			this->iExclusiveLockCount--;
			LeaveCriticalSection(&this->CS);
			return fValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int IsolationLock::UnlockExclusive(int iValue)
		{
			IsolationLockAssert(this->iSharedLockCount != 0, "Shared locks still held.");
			IsolationLockAssert(this->iExclusiveLockCount <= 0, "Exclusive lock not held.");
			this->iExclusiveLockCount--;
			LeaveCriticalSection(&this->CS);
			return iValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned int IsolationLock::UnlockExclusive(unsigned int iValue)
		{
			IsolationLockAssert(this->iSharedLockCount != 0, "Shared locks still held.");
			IsolationLockAssert(this->iExclusiveLockCount <= 0, "Exclusive lock not held.");
			this->iExclusiveLockCount--;
			LeaveCriticalSection(&this->CS);
			return iValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsolationLock::UnlockExclusive(bool bValue)
		{
			IsolationLockAssert(this->iSharedLockCount != 0, "Shared locks still held.");
			IsolationLockAssert(this->iExclusiveLockCount <= 0, "Exclusive lock not held.");
			this->iExclusiveLockCount--;
			LeaveCriticalSection(&this->CS);
			return bValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void* IsolationLock::UnlockExclusive(void* pValue)
		{
			IsolationLockAssert(this->iSharedLockCount != 0, "Shared locks still held.");
			IsolationLockAssert(this->iExclusiveLockCount <= 0, "Exclusive lock not held.");
			this->iExclusiveLockCount--;
			LeaveCriticalSection(&this->CS);
			return pValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void IsolationLock::UnlockExclusive(void)
		{
			IsolationLockAssert(this->iSharedLockCount != 0, "Shared locks still held.");
			IsolationLockAssert(this->iExclusiveLockCount <= 0, "Exclusive lock not held.");
			this->iExclusiveLockCount--;
			LeaveCriticalSection(&this->CS);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Isolation
} //namespace::NSWFL
#endif
