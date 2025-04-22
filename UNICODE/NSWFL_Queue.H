///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_Queue_H_
#define _NSWFL_Queue_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Collections {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class Queue {

			typedef struct _QUEUECOLLECTION {
				WCHAR** Data;
				int* DataSize;
				int Used;
				int Allocated;
				int AllocIncrement;
			} QUEUECOLLECTION, * LPQUEUECOLLECTION;

		public:

			Queue::Queue(void);
			Queue::Queue(int iAllocIncrement);
			Queue::~Queue(void);

			bool Initialize(int iAllocIncrement);
			bool Initialize(void);
			bool Destroy();

			bool Pop(void* pOutBuf, int iMaxSz, int* iOutSz);
			bool Pop(void* pOutBuf, int iMaxSz);
			bool Pop(WCHAR* sOutBuf, int iMaxSz);

			bool Peek(void* pOutBuf, int iMaxSz, int* iOutSz);
			bool Peek(void* pOutBuf, int iMaxSz);
			bool Peek(WCHAR* sOutBuf, int iMaxSz);
			void* Peek(void);

			bool Push(const void* vInBuf, int iInBufSz);
			bool Push(const WCHAR* sInBuf);

			bool Pop(void);

		private:
			QUEUECOLLECTION Collection;

		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Collections
} //namespace::NSWFL
#endif
