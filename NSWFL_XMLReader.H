///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_XMLReader_H_
#define _NSWFL_XMLReader_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace NSWFL::String;

namespace NSWFL {
	namespace XML {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class XMLReader {

		public:
			void *pUserData; //Use it for whatever you want!

			typedef struct _tag_XMLReader_State {
				char *sOriginalXML;
				int iOriginalXMLSz;
				bool bProgressiveScan;
				int iScanPosition;
			} XMLREADERSTATE, *LPXMLREADERSTATE;

			XMLReader(const char *sXML, const int iXMLSz, bool bCompressed);
			XMLReader(const char *sXML, const int iXMLSz);
			XMLReader(const char *sXML);
			XMLReader(XMLReader *pReader);
			XMLReader(void);
			~XMLReader(void);

			const char *Text(void);
			int Text(char *sTarget, int iMaxSz);
			int Length(void);

			bool Initialize(XMLReader *pReader);
			bool Initialize(const char *sXML);
			bool Initialize(const char *sXML, const int iXMLSz);
			XMLReader *SetPointer(const char *sXML, const int iXMLSz); //For low-level use.
			XMLReader *SetPointer(const char *sXML); //For low-level use.
			void Destroy(void);

			double ToDouble(const char *sTag);
			int ToInteger(const char *sTag);
			__int64 ToI64(const char *sTag);
			unsigned __int64 ToU64(const char *sTag);
			bool ToInteger(const char *sTag, int *iOutValue);
			bool ToI64(const char *sTag, __int64 *i64OutValue);
			bool ToU64(const char *sTag, unsigned __int64 *i64OutValue);
			bool ToDouble(const char *sTag, double *dOutValue);
			bool ToBoolean(const char *sTag, bool *bOutValue);
			bool ToBoolean(const char *sTag, bool bDefaultValue);
			bool ToBoolean(const char *sTag);
			int ToString(const char *sTag, char *sOutAttrib, int iMaxSz);
			int ToString(const char *sTag, char *sOutAttrib, int iMaxSz, const char *sDefaultValue);
			bool ToString(const char *sTag, char *sOutAttrib, int iMaxSz, const char *sDefaultValue, int *iReturnLength);
			bool ToString(const char *sTag, char *sOutAttrib, int iMaxSz, int *iReturnLength);
			int ToString(const char *sTag, StringBuilder *OutAttrib);
			int ToString(const char *sTag, StringBuilder *OutAttrib, const char *sDefaultValue);

			unsigned int ToUInteger(const char *sTag);
			bool ToUInteger(const char *sTag, unsigned int *uiOutValue);
			bool ToUInteger(const char *sTag, DWORD *uiOutValue);
			bool ToReader(const char *sTag, XMLReader *lpXMLReader);
			bool ToReader(const char *sTag, const int iTag, XMLReader *lpXMLReader);
			bool ToFile(const char *sFileName);

			int ToInteger(const char *sTag, int iDefaultValue);
			unsigned int ToUInteger(const char *sTag, unsigned int uiDefaultValue);
			__int64 ToI64(const char *sTag, __int64 i64DefaultValue);
			unsigned __int64 ToU64(const char *sTag, unsigned __int64 i64DefaultValue);
			double ToDouble(const char *sTag, double dDefaultValue);

			bool FromFile(const char *sFileName);
			int AttributeLength(const char *sTag);
			bool Contains(const char *sTag);
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
