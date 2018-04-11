///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
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
using namespace NSWFL::Memory;
extern MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Collections {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Stack::Stack(Stack *pStackToCopy)
		{
#ifndef _STACK_SINGLE_THREADED_ACCESS
			InitializeCriticalSection(&CS);
#endif

			this->Lock();
			pStackToCopy->Lock();

			this->Collection.AllocationIncrement = pStackToCopy->Collection.AllocationIncrement;
			this->Collection.Ceiling = pStackToCopy->Collection.Ceiling;
			this->Collection.Item = NULL;
			this->Collection.Used = pStackToCopy->Collection.Used;
			this->ciBehavior = pStackToCopy->ciBehavior;

#ifdef _USE_GLOBAL_MEMPOOL
			if (!(this->Collection.Item = (_STACKITEM *)pMem->Allocate(this->Collection.Ceiling, sizeof(_STACKITEM))))
#else
			if (!(this->Collection.Item = (_STACKITEM *)calloc(this->Collection.Ceiling, sizeof(_STACKITEM))))
#endif
			{
				memset(&this->Collection, 0, sizeof(this->Collection));
				this->UnLock();
				return;
			}

			size_t uAllocate;
			_STACKITEM *pItem;
			for (unsigned uItem = 0; uItem < pStackToCopy->Collection.Used; uItem++)
			{
				pItem = &pStackToCopy->Collection.Item[uItem];
				uAllocate = pItem->Size;

				if (pItem->Type == ITEM_TYPE_STRING)
				{
					uAllocate += 1; //MBS NULL terminator.
				}
				else if (pItem->Type == ITEM_TYPE_WSTRING)
				{
					uAllocate += 2; //WCS NULL terminator.
				}

#ifdef _USE_GLOBAL_MEMPOOL
				this->Collection.Item[uItem].Value = pMem->Allocate(uAllocate, sizeof(byte));
#else
				this->Collection.Item[uItem].Value = calloc(uAllocate, sizeof(byte));
#endif

				this->Collection.Item[uItem].Size = pItem->Size;
				this->Collection.Item[uItem].Type = pItem->Type;

				memcpy_s(this->Collection.Item[uItem].Value, uAllocate, pItem->Value, pItem->Size);

				if (pItem->Type == ITEM_TYPE_STRING)
				{
					((unsigned char *)this->Collection.Item[uItem].Value)[pItem->Size] = '\0';
				}
				else if (pItem->Type == ITEM_TYPE_WSTRING)
				{
					((WCHAR *)this->Collection.Item[uItem].Value)[pItem->Size] = '\0';
				}
			}

			pStackToCopy->UnLock();
			this->UnLock();
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Stack::Stack(StackBehavior iBehavior)
		{
#ifndef _STACK_SINGLE_THREADED_ACCESS
			InitializeCriticalSection(&CS);
#endif

			this->Lock();
			memset(&this->Collection, 0, sizeof(this->Collection));
			this->Collection.AllocationIncrement = 10;
			this->ciBehavior = iBehavior;
			this->UnLock();
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Stack::Stack()
		{
#ifndef _STACK_SINGLE_THREADED_ACCESS
			InitializeCriticalSection(&CS);
#endif

			this->Lock();
			memset(&this->Collection, 0, sizeof(this->Collection));
			this->Collection.AllocationIncrement = 10;
			this->ciBehavior = BEHAVIOR_FIFO;
			this->UnLock();
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Stack::~Stack()
		{
			this->Clear();
#ifndef _STACK_SINGLE_THREADED_ACCESS
			DeleteCriticalSection(&CS);
#endif
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		StackBehavior Stack::Behavior(void)
		{
			return this->ciBehavior;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		StackBehavior Stack::Behavior(StackBehavior iBehavior)
		{
			if (iBehavior == BEHAVIOR_FIFO || iBehavior == BEHAVIOR_FILO)
			{
				this->ciBehavior = iBehavior;
			}
			return this->ciBehavior;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		size_t Stack::Allocated(void)
		{
			return this->Collection.Ceiling;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		size_t Stack::StackSize(void)
		{
			return this->Collection.Used;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Stack::Lock(void)
		{
#ifndef _STACK_SINGLE_THREADED_ACCESS
			EnterCriticalSection(&CS);
#endif
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Stack::UnLock(void)
		{
#ifndef _STACK_SINGLE_THREADED_ACCESS
			LeaveCriticalSection(&CS);
#endif
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Stack::Clear(void)
		{
			this->Lock();
			if (this->Collection.Item)
			{
				for (unsigned int iIndex = 0; iIndex < this->Collection.Used; iIndex++)
				{
#ifdef _USE_GLOBAL_MEMPOOL
					pMem->Free(this->Collection.Item[iIndex].Value);
#else
					free(this->Collection.Item[iIndex].Value);
#endif
				}
#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(this->Collection.Item);
#else
				free(this->Collection.Item);
#endif
			}

			memset(&this->Collection, 0, sizeof(this->Collection));

			this->UnLock();
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Push(unsigned __int64 u64Value)
		{
			return this->Push((void *)&u64Value, sizeof(u64Value), ITEM_TYPE_UINT64);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Push(__int64 i64Value)
		{
			return this->Push((void *)&i64Value, sizeof(i64Value), ITEM_TYPE_INT64);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Push(unsigned int uValue)
		{
			return this->Push((void *)&uValue, sizeof(uValue), ITEM_TYPE_UINT);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Push(int iValue)
		{
			return this->Push((void *)&iValue, sizeof(iValue), ITEM_TYPE_INT);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Push(float fValue)
		{
			return this->Push((void *)&fValue, sizeof(fValue), ITEM_TYPE_FLOAT);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Push(double dValue)
		{
			return this->Push((void *)&dValue, sizeof(dValue), ITEM_TYPE_DOUBLE);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Push(const char *sValue)
		{
			return this->Push((void *)sValue, strlen(sValue), ITEM_TYPE_STRING);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Push(const char *sValue, size_t uLength)
		{
			return this->Push((void *)sValue, uLength, ITEM_TYPE_STRING);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Push(const WCHAR *wcValue)
		{
			return this->Push((void *)wcValue, wcslen(wcValue), ITEM_TYPE_WSTRING);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Push(const WCHAR *wcValue, size_t uLength)
		{
			return this->Push((void *)wcValue, uLength, ITEM_TYPE_WSTRING);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Push(const void *pValue, size_t uLength)
		{
			return this->Push((void *)pValue, uLength, ITEM_TYPE_BINARY);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Push(const void *pValue, size_t uLength, StackDataType uType)
		{
			this->Lock();
			if (this->Collection.Used == this->Collection.Ceiling)
			{
				this->Collection.AllocationIncrement += (this->Collection.AllocationIncrement / 2);
				this->Collection.Ceiling += this->Collection.AllocationIncrement;

#ifdef _USE_GLOBAL_MEMPOOL
				if (!(this->Collection.Item = (_STACKITEM *)pMem->ReAllocate(this->Collection.Item, this->Collection.Ceiling, sizeof(_STACKITEM))))
#else
				if (!(this->Collection.Item = (_STACKITEM *)realloc(this->Collection.Item, this->Collection.Ceiling * sizeof(_STACKITEM))))
#endif
				{
					this->UnLock();
					return false;
				}
			}

			size_t uAllocate = uLength;

			if (uType == ITEM_TYPE_STRING)
			{
				uAllocate += 1; //MBS NULL terminator.
			}
			else if (uType == ITEM_TYPE_WSTRING)
			{
				uAllocate += 2; //WCS NULL terminator.
			}

			size_t iNewItemIndex = 0;

			if (this->ciBehavior == BEHAVIOR_FIFO)
			{
				//Move all values down one position in the stack.
				for (register size_t uIndex = this->Collection.Used; uIndex > 0; uIndex--)
				{
					this->Collection.Item[uIndex] = this->Collection.Item[uIndex - 1];
				}
			}
			else {
				iNewItemIndex = this->Collection.Used;
			}

#ifdef _USE_GLOBAL_MEMPOOL
			this->Collection.Item[iNewItemIndex].Value = pMem->Allocate(uAllocate, sizeof(byte));
#else
			this->Collection.Item[iNewItemIndex].Value = calloc(uAllocate, sizeof(byte));
#endif

			this->Collection.Item[iNewItemIndex].Size = uLength;
			this->Collection.Item[iNewItemIndex].Type = uType;

			memcpy_s(this->Collection.Item[iNewItemIndex].Value, uAllocate, pValue, uLength);

			if (uType == ITEM_TYPE_STRING)
			{
				((unsigned char *)this->Collection.Item[iNewItemIndex].Value)[uLength] = '\0';
			}
			else if (uType == ITEM_TYPE_WSTRING)
			{
				((WCHAR *)this->Collection.Item[iNewItemIndex].Value)[uLength] = '\0';
			}

			this->Collection.Used++;

			this->UnLock();

			return true;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Pop(unsigned __int64 *u64Value)
		{
			return this->Pop((void *)u64Value);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Pop(__int64 *i64Value)
		{
			return this->Pop((void *)i64Value);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Pop(unsigned int *uValue)
		{
			return this->Pop((void *)uValue);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Pop(int *iValue)
		{
			return this->Pop((void *)iValue);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Pop(float *fValue)
		{
			return this->Pop((void *)fValue);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Pop(double *dValue)
		{
			return this->Pop((void *)dValue);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Pop(char *sValue, size_t *uLength)
		{
			return this->Pop((void *)sValue, uLength);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Pop(WCHAR *wcValue, size_t *uLength)
		{
			return this->Pop((void *)wcValue, uLength);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Pop(void *pValue, size_t *uLength)
		{
			StackDataType uType;
			return this->Pop(pValue, uLength, &uType);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Pop(void *pValue)
		{
			size_t uLength;
			StackDataType uType;
			return this->Pop(pValue, &uLength, &uType);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Pop(void)
		{
			this->Lock();
			if (this->Collection.Used)
			{
				_STACKITEM *pItem = &this->Collection.Item[--this->Collection.Used];
#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(pItem->Value);
#else
				free(pItem->Value);
#endif
				this->UnLock();
				return true;
			}
			this->UnLock();
			return false;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Pop(void *pValue, size_t *uLength, StackDataType *uType)
		{
			this->Lock();
			if (this->Collection.Used)
			{
				_STACKITEM *pItem = &this->Collection.Item[--this->Collection.Used];
				memcpy_s(pValue, pItem->Size, pItem->Value, pItem->Size);

				*uLength = pItem->Size;
				*uType = pItem->Type;

#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(pItem->Value);
#else
				free(pItem->Value);
#endif
				this->UnLock();
				return true;
			}

			this->UnLock();
			return false;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		_STACKITEM *Stack::Peek(void)
		{
			return &this->Collection.Item[this->Collection.Used - 1];
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(unsigned __int64 *u64Value)
		{
			return this->Peek((void *)u64Value);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(__int64 *i64Value)
		{
			return this->Peek((void *)i64Value);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(unsigned int *uValue)
		{
			return this->Peek((void *)uValue);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(int *iValue)
		{
			return this->Peek((void *)iValue);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(float *fValue)
		{
			return this->Peek((void *)fValue);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(double *dValue)
		{
			return this->Peek((void *)dValue);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(char *sValue, size_t *uLength)
		{
			return this->Peek((void *)sValue, uLength);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(WCHAR *wcValue, size_t *uLength)
		{
			return this->Peek((void *)wcValue, uLength);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(void *pValue, size_t *uLength)
		{
			StackDataType uType;
			return this->Peek(pValue, uLength, &uType);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(void *pValue)
		{
			size_t uLength;
			StackDataType uType;
			return this->Peek(pValue, &uLength, &uType);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(void *pValue, size_t *uLength, StackDataType *uType)
		{
			this->Lock();

			if (this->Collection.Used)
			{
				_STACKITEM *pItem = &this->Collection.Item[this->Collection.Used - 1];
				memcpy_s(pValue, pItem->Size, pItem->Value, pItem->Size);

				*uLength = pItem->Size;
				*uType = pItem->Type;

				this->UnLock();
				return true;
			}

			this->UnLock();
			return false;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		_STACKITEM *Stack::Peek(size_t uIndex)
		{
			if (uIndex >= this->Collection.Used || uIndex < 0)
			{
				return NULL;
			}
			return &this->Collection.Item[uIndex];
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(size_t uIndex, unsigned __int64 *u64Value)
		{
			return this->Peek(uIndex, (void *)u64Value);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(size_t uIndex, __int64 *i64Value)
		{
			return this->Peek(uIndex, (void *)i64Value);
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(size_t uIndex, unsigned int *uValue)
		{
			return this->Peek(uIndex, (void *)uValue);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(size_t uIndex, int *iValue)
		{
			return this->Peek(uIndex, (void *)iValue);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(size_t uIndex, float *fValue)
		{
			return this->Peek(uIndex, (void *)fValue);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(size_t uIndex, double *dValue)
		{
			return this->Peek(uIndex, (void *)dValue);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(size_t uIndex, char *sValue, size_t *uLength)
		{
			return this->Peek(uIndex, (void *)sValue, uLength);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(size_t uIndex, WCHAR *wcValue, size_t *uLength)
		{
			return this->Peek(uIndex, (void *)wcValue, uLength);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(size_t uIndex, void *pValue, size_t *uLength)
		{
			StackDataType uType;
			return this->Peek(uIndex, pValue, uLength, &uType);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(size_t uIndex, void *pValue)
		{
			size_t uLength;
			StackDataType uType;
			return this->Peek(uIndex, pValue, &uLength, &uType);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Stack::Peek(size_t uIndex, void *pValue, size_t *uLength, StackDataType *uType)
		{
			this->Lock();

			if (uIndex >= this->Collection.Used || uIndex < 0)
			{
				return NULL;
			}

			if (this->Collection.Used)
			{
				_STACKITEM *pItem = &this->Collection.Item[uIndex];
				memcpy_s(pValue, pItem->Size, pItem->Value, pItem->Size);

				*uLength = pItem->Size;
				*uType = pItem->Type;

				this->UnLock();
				return true;
			}

			this->UnLock();
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Collections
} //namespace::NSWFL
#endif
