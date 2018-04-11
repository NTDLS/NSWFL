///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_Stack_H_
#define _NSWFL_Stack_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Collections {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		enum StackBehavior {
			BEHAVIOR_FIFO = 0, //First-In First-Out
			BEHAVIOR_FILO = 1  //First-In Last-Out
		};

		enum StackDataType {
			ITEM_TYPE_BINARY = 0,
			ITEM_TYPE_UINT = 1,
			ITEM_TYPE_INT = 2,
			ITEM_TYPE_FLOAT = 3,
			ITEM_TYPE_DOUBLE = 4,
			ITEM_TYPE_STRING = 5,
			ITEM_TYPE_WSTRING = 6,
			ITEM_TYPE_INT64 = 7,
			ITEM_TYPE_UINT64 = 8
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		typedef struct _tag_Stack_Item {
			void *Value;
			size_t Size;
			StackDataType Type;
		} _STACKITEM, *_LPSTACKITEM;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		typedef struct _tag_Stack {
			_STACKITEM *Item;
			size_t Used;
			size_t Ceiling;
			size_t AllocationIncrement;
		} _STACK, *_LPSTACK;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class Stack {

		public:
			Stack();
			Stack(StackBehavior iBehavior);
			Stack(Stack *pStackToCopy);
			~Stack();

			StackBehavior Behavior(void);
			StackBehavior Behavior(StackBehavior iBehavior);

			void Clear(void);

			bool Push(unsigned __int64 u64Value);
			bool Push(__int64 i64Value);
			bool Push(unsigned int uValue);
			bool Push(int iValue);
			bool Push(float fValue);
			bool Push(double dValue);
			bool Push(const char *sValue, size_t uLength);
			bool Push(const char *sValue);
			bool Push(const WCHAR *wcValue, size_t uLength);
			bool Push(const WCHAR *wcValue);
			bool Push(const void *pValue, size_t uLength, StackDataType uType);
			bool Push(const void *pValue, size_t uLength);

			bool Pop(unsigned __int64 *u64Value);
			bool Pop(__int64 *i64Value);
			bool Pop(unsigned int *uValue);
			bool Pop(int *iValue);
			bool Pop(float *fValue);
			bool Pop(double *dValue);
			bool Pop(char *sValue, size_t *uLength);
			bool Pop(WCHAR *wcValue, size_t *uLength);
			bool Pop(void *pValue);
			bool Pop(void *pValue, size_t *uLength);
			bool Pop(void *pValue, size_t *uLength, StackDataType *uType);

			bool Pop(void);

			_STACKITEM *Peek(void);
			bool Peek(unsigned __int64 *u64Value);
			bool Peek(__int64 *i64Value);
			bool Peek(unsigned int *uValue);
			bool Peek(int *iValue);
			bool Peek(float *fValue);
			bool Peek(double *dValue);
			bool Peek(char *sValue, size_t *uLength);
			bool Peek(WCHAR *wcValue, size_t *uLength);
			bool Peek(void *pValue);
			bool Peek(void *pValue, size_t *uLength);
			bool Peek(void *pValue, size_t *uLength, StackDataType *uType);

			_STACKITEM *Peek(size_t uIndex);
			bool Peek(size_t uIndex, unsigned __int64 *u64Value);
			bool Peek(size_t uIndex, __int64 *i64Value);
			bool Peek(size_t uIndex, unsigned int *uValue);
			bool Peek(size_t uIndex, int *iValue);
			bool Peek(size_t uIndex, float *fValue);
			bool Peek(size_t uIndex, double *dValue);
			bool Peek(size_t uIndex, char *sValue, size_t *uLength);
			bool Peek(size_t uIndex, WCHAR *wcValue, size_t *uLength);
			bool Peek(size_t uIndex, void *pValue);
			bool Peek(size_t uIndex, void *pValue, size_t *uLength);
			bool Peek(size_t uIndex, void *pValue, size_t *uLength, StackDataType *uType);

			size_t StackSize(void);
			size_t Allocated(void);

			void Lock(void);
			void UnLock(void);

		protected:
			StackBehavior ciBehavior;
			_STACK Collection;

#ifndef _STACK_SINGLE_THREADED_ACCESS
			CRITICAL_SECTION CS;
#endif
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Collections
} //namespace::NSWFL
#endif
