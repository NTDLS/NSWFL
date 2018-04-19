///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_UIntegerStack_H_
#define _NSWFL_UIntegerStack_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Collections {

		class UIntegerStack {

		public:
			bool Init(unsigned int iInitialSize);
			bool Init(void);
			bool Destroy(void);
			unsigned int GetStackSize(void);
			unsigned int GetFreeSize(void);
			unsigned int GetStackLevel(void);
			bool IsStackEmpty(void);

			bool Push(unsigned int iNumber);
			bool Pop(unsigned int *iNumber);
			unsigned int Pop(void);

			bool Peek(unsigned int *iNumber);
			bool Peek(unsigned int iSlot, unsigned int *iNumber);

		private:
			unsigned int iStackSize;
			unsigned int iInitialSize;

			unsigned int *iStack;
			unsigned int iFree;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Collections
} //namespace::NSWFL
#endif
