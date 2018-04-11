///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_SHA1_H_
#define _NSWFL_SHA1_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Hashing {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//This class implements the Secure Hashing Standard as defined
		//	in FIPS PUB 180-1 published April 17, 1995.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool SimpleSHA1(const char *sStr, char *sOut, int iMaxSz);
		bool SimpleSHA1(const char *sStr, int iStrSz, char *sOut, int iMaxSz);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class SHA1
		{
		public:
			SHA1();
			virtual ~SHA1();

			//Re-initialize the class
			void Reset();

			//Returns the message digest
			bool Result(unsigned long *message_digest_array);

			//Provide input to SHA1
			void Input(const unsigned char	*message_array, unsigned length);
			void Input(const char *message_array, unsigned length);
			void Input(unsigned char message_element);
			void Input(char message_element);
			bool ToString(unsigned long *MessageDigest, char *sOut, int iMaxSz);
			bool ToString(char *sOut, int iMaxSz);

		private:

			//Process the next 512 bits of the message
			void ProcessMessageBlock();

			//Pads the current message block to 512 bits
			void PadMessage();

			//Performs a circular left shift operation
			inline unsigned CircularShift(int bits, unsigned word);

			unsigned H[5];						// Message digest buffers

			unsigned Length_Low;				// Message length in bits
			unsigned Length_High;				// Message length in bits

			unsigned char Message_Block[64];	// 512-bit message blocks
			int Message_Block_Index;			// Index into message block array

			bool Computed;						// Is the digest computed?
			bool Corrupted;						// Is the message digest corruped?
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Hashing
} //namespace::NSWFL
#endif
