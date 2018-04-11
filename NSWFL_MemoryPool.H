///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
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
				Disables all emmory pool debugging. Fully optimized.

			_MEMPOOL_DEBUG_LEVEL = BASIC
				Enables the memory pool to track memory allocations.

			_MEMPOOL_DEBUG_LEVEL = ADVANCED
				The memory pool will not only track what was allocated, but the number
					of bytes and the location in code (file & line) that reserved the memory.

			_MEMPOOL_DEBUG_LEVEL = VERBOSE
				Not only does the memory pool keep track of memory bytes and the location in code
					(file & line) that alloacted the them, but it also opens an independent console
					to which it writes verbose information. This option is very slow.
		*/

		// Disable "Sequential Memory Access when running Optimized" for best performance.
		//#define _MEMPOOL_SEQUENTIAL_WHEN_OPTIMIZED 1

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
		#define StrDupn(sString, iLength) _dbg_StrDupn(sString, iLength, __FILE__, __LINE__)
		#define StrDup(sString) _dbg_StrDup(sString, __FILE__, __LINE__)

		#define StrDupnSafe(sOutBuffer, sString, iLength) _dbg_StrDupnSafe(sOutBuffer, sString, iLength, __FILE__, __LINE__)
		#define StrDupSafe(sOutBuffer, sString) _dbg_StrDupSafe(sOutBuffer, sString, __FILE__, __LINE__)

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
				char *sCodeFile;
				size_t iLineOfCode;
		#endif
			} MEMITEM, *LPMEMITEM;

			typedef struct _TAG_MEM_ITEMS {
				MEMITEM *Array;

				size_t iAllocated;
				size_t iAllocIncrement;
				size_t iUsedCount; //Number of used memory items.
		#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
				size_t iReservedBytes;
		#endif
			} MEMITEMS, *LPMEMITEMS;

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

			bool Free(void *lpMemory);
			void *UnTrackMemory(void *lpMemory);
			bool Destroy(void);
		#else
			bool Free(void *lpMemory);
			void *UnTrackMemory(void *lpMemory);
			bool Destroy(void);
		#endif

			bool Initialize(void);
			bool IsDebug(void);

		#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_ADVANCED
			char sTempFile[MAX_PATH];
			size_t ReservedBytes(void);

			char *_dbg_StrDupn(const char *sString, size_t iLength, const char *sCodeFile, const size_t iLineOfCode);
			char *_dbg_StrDup(const char *sString, const char *sCodeFile, const size_t iLineOfCode);

			char *_dbg_StrDupnSafe(char *&sOutBuffer, const char *sString, size_t iLength, const char *sCodeFile, const size_t iLineOfCode);
			char *_dbg_StrDupSafe(char *&sOutBuffer, const char *sString, const char *sCodeFile, const size_t iLineOfCode);

			void *_dbg_Allocate(const size_t iSize, const size_t iCount,
				const char *sCodeFile, const size_t iLineOfCode);
			void *_dbg_ReAllocate(void *lpMemory, const size_t iSize, const size_t iCount,
				const char *sCodeFile, const size_t iLineOfCode);
			void *_dbg_TrackMemory(void *lpMemory, const char *sCodeFile, const size_t iLineOfCode);
		#else
			char *StrDupn(const char *sString, size_t iLength);
			char *StrDup(const char *sString);
			char *StrDupnSafe(char *&sOutBuffer, const char *sString, size_t iLength);
			char *StrDupSafe(char *&sOutBuffer, const char *sString);
			void *Allocate(const size_t iSize, const size_t iCount);
			void *ReAllocate(void *lpMemory, const size_t iSize, const size_t iCount);
			void *TrackMemory(void *lpMemory);
		#endif

			void Lock(void);
			void Unlock(void);
			MEMITEMS Items;

		private:
		#if _MEMPOOL_DEBUG_LEVEL >= _MEMPOOL_DEBUG_BASIC

			void MemoryPoolAssertEx(const char *sExpression, const char *sMsg,
				const char *sFunction, const char *sFile,
				const char *sDate, const char *sTime, size_t iLine);

			char sDebugText[2048];

			size_t FindFreeAllocSlot(void);
			size_t GetSlotByAddress(const size_t iAddress);
			size_t GetFileName(const char *sInPath, char *sOutFile);
			void DbgWrite(const char *sBuffer);
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
#endif
