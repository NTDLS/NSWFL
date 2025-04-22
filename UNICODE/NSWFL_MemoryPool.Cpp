///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_MEMORYPOOL_CPP_
#define _NSWFL_MEMORYPOOL_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Memory {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
#include <ConIO.H>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
#ifdef UNICODE
#define MemoryPoolAssert(sExpression, sMsg) \
					if(sExpression)\
					{ \
						this->MemoryPoolAssertEx(_T(#sExpression), sMsg, _T(__FUNCTION__), _T(__FILE__), _T(__DATE__), _T(__TIME__), __LINE__); \
					}
#else
#define MemoryPoolAssert(sExpression, sMsg) \
					if(sExpression)\
					{ \
						this->MemoryPoolAssertEx(_T(#sExpression), sMsg, _T(__FUNCTION__), _T(__FILE__), _T(__DATE__), _T(__TIME__), __LINE__); \
					}
#endif
#else
#define MemoryPoolAssert(sExpression, sMsg)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MemoryPool::~MemoryPool(void)
		{
			this->Destroy();
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MemoryPool::MemoryPool(void)
		{
			this->Initialize();
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		MemoryPool::MemoryPool(size_t iAllocateIncrement)
		{
			this->Initialize(iAllocateIncrement);
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
		size_t MemoryPool::ReservedBytes(void)
		{
			this->Lock();
			size_t iBytes = this->Items.iReservedBytes;
			this->Unlock();
			return iBytes;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool MemoryPool::IsDebug(void)
		{
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
			return true;
#else
			return false;
#endif
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		size_t MemoryPool::GetMemFreeCount(void)
		{
			return(this->Items.iAllocated - this->Items.iUsedCount);
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		size_t MemoryPool::GetMemAllocationCount(void)
		{
			return this->Items.iUsedCount;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		size_t MemoryPool::GetSlotAllocationCount(void)
		{
			return this->Items.iAllocated;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		bool MemoryPool::FreeAll(void)
		{
			bool bResult = true;

			//Free all the memory stored in the [Array] structure.
			for (register size_t iSlot = 0; iSlot < this->Items.iAllocated; iSlot++)
			{
				if (this->Items.Array[iSlot].iAddress)
				{
					swprintf_s(this->sDebugText, sizeof(this->sDebugText),
						_T("Memory leak detected while destroying a managed memory pool item at [0x%zX].")
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
						_T("\r\n\r\nAllocated in \"%s\"")
						_T("\r\non line %zd for %zd bytes.")
#endif
						, this->Items.Array[iSlot].iAddress
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
						, this->Items.Array[iSlot].sCodeFile,
						this->Items.Array[iSlot].iLineOfCode,
						this->Items.Array[iSlot].iSize
#endif						
					);
					MemoryPoolAssert(_T("FreeAll"), this->sDebugText);

					if (!this->Free((void*)this->Items.Array[iSlot].iAddress))
					{
						bResult = false;
					}
				}
			}

			return bResult;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		size_t MemoryPool::GetSlotByAddress(const size_t iAddress)
		{
			for (register size_t iSlot = 0; iSlot < this->Items.iAllocated; iSlot++)
			{
				if (this->Items.Array[iSlot].iAddress == iAddress)
				{
					return iSlot;
				}
			}

			swprintf_s(this->sDebugText, sizeof(this->sDebugText),
				_T("Application attempted to access an unmanaged memory address at [0x%zX]."), iAddress);
			MemoryPoolAssert(_T("GetSlotByAddress"), this->sDebugText);

			return -1; //Did not find the address specified.
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		size_t MemoryPool::FindFreeAllocSlot(void)
		{
			for (register size_t iSlot = 0; iSlot < this->Items.iAllocated; iSlot++)
			{
				if (this->Items.Array[iSlot].iAddress == 0)
				{
					return iSlot;
				}
			}

			//Increase the allocation increment by 25%
			this->Items.iAllocIncrement += (size_t)((float)this->Items.iAllocIncrement * 0.25f);

			size_t iMemory = (sizeof(MEMITEM) * this->Items.iAllocated)
				+ (sizeof(MEMITEM) * this->Items.iAllocIncrement);

			if ((this->Items.Array = (MEMITEM*)realloc(this->Items.Array, iMemory)) == NULL)
			{
				swprintf_s(this->sDebugText, sizeof(this->sDebugText),
					_T("Failed to reallocate the managed Array structure for [%zd] bytes."), iMemory);
				MemoryPoolAssert(_T("FindFreeAllocSlot"), this->sDebugText);

				return -1;
			}

			size_t iPreviousCeleing = this->Items.iAllocated;
			this->Items.iAllocated += this->Items.iAllocIncrement;

			//Clean the new slots.
			for (register size_t iSlot = iPreviousCeleing; iSlot < this->Items.iAllocated; iSlot++)
			{
				memset(&this->Items.Array[iSlot], 0, sizeof(MEMITEM));
			}

			return this->FindFreeAllocSlot();
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		bool MemoryPool::Initialize(size_t iAllocateIncrement)
		{
			InitializeCriticalSection(&this->csMemLock);

			memset(&this->Items, 0, sizeof(Items));
			this->Items.iAllocIncrement = iAllocateIncrement;

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
			this->hStdOut = NULL;
#endif

			if ((this->Items.Array = (MEMITEM*)
				calloc(sizeof(MEMITEM), this->Items.iAllocIncrement)) == NULL)
			{
				MemoryPoolAssert(_T("Initialize"), _T("Memory pool Failed to initialize."));

				DeleteCriticalSection(&this->csMemLock);
				return false;
			}

			this->Items.iAllocated = this->Items.iAllocIncrement;

			for (register size_t iSlot = 0; iSlot < this->Items.iAllocated; iSlot++)
			{
				this->Items.Array[iSlot].iAddress = 0;
#if _MEMPOOL_DEBUG_LEVEL > _MEMPOOL_DEBUG_ADVANCED
				this->Items.Array[iSlot].iSize = 0;
				this->Items.Array[iSlot].iLineOfCode = -1;
				this->Items.Array[iSlot].sCodeFile = NULL;
#endif
			}

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
			return this->OpenConsole();
#else
			return true;
#endif
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool MemoryPool::Initialize(void)
		{
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
			return this->Initialize(100);
#else
#if _MEMPOOL_SEQUENTIAL_WHEN_OPTIMIZED != 0
			InitializeCriticalSectionAndSpinCount(&this->csMemLock, 4000);
#endif
			return true;
#endif
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		bool MemoryPool::Destroy(void)
		{
			bool bResult = false;

			this->Lock();

			bResult = this->FreeAll();

			free(this->Items.Array);

			this->Unlock();

			DeleteCriticalSection(&this->csMemLock);

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
			bResult = bResult && this->CloseConsole();
#endif

			return bResult;
		}
#else
		bool MemoryPool::Destroy(void)
		{
#if _MEMPOOL_SEQUENTIAL_WHEN_OPTIMIZED != 0
			DeleteCriticalSection(&this->csMemLock);
#elif _MEMPOOL_DEBUG_LEVEL > _MEMPOOL_DEBUG_DISABLED
			DeleteCriticalSection(&this->csMemLock);
#endif
			return true;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void MemoryPool::Lock(void)
		{
			EnterCriticalSection(&this->csMemLock);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void MemoryPool::Unlock(void)
		{
			LeaveCriticalSection(&this->csMemLock);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		bool MemoryPool::Free(void* lpMemory)
		{
			this->Lock();

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
			swprintf_s(this->sDebugText, sizeof(this->sDebugText), _T("Freeing address 0x%X\n"), lpMemory);
			this->DbgWrite(this->sDebugText);
#endif

			size_t iSlot = 0;

			if (lpMemory == NULL)
			{
				MemoryPoolAssert(_T("Free"), _T("Application attempted to free a NULL pointer."));

				this->Unlock();
				return false;
			}

			if ((iSlot = this->GetSlotByAddress((size_t)lpMemory)) != (size_t)-1)
			{
				//Free the memory that the user allocated, also mark the [Array] slot as free.
				free(lpMemory);

				lpMemory = NULL;

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
				free(this->Items.Array[iSlot].sCodeFile);
				this->Items.iReservedBytes -= this->Items.Array[iSlot].iSize;
				this->Items.Array[iSlot].sCodeFile = NULL;
				this->Items.Array[iSlot].iLineOfCode = 0;
				this->Items.Array[iSlot].iSize = 0;
#endif

				this->Items.Array[iSlot].iAddress = 0;

				this->Items.iUsedCount--;
			}
			else {
				swprintf_s(this->sDebugText, sizeof(this->sDebugText),
					_T("Address [0x%zX] not found in the managed memory pool."), (size_t)lpMemory);
				MemoryPoolAssert(_T("Free"), this->sDebugText);

				this->Unlock();
				return false;
			}

			this->Unlock();
			return true;
		}
#else
		bool MemoryPool::Free(void* lpMemory)
		{
#if _MEMPOOL_SEQUENTIAL_WHEN_OPTIMIZED != 0
			this->Lock();
			free(lpMemory);
			this->Unlock();
#else
			free(lpMemory);
#endif
			return true;
		}
#endif


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
		void* MemoryPool::_dbg_ReAllocate(void* lpMemory, const size_t iSize, const size_t iCount,
			const WCHAR* sCodeFile, const size_t iLineOfCode)
#else
		void* MemoryPool::ReAllocate(void* lpMemory, const size_t iSize, const size_t iCount)
#endif
		{
			if (lpMemory)
			{
				this->Lock();

				size_t iSlot = 0;
				WCHAR* sMemory = NULL;

				if ((iSlot = this->GetSlotByAddress((size_t)lpMemory)) != (size_t)-1)
				{
					if ((sMemory = (WCHAR*)realloc(lpMemory, iSize * iCount)) == NULL)
					{
						swprintf_s(this->sDebugText, sizeof(this->sDebugText),
							_T("Failed to reallocate managed user memory for [%zd] bytes."), iSize * iCount);
						MemoryPoolAssert(_T("ReAllocate"), this->sDebugText);

						this->Unlock();
						return NULL;
					}

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
					swprintf_s(this->sDebugText, sizeof(this->sDebugText),
						_T("Realloc %d bytes. (0x%X -> 0x%X) @ %s:%d\n"),
						iSize * iCount, lpMemory, sMemory,
						this->Items.Array[iSlot].sCodeFile,
						this->Items.Array[iSlot].iLineOfCode);
					this->DbgWrite(this->sDebugText);
#endif

					this->Items.Array[iSlot].iAddress = (size_t)sMemory;
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
					this->Items.iReservedBytes -= this->Items.Array[iSlot].iSize;
					this->Items.Array[iSlot].iSize = iSize * iCount;
					this->Items.iReservedBytes += this->Items.Array[iSlot].iSize;
#endif
				}
				else {
					swprintf_s(this->sDebugText, sizeof(this->sDebugText),
						_T("Address [0x%zX] not found in the managed memory pool."), (size_t)lpMemory);
					MemoryPoolAssert(_T("ReAllocate"), this->sDebugText);

					this->Unlock();
					return NULL;
				}

				this->Unlock();
				return sMemory;
			}
			else {
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
				return this->_dbg_Allocate(iSize, iCount, sCodeFile, iLineOfCode);
#else
				return this->Allocate(iSize, iCount);
#endif
			}
		}
#else
		void* MemoryPool::ReAllocate(void* lpMemory, const size_t iSize, const size_t iCount)
		{
#if _MEMPOOL_SEQUENTIAL_WHEN_OPTIMIZED != 0
			this->Lock();
			WCHAR* sMemory = (WCHAR*)realloc(lpMemory, iSize * iCount);
			this->Unlock();

			return sMemory;
#else
			return (WCHAR*)realloc(lpMemory, iSize * iCount);
#endif
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
		WCHAR* MemoryPool::_dbg_CloneStringNSafe(WCHAR*& sOutBuffer, const WCHAR* sString, size_t iLength, const WCHAR* sCodeFile, const size_t iLineOfCode)
#else
		WCHAR* MemoryPool::CloneStringNSafe(WCHAR*& sOutBuffer, const WCHAR* sString, size_t iLength)
#endif
		{
			if (sOutBuffer != NULL)
			{
				this->Free(sOutBuffer);
			}

			size_t iCount = iLength + 1;
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED

			sOutBuffer = (WCHAR*)this->_dbg_Allocate(sizeof(WCHAR), iCount, sCodeFile, iLineOfCode);
#else
			sOutBuffer = (WCHAR*)this->Allocate(sizeof(WCHAR), iCount);
#endif
			if (sOutBuffer)
			{
				wcsncpy_s(sOutBuffer, iCount, sString, iLength);
			}
			return sOutBuffer;
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
		WCHAR* MemoryPool::_dbg_CloneStringN(const WCHAR* sString, size_t iLength, const WCHAR* sCodeFile, const size_t iLineOfCode)
#else
		WCHAR* MemoryPool::CloneStringN(const WCHAR* sString, size_t iLength)
#endif
		{
			size_t iCount = iLength + 1;
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
			WCHAR* sNewText = (WCHAR*)this->_dbg_Allocate(sizeof(WCHAR), iCount, sCodeFile, iLineOfCode);
#else
			WCHAR* sNewText = (WCHAR*)this->Allocate(sizeof(WCHAR), iCount);
#endif
			if (sNewText)
			{
				wcsncpy_s(sNewText, iCount, sString, iLength);
			}
			return sNewText;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
		WCHAR* MemoryPool::_dbg_CloneStringSafe(WCHAR*& sOutBuffer, const WCHAR* sString, const WCHAR* sCodeFile, const size_t iLineOfCode)
#else
		WCHAR* MemoryPool::CloneStringSafe(WCHAR*& sOutBuffer, const WCHAR* sString)
#endif
		{
			if (sOutBuffer != NULL)
			{
				this->Free(sOutBuffer);
			}

			size_t iCount = wcslen(sString) + 1;
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
			sOutBuffer = (WCHAR*)this->_dbg_Allocate(sizeof(WCHAR), iCount, sCodeFile, iLineOfCode);
#else
			sOutBuffer = (WCHAR*)this->Allocate(sizeof(WCHAR), iCount);
#endif
			if (sOutBuffer)
			{
				wcscpy_s(sOutBuffer, iCount, sString);
			}
			return sOutBuffer;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
		WCHAR* MemoryPool::_dbg_CloneString(const WCHAR* sString, const WCHAR* sCodeFile, const size_t iLineOfCode)
#else
		WCHAR* MemoryPool::CloneString(const WCHAR* sString)
#endif
		{
			size_t iCount = wcslen(sString) + 1;
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
			WCHAR* sNewText = (WCHAR*)this->_dbg_Allocate(sizeof(WCHAR), iCount, sCodeFile, iLineOfCode);
#else
			WCHAR* sNewText = (WCHAR*)this->Allocate(sizeof(WCHAR), iCount);
#endif
			if (sNewText)
			{
				wcscpy_s(sNewText, iCount, sString);
			}
			return sNewText;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
		void* MemoryPool::_dbg_Allocate(const size_t iSize, const size_t iCount,
			const WCHAR* sCodeFile, const size_t iLineOfCode)
#else
		void* MemoryPool::Allocate(const size_t iSize, const size_t iCount)
#endif
		{
			this->Lock();

			WCHAR* sMemory = NULL;
			size_t iSlot = 0;

			if ((sMemory = (WCHAR*)calloc(iSize, iCount)) == NULL)
			{
				swprintf_s(this->sDebugText, sizeof(this->sDebugText),
					_T("Failed to allocate managed user memory for [%zd] bytes."), iSize * iCount);
				MemoryPoolAssert(_T("Allocate"), this->sDebugText);

				this->Unlock();
				return NULL;
			}

			if ((iSlot = this->FindFreeAllocSlot()) == -1)
			{
				swprintf_s(this->sDebugText, sizeof(this->sDebugText),
					_T("Failed to find qualified memory slot for [%zd] bytes."), iSize * iCount);
				MemoryPoolAssert(_T("Allocate"), this->sDebugText);

				this->Unlock();
				return NULL;
			}

			this->Items.Array[iSlot].iAddress = (size_t)sMemory;

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
			this->Items.Array[iSlot].iSize = (iCount * iSize);
			this->Items.Array[iSlot].iLineOfCode = iLineOfCode;
			this->Items.iReservedBytes += this->Items.Array[iSlot].iSize;

			size_t iCodeFileAlloc = this->GetFileName(sCodeFile, sTempFile) + 1;
			this->Items.Array[iSlot].sCodeFile = (WCHAR*)calloc(iCodeFileAlloc, sizeof(WCHAR));

			memcpy_s(this->Items.Array[iSlot].sCodeFile, iCodeFileAlloc, sTempFile, iCodeFileAlloc);

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
			swprintf_s(this->sDebugText, sizeof(this->sDebugText),
				_T("Alloc %d bytes. (0x%X) @ %s:%d\n"),
				iSize * iCount, sMemory,
				this->Items.Array[iSlot].sCodeFile,
				this->Items.Array[iSlot].iLineOfCode);
			this->DbgWrite(this->sDebugText);
#endif
#endif

			this->Items.iUsedCount++;

			this->Unlock();
			return sMemory;
		}
#else
		void* MemoryPool::Allocate(const size_t iSize, const size_t iCount)
		{
#if _MEMPOOL_SEQUENTIAL_WHEN_OPTIMIZED != 0
			this->Lock();
			WCHAR* sMemory = (WCHAR*)calloc(iCount, iSize);
			this->Unlock();

			return sMemory;
#else
			return (WCHAR*)calloc(iCount, iSize);
#endif
		}
#endif

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
		BOOL WINAPI MemoryPool_ConsoleHandler(DWORD dwCtrlType)
		{
			if (dwCtrlType == CTRL_CLOSE_EVENT)
			{
				return true;
			}

			return FALSE;
		}
#endif

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
		bool MemoryPool::OpenConsole(void)
		{
			if (this->hStdOut == NULL)
			{
				if (AllocConsole())
				{
					SetConsoleCtrlHandler(MemoryPool_ConsoleHandler, TRUE);
#ifdef UNICODE
					SetConsoleTitle(L"MemoryPool");
#else
					SetConsoleTitle("MemoryPool");
#endif
				}

				if ((this->hStdOut = GetStdHandle(STD_OUTPUT_HANDLE)) != NULL)
				{
					return true;
				}
			}
			return false;
		}
#endif

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
		bool MemoryPool::CloseConsole(void)
		{
			if (this->hStdOut)
			{
				if (FreeConsole())
				{
					CloseHandle(this->hStdOut);
					this->hStdOut = NULL;
					return true;
				}
			}
			return false;
		}
#endif

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		size_t MemoryPool::GetFileName(const WCHAR* sInPath, WCHAR* sOutFile)
		{
			size_t iWPos = 0;

			for (size_t iPos = (int)wcslen(sInPath) - 1;
				iPos >= 0 && sInPath[iPos] != '\\' && sInPath[iPos] != '/'; iPos--)
			{
				sOutFile[iWPos++] = sInPath[iPos];
			}

			sOutFile[iWPos] = '\0';

			WCHAR* String1 = NULL;
			WCHAR* String2 = NULL;

			for (String1 = sOutFile, String2 = sOutFile + iWPos - 1;
				String2 > String1;
				++String1, --String2)
			{
				*String1 ^= *String2;
				*String2 ^= *String1;
				*String1 ^= *String2;
			}

			return iWPos;
		}
#endif

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
		void MemoryPool::DbgWrite(const size_t iValue)
		{
			WCHAR sVal[64];
			swprintf_s(sVal, _countof(sVal), L"%zu", iValue);
			this->DbgWrite(sVal);
		}
#endif

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		void MemoryPool::DbgWrite(const WCHAR* sBuffer)
		{
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
			if (this->hStdOut)
			{
				DWORD dwWritten = 0;
				WriteFile(this->hStdOut, sBuffer, (int)wcslen(sBuffer), &dwWritten, 0);
			}
#endif
		}
#endif

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		void MemoryPool::MemoryPoolAssertEx(const WCHAR* sExpression, const WCHAR* sMsg,
			const WCHAR* sFunction, const WCHAR* sFile,
			const WCHAR* sDate, const WCHAR* sTime, size_t iLine)
		{
			WCHAR sAssert[2048];
			swprintf_s(sAssert, sizeof(sAssert),
				_T("%s\r\n\r\n")
				_T("Procedure: %s\r\n")
				_T("Source File: %s\r\n")
				_T("Build Date/Time: %s (%s)\r\n")
				_T("Expression: \"%s\"\r\n")
				_T("Line: %zd\r\n\r\n"),
				sMsg, sFunction, sFile, sDate, sTime, sExpression, iLine);
			this->DbgWrite(sAssert);

			MessageBox(NULL, sAssert, _T("MemoryPool Exception"), MB_ICONSTOP | MB_SYSTEMMODAL);

			if (IsDebuggerPresent())
			{
				__debugbreak();
			}
		}
#endif

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
		void* MemoryPool::UnTrackMemory(void* lpMemory)
		{
			this->Lock();

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
			swprintf_s(this->sDebugText, sizeof(this->sDebugText), _T("Untracking address 0x%X\n"), lpMemory);
			this->DbgWrite(this->sDebugText);
#endif

			size_t iSlot = 0;

			if (lpMemory == NULL)
			{
				MemoryPoolAssert(_T("Free"), _T("Application attempted to untrack a NULL pointer."));

				this->Unlock();
				return lpMemory;
			}

			if ((iSlot = this->GetSlotByAddress((size_t)lpMemory)) != (size_t)-1)
			{
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
				free(this->Items.Array[iSlot].sCodeFile);
				this->Items.iReservedBytes -= this->Items.Array[iSlot].iSize;
				this->Items.Array[iSlot].sCodeFile = NULL;
				this->Items.Array[iSlot].iLineOfCode = 0;
				this->Items.Array[iSlot].iSize = 0;
#endif

				this->Items.Array[iSlot].iAddress = 0;

				this->Items.iUsedCount--;
			}
			else {
				swprintf_s(this->sDebugText, sizeof(this->sDebugText),
					_T("Address [0x%zX] not found in the managed memory pool."), (size_t)lpMemory);
				MemoryPoolAssert("Free", this->sDebugText);

				this->Unlock();
				return lpMemory;
			}

			this->Unlock();
			return lpMemory;
		}
#else
		void* MemoryPool::UnTrackMemory(void* lpMemory)
		{
			return lpMemory;
		}
#endif

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
		void* MemoryPool::_dbg_TrackMemory(void* lpMemory, const WCHAR* sCodeFile, const size_t iLineOfCode)
#else
		void* MemoryPool::TrackMemory(void* lpMemory)
#endif
		{
			this->Lock();

			size_t iSlot = 0;

			if ((iSlot = this->FindFreeAllocSlot()) == -1)
			{
				swprintf_s(this->sDebugText, sizeof(this->sDebugText),
					_T("Failed to find qualified memory slot for tracked memory."));
				MemoryPoolAssert(_T("Allocate"), this->sDebugText);

				this->Unlock();
				return NULL;
			}

			this->Items.Array[iSlot].iAddress = (size_t)lpMemory;

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
			this->Items.Array[iSlot].iSize = 0; //We do not know how large this allocation is.
			this->Items.Array[iSlot].iLineOfCode = iLineOfCode;
			//this->Items.iReservedBytes += this->Items.Array[iSlot].iSize;

			size_t iCodeFileAlloc = this->GetFileName(sCodeFile, sTempFile) + 1;
			this->Items.Array[iSlot].sCodeFile = (WCHAR*)calloc(iCodeFileAlloc, sizeof(WCHAR));

			memcpy_s(this->Items.Array[iSlot].sCodeFile, iCodeFileAlloc, sTempFile, iCodeFileAlloc);

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
			swprintf_s(this->sDebugText, sizeof(this->sDebugText),
				_T("Alloc unknow bytes. (0x%X) @ %s:%d\n"),
				lpMemory,
				this->Items.Array[iSlot].sCodeFile,
				this->Items.Array[iSlot].iLineOfCode);
			this->DbgWrite(this->sDebugText);
#endif
#endif

			this->Items.iUsedCount++;

			this->Unlock();

			return lpMemory;
		}
#else
		void* MemoryPool::TrackMemory(void* lpMemory)
		{
			return lpMemory;
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Memory
} //namespace::NSWFL
#endif
