///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_KEYGENERATIION_CPP_
#define _NSWFL_KEYGENERATIION_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
using namespace NSWFL::Memory;
extern MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace KeyGeneration {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned long GenerateSeed(void)
		{
			srand(GetTickCount());
			return rand();
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GenerateKey(int iNewKeyLen, unsigned long lSeed, int iAttribs, char *sOutKey)
		{
			bool ValidValue = false;

			int iWPos = 0;
			unsigned char Random = 0;
			unsigned char LastValue = 0;

			if (!(iAttribs &GKNUMBERS) && !(iAttribs &GKUPPER_AZ)
				&& !(iAttribs &GKLOWER_AZ) && !(iAttribs &GKBROADKEY))
			{
				return false;
			}

			srand(lSeed);

			while (iWPos < iNewKeyLen)
			{
				Random = (unsigned char)rand() % 256;

				if (iAttribs &GKNUMBERS)  // 0 - 9
					if ((Random < 57) && (Random > 48))
						ValidValue = true;

				if (iAttribs &GKUPPER_AZ) // A - Z
					if ((Random < 90) && (Random > 65))
						ValidValue = true;

				if (iAttribs &GKLOWER_AZ) // a - z
					if ((Random < 122) && (Random > 97))
						ValidValue = true;

				if (iAttribs &GKBROADKEY) // Broad Key
					ValidValue = true;

				if (Random == 0)        ValidValue = false; // Null
				if (Random == 10)        ValidValue = false; // lf
				if (Random == 13)        ValidValue = false; // cr

				if (iAttribs &GKNONREPETITION)
				{
					if (Random == LastValue)
					{
						ValidValue = false;
					}
				}

				if (ValidValue)
				{
					sOutKey[iWPos] = Random;
					LastValue = Random;
					iWPos++;
				}

				ValidValue = false;
			}

			sOutKey[iWPos] = '\0';

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::KeyGeneration
} //namespace::NSWFL
#endif
