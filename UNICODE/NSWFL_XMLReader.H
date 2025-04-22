///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_XMLReader_H_
#define _NSWFL_XMLReader_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace XML {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class XMLReader {

		public:
			void* pUserData; //Use it for whatever you want!

			typedef struct _tag_XMLReader_State {
				WCHAR* sOriginalXML;
				int iOriginalXMLSz;
				bool bProgressiveScan;
				int iScanPosition;
			} XMLREADERSTATE, * LPXMLREADERSTATE;

			XMLReader(const WCHAR* sXML, const int iXMLSz, bool bCompressed);
			XMLReader(const WCHAR* sXML, const int iXMLSz);
			XMLReader(const WCHAR* sXML);
			XMLReader(XMLReader* pReader);
			XMLReader(void);
			~XMLReader(void);

			const WCHAR* Text(void);
			int Text(WCHAR* sTarget, int iMaxSz);
			int Length(void);

			bool Initialize(XMLReader* pReader);
			bool Initialize(const WCHAR* sXML);
			bool Initialize(const WCHAR* sXML, const int iXMLSz);
			XMLReader* SetPointer(const WCHAR* sXML, const int iXMLSz); //For low-level use.
			XMLReader* SetPointer(const WCHAR* sXML); //For low-level use.
			void Destroy(void);

			double ToDouble(const WCHAR* sTag);
			int ToInteger(const WCHAR* sTag);
			__int64 ToI64(const WCHAR* sTag);
			unsigned __int64 ToU64(const WCHAR* sTag);
			bool ToInteger(const WCHAR* sTag, int* iOutValue);
			bool ToI64(const WCHAR* sTag, __int64* i64OutValue);
			bool ToU64(const WCHAR* sTag, unsigned __int64* i64OutValue);
			bool ToDouble(const WCHAR* sTag, double* dOutValue);
			bool ToBoolean(const WCHAR* sTag, bool* bOutValue);
			bool ToBoolean(const WCHAR* sTag, bool bDefaultValue);
			bool ToBoolean(const WCHAR* sTag);
			int ToString(const WCHAR* sTag, WCHAR* sOutAttrib, int iMaxSz);
			int ToString(const WCHAR* sTag, WCHAR* sOutAttrib, int iMaxSz, const WCHAR* sDefaultValue);
			bool ToString(const WCHAR* sTag, WCHAR* sOutAttrib, int iMaxSz, const WCHAR* sDefaultValue, int* iReturnLength);
			bool ToString(const WCHAR* sTag, WCHAR* sOutAttrib, int iMaxSz, int* iReturnLength);
			int ToString(const WCHAR* sTag, String::StringBuilder* OutAttrib);
			int ToString(const WCHAR* sTag, String::StringBuilder* OutAttrib, const WCHAR* sDefaultValue);

			unsigned int ToUInteger(const WCHAR* sTag);
			bool ToUInteger(const WCHAR* sTag, unsigned int* uiOutValue);
			bool ToUInteger(const WCHAR* sTag, DWORD* uiOutValue);
			bool ToReader(const WCHAR* sTag, XMLReader* lpXMLReader);
			bool ToReader(const WCHAR* sTag, const int iTag, XMLReader* lpXMLReader);
			bool ToFile(const WCHAR* sFileName);

			int ToInteger(const WCHAR* sTag, int iDefaultValue);
			unsigned int ToUInteger(const WCHAR* sTag, unsigned int uiDefaultValue);
			__int64 ToI64(const WCHAR* sTag, __int64 i64DefaultValue);
			unsigned __int64 ToU64(const WCHAR* sTag, unsigned __int64 i64DefaultValue);
			double ToDouble(const WCHAR* sTag, double dDefaultValue);

			bool FromFile(const WCHAR* sFileName);
			int AttributeLength(const WCHAR* sTag);
			bool Contains(const WCHAR* sTag);
			bool ProgressiveScan(bool bEnable);

			int ScanPosition(void);
			int ScanPosition(int iPosition);

		protected:
			XMLREADERSTATE State;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::XML
} //namespace::NSWFL
#endif
