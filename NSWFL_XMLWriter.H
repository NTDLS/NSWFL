///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_XMLWriter_H_
#define _NSWFL_XMLWriter_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
using namespace NSWFL::Memory;
extern MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

namespace NSWFL {
	namespace XML {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class CXMLWriter {

		public:
			void *pUserData; //Use it for whatever you want!

			typedef struct _tag_XMLWriter_State {
				StringBuilder *pBuilder;
				char sName[255];
			} XMLWriterState, *LPXMLWriterState;

			CXMLWriter(const char *sChildName);
			CXMLWriter(void);
			~CXMLWriter(void);

			bool Initialize(void);
			bool Initialize(const char *sChildName);
			bool ChangeName(const char *sChildName);
			int GetName(char *sOutName, int iMaxOut);
			void Destroy(void);

			StringBuilder *GetBuilder(void);

			CXMLReader *ToReader(CXMLReader *lpXMLReader);

			bool Add(const char *sAttributeName, const char *sValue);
			bool Add(const char *sAttributeName, const char *sValue, const int iValueSz);
			bool Add(const char *sAttributeName, const unsigned int iValue);
			bool Add(const char *sAttributeName, const DWORD dwValue);
			bool Add(const char *sAttributeName, const signed int iValue);
			bool Add(const char *sAttributeName, const double dValue);
			bool Add(const char *sAttributeName, const double dValue, const int iDecimalPlaces);
			bool Add(const char *sAttributeName, const __int64 i64Value);
			bool Add(const char *sAttributeName, const unsigned __int64 i64Value);
			bool AddBool(const char *sAttributeName, const bool bValue);

			bool Add(const char *sAttributeName, CXMLWriter *XML);
			bool Add(const char *sAttributeName, CXMLReader *XML);

			bool Add(CXMLReader *XML);
			bool Add(CXMLWriter *XML);

			int Length(void);

			int Text(char *sOutXML, int iMaxOutSz);
			void Text(StringBuilder *textOut);
			char *Text(void);
			char *Text(int *iOutSz);
			void FreePointer(void *pPointer);

#ifdef _CXML_USE_COMPRESSION
			char *Compress(int *iOutLength);
			char *Compress(int *iOutLength, int iCompressionLevel);
#endif

			bool ToFile(const char *sFileName);

		protected:
			XMLWriterState State;

		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::XML
} //namespace::NSWFL
#endif
