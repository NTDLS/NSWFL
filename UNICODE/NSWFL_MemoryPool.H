///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_MEMORYPOOL_H_
#define _NSWFL_MEMORYPOOL_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Memory {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define _MEMPOOL_DEBUG_DISABLED	0
#define _MEMPOOL_DEBUG_BASIC	1
#define _MEMPOOL_DEBUG_ADVANCED	2
#define _MEMPOOL_DEBUG_VERBOSE	3

/*
	_MEMPOOL_DEBUG_LEVEL = DISABLED
		Disables all memory pool debugging. Fully optimized.

	_MEMPOOL_DEBUG_LEVEL = BASIC
		Enables the memory pool to track memory allocations.

	_MEMPOOL_DEBUG_LEVEL = ADVANCED
		The memory pool will not only track what was allocated, but the number
			of bytes and the location in code (file & line) that reserved the memory.

	_MEMPOOL_DEBUG_LEVEL = VERBOSE
		Not only does the memory pool keep track of memory bytes and the location in code
			(file & line) that allocated the them, but it also opens an independent console
			to which it writes verbose information. This option is very slow.
*/

// Disable "Sequential Memory Access when running Optimized" for best performance.
//#define _MEMPOOL_SEQUENTIAL_WHEN_OPTIMIZED 1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
#define CloneStringN(sString, iLength) _dbg_CloneStringN(sString, iLength, __FILE__, __LINE__)
#define CloneString(sString) _dbg_CloneString(sString, __FILE__, __LINE__)

#define CloneStringNSafe(sOutBuffer, sString, iLength) _dbg_CloneStringNSafe(sOutBuffer, sString, iLength, __FILE__, __LINE__)
#define CloneStringSafe(sOutBuffer, sString) _dbg_CloneStringSafe(sOutBuffer, sString, __FILE__, __LINE__)

#define Allocate(iSize, iCount) _dbg_Allocate(iSize, iCount, __FILE__, __LINE__)
#define ReAllocate(lpMemory, iSize, iCount) _dbg_ReAllocate(lpMemory, iSize, iCount, __FILE__, __LINE__)
#define TrackMemory(lpMemory) _dbg_TrackMemory(lpMemory, __FILE__, __LINE__)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class MemoryPool {
			typedef struct _TAG_MEM_ITEM {
				size_t iAddress;
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
				size_t iSize;
				WCHAR* sCodeFile;
				size_t iLineOfCode;
#endif
			} MEMITEM, * LPMEMITEM;

			typedef struct _TAG_MEM_ITEMS {
				MEMITEM* Array;

				size_t iAllocated;
				size_t iAllocIncrement;
				size_t iUsedCount; //Number of used memory items.
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
				size_t iReservedBytes;
#endif
			} MEMITEMS, * LPMEMITEMS;

		public:
			~MemoryPool(void);
			MemoryPool(void);

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC
			MemoryPool(size_t iAllocateIncrement);

			bool FreeAll(void);
			size_t GetMemAllocationCount(void);
			size_t GetMemFreeCount(void);
			size_t GetSlotAllocationCount(void);

			bool Initialize(size_t iAllocateIncrement);

			bool Free(void* lpMemory);
			void* UnTrackMemory(void* lpMemory);
			bool Destroy(void);
#else
			bool Free(void* lpMemory);
			void* UnTrackMemory(void* lpMemory);
			bool Destroy(void);
#endif

			bool Initialize(void);
			bool IsDebug(void);

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
			WCHAR sTempFile[MAX_PATH];
			size_t ReservedBytes(void);

			WCHAR* _dbg_CloneStringN(const WCHAR* sString, size_t iLength, const WCHAR* sCodeFile, const size_t iLineOfCode);
			WCHAR* _dbg_CloneString(const WCHAR* sString, const WCHAR* sCodeFile, const size_t iLineOfCode);

			WCHAR* _dbg_CloneStringNSafe(WCHAR*& sOutBuffer, const WCHAR* sString, size_t iLength, const WCHAR* sCodeFile, const size_t iLineOfCode);
			WCHAR* _dbg_CloneStringSafe(WCHAR*& sOutBuffer, const WCHAR* sString, const WCHAR* sCodeFile, const size_t iLineOfCode);

			void* _dbg_Allocate(const size_t iSize, const size_t iCount,
				const WCHAR* sCodeFile, const size_t iLineOfCode);
			void* _dbg_ReAllocate(void* lpMemory, const size_t iSize, const size_t iCount,
				const WCHAR* sCodeFile, const size_t iLineOfCode);
			void* _dbg_TrackMemory(void* lpMemory, const WCHAR* sCodeFile, const size_t iLineOfCode);
#else
			WCHAR* CloneStringN(const WCHAR* sString, size_t iLength);
			WCHAR* CloneString(const WCHAR* sString);
			WCHAR* CloneStringNSafe(WCHAR*& sOutBuffer, const WCHAR* sString, size_t iLength);
			WCHAR* CloneStringSafe(WCHAR*& sOutBuffer, const WCHAR* sString);
			void* Allocate(const size_t iSize, const size_t iCount);
			void* ReAllocate(void* lpMemory, const size_t iSize, const size_t iCount);
			void* TrackMemory(void* lpMemory);
#endif

			void Lock(void);
			void Unlock(void);
			MEMITEMS Items;

		private:
#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC

			void MemoryPoolAssertEx(const WCHAR* sExpression, const WCHAR* sMsg,
				const WCHAR* sFunction, const WCHAR* sFile,
				const WCHAR* sDate, const WCHAR* sTime, size_t iLine);

			WCHAR sDebugText[2048];

			size_t FindFreeAllocSlot(void);
			size_t GetSlotByAddress(const size_t iAddress);
			size_t GetFileName(const WCHAR* sInPath, WCHAR* sOutFile);
			void DbgWrite(const WCHAR* sBuffer);
			void DbgWrite(const size_t iValue);

#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_VERBOSE
			bool OpenConsole(void);
			bool CloseConsole(void);
			HANDLE hStdOut;
#endif
#endif

			CRITICAL_SECTION csMemLock;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Memory
} //namespace::NSWFL

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

#endif


