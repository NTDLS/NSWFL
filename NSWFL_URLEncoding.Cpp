///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_URLEncoding_CPP_
#define _NSWFL_URLEncoding_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

namespace NSWFL {
	namespace Conversion {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		inline BYTE URLCoding_ToHex(const BYTE &x)
		{
			return x > 9 ? x + 55 : x + 48;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// First, a subroutine that substitutes any instance of cBad with
		// cGood in a string.  This is used to replace the plus sign with
		// a space character.
		inline void URLCoding_SwapChar(unsigned char *pOriginal, unsigned char cBad, unsigned char cGood)
		{
			int iPos = 0;

			// Loop through the input string (cOriginal), character by
			// character, replacing each instance of cBad with cGood

			while (pOriginal[iPos])
			{
				if (pOriginal[iPos] == cBad)
				{
					pOriginal[iPos] = cGood;
				}
				iPos++;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Now, a subroutine that unescapes escaped characters.
		inline unsigned char URLCoding_IntFromHex(unsigned char *pChars)
		{
			unsigned char Hi = 0; // holds high byte.
			unsigned char Lo = 0; // holds low byte.

			// Get the value of the first byte to Hi
			Hi = pChars[0];

			if ('0' <= Hi && Hi <= '9') {
				Hi -= '0';
			}
			else if ('a' <= Hi && Hi <= 'f') {
				Hi -= ('a' - 10);
			}
			else if ('A' <= Hi && Hi <= 'F') {
				Hi -= ('A' - 10);
			}

			// Get the value of the second byte to Lo
			Lo = pChars[1];

			if ('0' <= Lo && Lo <= '9') {
				Lo -= '0';
			}
			else if ('a' <= Lo && Lo <= 'f') {
				Lo -= ('a' - 10);
			}
			else if ('A' <= Lo && Lo <= 'F') {
				Lo -= ('A' - 10);
			}

			return (Lo + (16 * Hi));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int URLDecode(char *sBuffer)
		{
			unsigned char *uBuf = (unsigned char *)sBuffer;
			int iWPos = 0;

			// First, change those pesky plusses to spaces.
			URLCoding_SwapChar(uBuf, '+', ' ');

			// Now, loop through looking for escapes.
			for (int iRPos = 0; uBuf[iRPos];)
			{
				if (uBuf[iRPos] == '%')
				{
					// A percent sign followed by two hex digits means
					// that the digits represent an escaped character.
					// We must decode it.
					iRPos++;
					if (isxdigit(uBuf[iRPos]) && isxdigit(uBuf[iRPos + 1]))
					{
						uBuf[iWPos++] = URLCoding_IntFromHex(uBuf + iRPos);
						iRPos += 2;
					}
				}
				else {
					uBuf[iWPos++] = uBuf[iRPos++];
				}
			}

			uBuf[iWPos] = '\0';

			return iWPos;
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		inline bool URLCoding_IsSafeChar(unsigned char iChr)
		{
			return((iChr >= 'A' && iChr <= 'Z')
				|| (iChr >= 'a' && iChr <= 'z')
				|| (iChr >= '0' && iChr <= '9')
				//|| iChr == '$' || iChr == '+' || iChr == ',' || iChr == '~'
				|| iChr == '-' || iChr == '_' || iChr == '.' || iChr == '!'
				|| iChr == '*' || iChr == '\'' || iChr == '(' || iChr == ')');
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int URLEncodeSimulate(const char *sIn, int iLength)
		{
			int iWPos = 0;
			const unsigned char *uIn = (const unsigned char *)sIn;

			for (int iRPos = 0; iRPos < iLength; iRPos++)
			{
				if (URLCoding_IsSafeChar(uIn[iRPos]))
				{
					iWPos++;
				}
				else {
					iWPos += 3;
				}
			}

			return iWPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int URLEncode(const char *sIn, int iLength, char *sOut, int iMaxOut)
		{
			int iWPos = 0;
			const unsigned char *uIn = (const unsigned char *)sIn;

			for (int iRPos = 0; iRPos < iLength; iRPos++)
			{
				if (URLCoding_IsSafeChar(uIn[iRPos]))
				{
					if (iWPos >= iMaxOut) return -1;
					sOut[iWPos++] = uIn[iRPos];
				}
				else {
					/*
					if(uIn[iRPos] == ' ')
					{
						if(iWPos >= iMaxOut) return -1;
						sOut[iWPos++] = '+';
					}
					else{
					*/
					if (iWPos + 3 >= iMaxOut) return -1;
					sOut[iWPos++] = '%';
					sOut[iWPos++] = URLCoding_ToHex(uIn[iRPos] >> 4);
					sOut[iWPos++] = URLCoding_ToHex(uIn[iRPos] % 16);
					/*
					}
					*/
				}
			}

			sOut[iWPos] = '\0';

			return iWPos;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int URLEncode(const char *sIn, char *sOut, int iMaxOut)
		{
			return URLEncode(sIn, (int)strlen(sIn), sOut, iMaxOut);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Conversion
} //namespace::NSWFL
#endif
