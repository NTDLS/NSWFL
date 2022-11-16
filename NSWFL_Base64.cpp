///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_Conversion_CPP_
#define _NSWFL_Conversion_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#include <Math.H>

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Conversion {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		static unsigned char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define char64(c)  ((c > 127) ? 0xff : index_64[(c)])

		//This assumes that an unsigned char is exactly 8 bits.
		static unsigned char index_64[128] = {
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,   62, 0xff, 0xff, 0xff,   63,
			  52,   53,   54,   55,   56,   57,   58,   59,   60,   61, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff,    0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
			  15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
			  41,   42,   43,   44,   45,   46,   47,   48,   49,   50,   51, 0xff, 0xff, 0xff, 0xff, 0xff
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Base64Encode(const unsigned char *input, int input_length, unsigned char *output, int output_length)
		{
			int	i = 0, j = 0;
			int	pad;
			int requiredLength = (int)((ceil((double)input_length / 3.0)*4.0) + 1.0) + 10;

			if (output_length < requiredLength)
			{
				return -requiredLength;
			}

			while (i < input_length)
			{
				pad = 3 - (input_length - i);

				if (pad == 2)
				{
					output[j] = basis_64[input[i] >> 2];
					output[j + 1] = basis_64[(input[i] & 0x03) << 4];
					output[j + 2] = '=';
					output[j + 3] = '=';
				}
				else if (pad == 1) {
					output[j] = basis_64[input[i] >> 2];
					output[j + 1] = basis_64[((input[i] & 0x03) << 4) | ((input[i + 1] & 0xf0) >> 4)];
					output[j + 2] = basis_64[(input[i + 1] & 0x0f) << 2];
					output[j + 3] = '=';
				}
				else
				{
					output[j] = basis_64[input[i] >> 2];
					output[j + 1] = basis_64[((input[i] & 0x03) << 4) | ((input[i + 1] & 0xf0) >> 4)];
					output[j + 2] = basis_64[((input[i + 1] & 0x0f) << 2) | ((input[i + 2] & 0xc0) >> 6)];
					output[j + 3] = basis_64[input[i + 2] & 0x3f];
				}
				i += 3;
				j += 4;
			}
			output[j] = '\0';
			return j;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int Base64Decode(const unsigned char *input, int input_length, unsigned char *output, int output_length)
		{
			int		i = 0, j = 0, pad;
			unsigned char	c[4];

			if (!(output_length >= (input_length * 3 / 4)) || !((input_length % 4) == 0))
			{
				return -1;
			}

			while ((i + 3) < input_length)
			{
				pad = 0;
				c[0] = char64(input[i]); pad += (c[0] == 0xff);
				c[1] = char64(input[i + 1]); pad += (c[1] == 0xff);
				c[2] = char64(input[i + 2]); pad += (c[2] == 0xff);
				c[3] = char64(input[i + 3]); pad += (c[3] == 0xff);

				if (pad == 2)
				{
					output[j++] = (c[0] << 2) | ((c[1] & 0x30) >> 4);
					output[j] = (c[1] & 0x0f) << 4;
				}
				else if (pad == 1)
				{
					output[j++] = (c[0] << 2) | ((c[1] & 0x30) >> 4);
					output[j++] = ((c[1] & 0x0f) << 4) | ((c[2] & 0x3c) >> 2);
					output[j] = (c[2] & 0x03) << 6;
				}
				else
				{
					output[j++] = (c[0] << 2) | ((c[1] & 0x30) >> 4);
					output[j++] = ((c[1] & 0x0f) << 4) | ((c[2] & 0x3c) >> 2);
					output[j++] = ((c[2] & 0x03) << 6) | (c[3] & 0x3f);
				}
				i += 4;
			}
			return j;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Conversion
} //namespace::NSWFL
#endif
