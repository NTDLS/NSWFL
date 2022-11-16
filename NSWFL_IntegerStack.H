///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_IntegerStack_H_
#define _NSWFL_IntegerStack_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Collections {

		class IntegerStack
		{
		public:
			unsigned int GetStackSize(void);
			unsigned int GetFreeSize(void);
			unsigned int GetStackLevel(void);
			bool IsStackEmpty(void);
			bool IsStackFull(void);

			bool Push(int iNumber);
			bool Pop(int *iNumber);
			int Pop(void);

			bool Peek(int *iNumber);
			bool Peek(unsigned int iSlot, int *iNumber);

			bool Initialize(unsigned int iInitialSize, bool bAutoGrow);
			bool Initialize(unsigned int iInitialSize);
			bool Initialize(void);
			bool Destroy(void);

			IntegerStack();
			IntegerStack(unsigned int iInitialSize);
			IntegerStack(unsigned int iInitialSize, bool bAutoGrow);
			~IntegerStack();

		private:
			unsigned int iStackSize;
			unsigned int iInitialSize;

			int *iStack;
			unsigned int iFree;
			bool bAutoGrow;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Collections
} //namespace::NSWFL
#endif
