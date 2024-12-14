///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_CRC32_CPP_
#define _NSWFL_CRC32_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Hashing {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function initializes "CRC Lookup Table". You only need to call it once to
				initialize the table before using any of the other CRC32 calculation functions.
		*/
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CRC32::CRC32(void)
		{
			this->Initialize();
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CRC32::~CRC32(void)
		{
			//No destructor code.
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function initializes "CRC Lookup Table". You only need to call it once to
				initialize the table before using any of the other CRC32 calculation functions.
		*/

		void CRC32::Initialize(void)
		{
			//0x04C11DB7 is the official polynomial used by PKZip, WinZip and Ethernet.
			unsigned int iPolynomial = 0x04C11DB7;

			memset(&this->iTable, 0, sizeof(this->iTable));

			// 256 values representing ASCII character codes.
			for (int iCodes = 0; iCodes <= 0xFF; iCodes++)
			{
				this->iTable[iCodes] = this->Reflect(iCodes, 8) << 24;

				for (int iPos = 0; iPos < 8; iPos++)
				{
					this->iTable[iCodes] = (this->iTable[iCodes] << 1)
						^ ((this->iTable[iCodes] & (1 << 31)) ? iPolynomial : 0);
				}

				this->iTable[iCodes] = this->Reflect(this->iTable[iCodes], 32);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Reflection is a requirement for the official CRC-32 standard.
			You can create CRCs without it, but they won't conform to the standard.
		*/

		unsigned int CRC32::Reflect(unsigned int iReflect, const char cChar)
		{
			unsigned int iValue = 0;

			// Swap bit 0 for bit 7, bit 1 For bit 6, etc....
			for (int iPos = 1; iPos < (cChar + 1); iPos++)
			{
				if (iReflect & 1)
				{
					iValue |= (1 << (cChar - iPos));
				}
				iReflect >>= 1;
			}

			return iValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Calculates the CRC32 by looping through each of the bytes in sData.

			Note: For Example usage example, see FileCRC().
		*/

		void CRC32::PartialCRC(unsigned int* iCRC, const unsigned char* sData, size_t iDataLength)
		{
			while (iDataLength--)
			{
				//If your compiler complains about the following line, try changing
				//	each occurrence of *iCRC with ((unsigned int)*iCRC).

				*iCRC = (*iCRC >> 8) ^ this->iTable[(*iCRC & 0xFF) ^ *sData++];
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the calculated CRC32 (through iOutCRC) for the given string.
		*/

		void CRC32::FullCRC(const unsigned char* sData, size_t iDataLength, unsigned int* iOutCRC)
		{
			((unsigned int)*iOutCRC) = 0xffffffff; //Initilaize the CRC.

			this->PartialCRC(iOutCRC, sData, iDataLength);

			((unsigned int)*iOutCRC) ^= 0xffffffff; //Finalize the CRC.
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the calculated CRC23 for the given string.
		*/

		unsigned int CRC32::FullCRC(const unsigned char* sData, size_t iDataLength)
		{
			unsigned int iCRC = 0xffffffff; //Initilaize the CRC.

			this->PartialCRC(&iCRC, sData, iDataLength);

			return(iCRC ^ 0xffffffff); //Finalize the CRC and return.
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Calculates the CRC32 of a file using the a user defined buffer.

			Note: The buffer size DOES NOT affect the resulting CRC,
					it has been provided for performance purposes only.
		*/

		bool CRC32::FileCRC(const char* sFileName, unsigned int* iOutCRC, size_t iBufferSize)
		{
			((unsigned int)*iOutCRC) = 0xffffffff; //Initilaize the CRC.

			FILE* fSource = NULL;
			unsigned char* sBuf = NULL;
			size_t iBytesRead = 0;

			if ((fopen_s(&fSource, sFileName, "rb")) != 0)
			{
				return false; //Failed to open file for read access.
			}

#ifdef _USE_GLOBAL_MEMPOOL
			if (!(sBuf = (unsigned char*)pMem->Allocate(iBufferSize, 1))) //Allocate memory for file buffering.
#else
			if (!(sBuf = (unsigned char*)malloc(iBufferSize))) //Allocate memory for file buffering.
#endif
			{
				fclose(fSource);
				return false; //Out of memory.
			}

			while ((iBytesRead = fread(sBuf, sizeof(char), iBufferSize, fSource)))
			{
				this->PartialCRC(iOutCRC, sBuf, iBytesRead);
			}

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sBuf);
#else
			free(sBuf);
#endif

			fclose(fSource);

			((unsigned int)*iOutCRC) ^= 0xffffffff; //Finalize the CRC.

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Calculates the CRC32 of a file using the a default buffer size of 1MB.
		*/

		unsigned int CRC32::FileCRC(const char* sFileName)
		{
			unsigned int iCRC;
			if (this->FileCRC(sFileName, &iCRC, 1048576))
			{
				return iCRC;
			}
			else return 0xffffffff; //While we return this as an error code, it is infact a valid CRC!
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Calculates the CRC32 of a file using the a default buffer size of 1MB.

			Note: The buffer size DOES NOT affect the resulting CRC,
					it has been provided for performance purposes only.
		*/

		unsigned int CRC32::FileCRC(const char* sFileName, size_t iBufferSize)
		{
			unsigned int iCRC;
			if (this->FileCRC(sFileName, &iCRC, iBufferSize))
			{
				return iCRC;
			}
			else return 0xffffffff; //While we return this as an error code, it is infact a valid CRC!
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Calculates the CRC32 of a file using the a default buffer size of 1MB.
		*/

		bool CRC32::FileCRC(const char* sFileName, unsigned int* iOutCRC)
		{
			return this->FileCRC(sFileName, iOutCRC, 1048576);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Hashing
} //namespace::NSWFL
#endif
