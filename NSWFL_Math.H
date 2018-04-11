///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_MATH_H_
#define _NSWFL_MATH_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Math {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(WIN64) || defined(_WIN64) || defined(_AMD64)
#define SetBit(Value, Bit) ((Value) |= 1i64 << (Bit))
#define ClearBit(Value, Bit) ((Value) &= ~(1i64 << (Bit)))
#define IsBitSet(Value, Bit) (((Value) & (1i64 << (Bit))) != 0)
#else
#define SetBit(Value, Bit) ((Value) |= 1 << (Bit))
#define ClearBit(Value, Bit) ((Value) &= ~(1 << (Bit)))
#define IsBitSet(Value, Bit) (((Value) & (1 << (Bit))) != 0)
#endif

#define SwapBytes(x,y) x ^= y ^= x ^= y

		int SquareRoot(int iInput);
		int LoByte(unsigned int iInput);
		int HiByte(unsigned int iInput);
		int LongLength(long lIn);
		int IntLength(int iIn);
		int ReverseLong(long lIn);
		int ReverseInt(int iIn);
		unsigned long VersionToLong(int iBit1, int iBit2, int iBit3, int iBit4);
		void LongToVersion(DWORD dwInVer, int *iBit1, int *iBit2, int *iBit3, int *iBit4);
		__int64 *SortIntegerArrayASC(__int64 *i64Array, unsigned int iItems);
		__int64 *SortIntegerArrayDESC(__int64 *i64Array, unsigned int iItems);
		unsigned int RangeValue(unsigned int iValue, unsigned int iAtLeast, unsigned int iAtMost);
		double RoundToNearest(double r);

		unsigned __int64 ToU64(unsigned int ulLowPart, unsigned int ulHighPart);
		__int64 ToI64(unsigned int ulLowPart, unsigned int ulHighPart);

		bool WithinPercent(double dValue, int iPercent, double dOf);

		char *ULongToBinary(DWORD_PTR iBitMask, char *sBinary, int iMaxOutSz);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Math
} //namespace::NSWFL
#endif
