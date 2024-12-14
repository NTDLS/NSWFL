///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_XMLWriter_H_
#define _NSWFL_XMLWriter_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace XML {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class XMLWriter {

		public:
			void* pUserData; //Use it for whatever you want!

			typedef struct _tag_XMLWriter_State {
				String::StringBuilder* pBuilder;
				char sName[255];
			} XMLWriterState, * LPXMLWriterState;

			XMLWriter(const char* sChildName);
			XMLWriter(void);
			~XMLWriter(void);

			bool Initialize(void);
			bool Initialize(const char* sChildName);
			bool ChangeName(const char* sChildName);
			int GetName(char* sOutName, int iMaxOut);
			void Destroy(void);

			String::StringBuilder* GetBuilder(void);

			XMLReader* ToReader(XMLReader* lpXMLReader);

			bool Add(const char* sAttributeName, const char* sValue);
			bool Add(const char* sAttributeName, const char* sValue, const int iValueSz);
			bool Add(const char* sAttributeName, const unsigned int iValue);
			bool Add(const char* sAttributeName, const DWORD dwValue);
			bool Add(const char* sAttributeName, const signed int iValue);
			bool Add(const char* sAttributeName, const double dValue);
			bool Add(const char* sAttributeName, const double dValue, const int iDecimalPlaces);
			bool Add(const char* sAttributeName, const __int64 i64Value);
			bool Add(const char* sAttributeName, const unsigned __int64 i64Value);
			bool AddBool(const char* sAttributeName, const bool bValue);

			bool Add(const char* sAttributeName, XMLWriter* XML);
			bool Add(const char* sAttributeName, XMLReader* XML);

			bool Add(XMLReader* XML);
			bool Add(XMLWriter* XML);

			int Length(void);

			int Text(char* sOutXML, int iMaxOutSz);
			void Text(String::StringBuilder* textOut);
			char* Text(void);
			char* Text(int* iOutSz);
			void FreePointer(void* pPointer);

#ifdef _XML_USE_COMPRESSION
			char* Compress(int* iOutLength);
			char* Compress(int* iOutLength, int iCompressionLevel);
#endif

			bool ToFile(const char* sFileName);

		protected:
			XMLWriterState State;

		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::XML
} //namespace::NSWFL
#endif
