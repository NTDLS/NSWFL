///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_Stack_CPP_
#define _NSWFL_Stack_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Collections {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Queue::Queue(void)
		{
			this->Destroy();
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Queue::Queue(int iAllocIncrement)
		{
			this->Initialize(iAllocIncrement);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Queue::~Queue(void)
		{
			this->Initialize(100);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Queue::Initialize(void)
		{
			return this->Initialize(100);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Queue::Initialize(int iAllocIncrement)
		{
			memset(&this->Collection, 0, sizeof(QUEUECOLLECTION));
			this->Collection.AllocIncrement = iAllocIncrement;
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Queue::Destroy(void)
		{
			if (this->Collection.Allocated > 0)
			{
				for (int iItem = 0; iItem < this->Collection.Used; iItem++)
				{
#ifdef _USE_GLOBAL_MEMPOOL
					pMem->Free(this->Collection.Data[iItem]);
#else
					free(this->Collection.Data[iItem]);
#endif
					this->Collection.DataSize[iItem] = 0;
				}

#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(this->Collection.Data);
				pMem->Free(this->Collection.DataSize);
#else
				free(this->Collection.Data);
				free(this->Collection.DataSize);
#endif
			}

			memset(&this->Collection, 0, sizeof(QUEUECOLLECTION));

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Queue::Push(const void *vInBuf, int iInBufSz)
		{
			if (this->Collection.Used == this->Collection.Allocated)
			{
				this->Collection.Allocated += this->Collection.AllocIncrement;

#ifdef _USE_GLOBAL_MEMPOOL
				if (!(this->Collection.DataSize = (int *)pMem->ReAllocate(this->Collection.DataSize, sizeof(int), this->Collection.Allocated)))
				{
					return false;
				}
				else if (!(this->Collection.Data = (char **)pMem->ReAllocate(this->Collection.Data, sizeof(char **), this->Collection.Allocated)))
				{
					return false;
				}
#else
				if (!(this->Collection.DataSize = (int *)realloc(this->Collection.DataSize, sizeof(int) * this->Collection.Allocated)))
				{
					return false;
				}
				else if (!(this->Collection.Data = (char **)realloc(this->Collection.Data, sizeof(char **) * this->Collection.Allocated)))
				{
					return false;
				}
#endif
			}

			this->Collection.DataSize[this->Collection.Used] = iInBufSz;

#ifdef _USE_GLOBAL_MEMPOOL
			this->Collection.Data[this->Collection.Used] = (char *)pMem->Allocate(iInBufSz + 1, 1);
#else
			this->Collection.Data[this->Collection.Used] = (char *)calloc(iInBufSz + 1, 1);
#endif

			memcpy(this->Collection.Data[this->Collection.Used], vInBuf, iInBufSz);

			this->Collection.Used++;

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Queue::Push(const char *sInBuf)
		{
			return this->Push(sInBuf, (int)strlen(sInBuf));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Queue::Peek(void *pOutBuf, int iMaxSz, int *iOutSz)
		{
			if (this->Collection.Used == 0)
			{
				((int)*iOutSz) = 0;
				return false;
			}

			((int)*iOutSz) = this->Collection.DataSize[0];

			memcpy_s(pOutBuf, iMaxSz, this->Collection.Data[0], this->Collection.DataSize[0]);

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void *Queue::Peek(void)
		{
			if (this->Collection.Used > 0)
			{
				return this->Collection.Data[0];
			}
			return NULL;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Queue::Peek(void *pOutBuf, int iMaxSz)
		{
			int iOutSz = 0;
			return this->Peek(pOutBuf, iMaxSz, &iOutSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Queue::Peek(char *sOutBuf, int iMaxSz)
		{
			int iOutSz = 0;
			bool bResult = this->Peek(sOutBuf, iMaxSz, &iOutSz);
			sOutBuf[iOutSz] = '\0';
			return bResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Queue::Pop(void *pOutBuf, int iMaxSz, int *iOutSz)
		{
			if (this->Peek(pOutBuf, iMaxSz, iOutSz))
			{
				return this->Pop();
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Queue::Pop(void *pOutBuf, int iMaxSz)
		{
			int iOutSz = 0;
			return this->Pop(pOutBuf, iMaxSz, &iOutSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Queue::Pop(char *sOutBuf, int iMaxSz)
		{
			int iOutSz = 0;
			bool bResult = this->Pop(sOutBuf, iMaxSz, &iOutSz);
			sOutBuf[iOutSz] = '\0';
			return bResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Removes an item from the queu.
		*/
		bool Queue::Pop(void)
		{
			if (this->Collection.Used == 0)
			{
				return false;
			}

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(this->Collection.Data[0]);
#else
			free(this->Collection.Data[0]);
#endif
			this->Collection.DataSize[0] = 0;

			for (int iCount = 0; iCount < this->Collection.Used; iCount++)
			{
				this->Collection.Data[iCount] = this->Collection.Data[iCount + 1];
				this->Collection.DataSize[iCount] = this->Collection.DataSize[iCount + 1];
			}

			this->Collection.Used--;

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Collections
} //namespace::NSWFL
#endif
