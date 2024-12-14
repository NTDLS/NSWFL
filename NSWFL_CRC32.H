///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_CRC32_H_
#define _NSWFL_CRC32_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Hashing {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class CRC32 {

		public:
			CRC32(void);
			~CRC32(void);

			void Initialize(void);

			bool FileCRC(const char* sFileName, unsigned int* iOutCRC);
			bool FileCRC(const char* sFileName, unsigned int* iOutCRC, size_t iBufferSize);
			unsigned int FileCRC(const char* sFileName);
			unsigned int FileCRC(const char* sFileName, size_t iBufferSize);

			unsigned int FullCRC(const unsigned char* sData, size_t iDataLength);
			void FullCRC(const unsigned char* sData, size_t iLength, unsigned int* iOutCRC);

			void PartialCRC(unsigned int* iCRC, const unsigned char* sData, size_t iDataLength);

		private:
			unsigned int Reflect(unsigned int iReflect, const char cChar);
			unsigned int iTable[256]; // CRC lookup table array.
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Hashing
} //namespace::NSWFL
#endif
