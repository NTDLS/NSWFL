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
				WCHAR sName[255];
			} XMLWriterState, * LPXMLWriterState;

			XMLWriter(const WCHAR* sChildName);
			XMLWriter(void);
			~XMLWriter(void);

			bool Initialize(void);
			bool Initialize(const WCHAR* sChildName);
			bool ChangeName(const WCHAR* sChildName);
			int GetName(WCHAR* sOutName, int iMaxOut);
			void Destroy(void);

			String::StringBuilder* GetBuilder(void);

			XMLReader* ToReader(XMLReader* lpXMLReader);

			bool Add(const WCHAR* sAttributeName, const WCHAR* sValue);
			bool Add(const WCHAR* sAttributeName, const WCHAR* sValue, const int iValueSz);
			bool Add(const WCHAR* sAttributeName, const unsigned int iValue);
			bool Add(const WCHAR* sAttributeName, const DWORD dwValue);
			bool Add(const WCHAR* sAttributeName, const signed int iValue);
			bool Add(const WCHAR* sAttributeName, const double dValue);
			bool Add(const WCHAR* sAttributeName, const double dValue, const int iDecimalPlaces);
			bool Add(const WCHAR* sAttributeName, const __int64 i64Value);
			bool Add(const WCHAR* sAttributeName, const unsigned __int64 i64Value);
			bool AddBool(const WCHAR* sAttributeName, const bool bValue);

			bool Add(const WCHAR* sAttributeName, XMLWriter* XML);
			bool Add(const WCHAR* sAttributeName, XMLReader* XML);

			bool Add(XMLReader* XML);
			bool Add(XMLWriter* XML);

			int Length(void);

			int Text(WCHAR* sOutXML, int iMaxOutSz);
			void Text(String::StringBuilder* textOut);
			WCHAR* Text(void);
			WCHAR* Text(int* iOutSz);
			void FreePointer(void* pPointer);

#ifdef _XML_USE_COMPRESSION
			WCHAR* Compress(int* iOutLength);
			WCHAR* Compress(int* iOutLength, int iCompressionLevel);
#endif

			bool ToFile(const WCHAR* sFileName);

		protected:
			XMLWriterState State;

		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::XML
} //namespace::NSWFL
#endif
