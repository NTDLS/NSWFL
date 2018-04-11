///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_XMLReader_CPP_
#define _NSWFL_XMLReader_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
using namespace NSWFL::Memory;
extern MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

namespace NSWFL {
	namespace XML {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		using namespace NSWFL::File;
		using namespace NSWFL::String;
		using namespace NSWFL::Conversion;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CXMLReader::~CXMLReader(void)
		{
			this->Destroy();
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CXMLReader::ScanPosition(void)
		{
			return this->State.iScanPosition;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CXMLReader::ScanPosition(int iPosition)
		{
			int iOldValue = this->State.iScanPosition;

			this->State.iScanPosition = iPosition;

			return iOldValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ProgressiveScan(bool bEnable)
		{
			bool bOldValue = this->State.bProgressiveScan;
			this->State.bProgressiveScan = bEnable;

			this->State.iScanPosition = 0;

			return bOldValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CXMLReader::Text(char *sTarget, int iMaxSz)
		{
			strcpy_s(sTarget, iMaxSz, this->State.sOriginalXML);
			return this->State.iOriginalXMLSz;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		const char *CXMLReader::Text(void)
		{
			return this->State.sOriginalXML;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CXMLReader::Length(void)
		{
			return this->State.iOriginalXMLSz;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _CXML_USE_COMPRESSION
		CXMLReader::CXMLReader(const char *sXML, const int iXMLSz, bool bCompressed)
		{
			memset(&this->State, 0, sizeof(this->State));

			if (bCompressed)
			{
				DecompressXML(sXML, iXMLSz, this);
			}
			else this->Initialize(sXML, iXMLSz);
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CXMLReader::CXMLReader(CXMLReader *pReader)
		{
			Initialize(pReader);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::Initialize(CXMLReader *pReader)
		{
			return this->Initialize(pReader->Text(), pReader->Length());
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CXMLReader::CXMLReader(const char *sXML, const int iXMLSz)
		{
			memset(&this->State, 0, sizeof(this->State));

			this->Initialize(sXML, iXMLSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CXMLReader::CXMLReader(const char *sXML)
		{
			memset(&this->State, 0, sizeof(this->State));

			this->Initialize(sXML);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CXMLReader::CXMLReader(void)
		{
			memset(&this->State, 0, sizeof(this->State));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CXMLReader *CXMLReader::SetPointer(const char *sXML, const int iXMLSz)
		{
			memset(&this->State, 0, sizeof(this->State));

			this->State.sOriginalXML = (char *)sXML;
			this->State.iOriginalXMLSz = iXMLSz;

			this->ProgressiveScan(false);

			return this;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CXMLReader *CXMLReader::SetPointer(const char *sXML)
		{
			return this->SetPointer(sXML, (int)strlen(sXML));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::Initialize(const char *sXML)
		{
			return this->Initialize(sXML, (int)strlen(sXML));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			//Read XML from a file.

			1) DO NOT Initialize before this, that would cause a memory leak.
			2) Always Destroy() if this function returns TRUE.
		*/
		bool CXMLReader::FromFile(const char *sFileName)
		{
			HANDLE hSource = NULL;

			hSource = CreateFile(sFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hSource == INVALID_HANDLE_VALUE)
			{
				return false; //File not found or access denied.
			}

			DWORD dwFileSz = 0;
			//Thats right, we don't support files over 1.9GB. Get over it!!
			if (!FileSize(hSource, &dwFileSz))
			{
				//Failed to get the file size.
				CloseHandle(hSource);
				return false;

			}
			if (dwFileSz == 0)
			{
				//The file is empty.
				CloseHandle(hSource);
				return false;
			}
			else if (dwFileSz < 0)
			{
				//File access error.
				CloseHandle(hSource);
				return false;
			}

			DWORD dwBytesRead = 0;
#ifdef _USE_GLOBAL_MEMPOOL
			char *sBuf = (char *)pMem->Allocate(dwFileSz + 1, sizeof(char));
#else
			char *sBuf = (char *)calloc(dwFileSz + 1, sizeof(char));
#endif
			if (sBuf == NULL)
			{
				//Couldnt allocate RAM. Are we out?
				CloseHandle(hSource);
				return false;
			}

			ReadFile(hSource, sBuf, dwFileSz, &dwBytesRead, NULL);
			if (dwBytesRead != dwFileSz)
			{
				//File read error. What the...!?
				CloseHandle(hSource);
				return false;
			}

			CloseHandle(hSource);

			bool bResult = this->Initialize(sBuf, dwBytesRead);

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sBuf);
#else
			free(sBuf);
#endif

			return bResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CXMLReader::Destroy(void)
		{
			this->ProgressiveScan(false);
			if (this->State.sOriginalXML)
			{
#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(this->State.sOriginalXML);
#else
				free(this->State.sOriginalXML);
#endif
			}

			memset(&this->State, 0, sizeof(this->State));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			//Initialize the XML class

			1) Always Destroy() if this function returns TRUE.
		*/
		bool CXMLReader::Initialize(const char *sXML, const int iXMLSz)
		{
			memset(&this->State, 0, sizeof(this->State));

			if (iXMLSz <= 0)
			{
				//No XML to parse.
				return false;
			}

			this->State.iOriginalXMLSz = iXMLSz;

#ifdef _USE_GLOBAL_MEMPOOL
			this->State.sOriginalXML = (char *)pMem->Allocate(this->State.iOriginalXMLSz + 1, sizeof(char));
#else
			this->State.sOriginalXML = (char *)calloc(this->State.iOriginalXMLSz + 1, sizeof(char));
#endif

			if (!this->State.sOriginalXML)
			{
				//Memory allocation failure.
				return false;
			}

			memcpy_s(this->State.sOriginalXML, this->State.iOriginalXMLSz + 1, sXML, this->State.iOriginalXMLSz);
			this->State.sOriginalXML[this->State.iOriginalXMLSz] = '\0';

			this->ProgressiveScan(false);

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToDouble(const char *sTag, double *dOutValue)
		{
			int iResult = 0;
			char sInteger[64];
			if ((iResult = ToString(sTag, sInteger, sizeof(sInteger))) >= 0)
			{
				*dOutValue = fFormattedValue(sInteger);
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToU64(const char *sTag, unsigned __int64 *i64OutValue)
		{
			int iResult = 0;
			char sInteger[64];
			if ((iResult = ToString(sTag, sInteger, sizeof(sInteger))) >= 0)
			{
				*i64OutValue = i64FormattedValue(sInteger);
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToI64(const char *sTag, __int64 *i64OutValue)
		{
			int iResult = 0;
			char sInteger[64];
			if ((iResult = ToString(sTag, sInteger, sizeof(sInteger))) >= 0)
			{
				*i64OutValue = i64FormattedValue(sInteger);
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToBoolean(const char *sTag, bool *bOutValue)
		{
			int iResult = 0;
			char sValue[32];
			if ((iResult = this->ToString(sTag, sValue, sizeof(sValue))) >= 0)
			{
				int iValue = 0;

				if (_strcmpi(sValue, "yes") == 0
					|| _strcmpi(sValue, "true") == 0
					|| _strcmpi(sValue, "on") == 0)
				{
					iValue = 1;
				}
				else {
					iValue = iFormattedValue(sValue);
				}

				if (iValue)
				{
					*bOutValue = true;
				}
				else {
					*bOutValue = false;
				}

				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToUInteger(const char *sTag, DWORD *uiOutValue)
		{
			int iResult = 0;
			char sInteger[64];
			if ((iResult = ToString(sTag, sInteger, sizeof(sInteger))) >= 0)
			{
				*uiOutValue = uiFormattedValue(sInteger);
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToUInteger(const char *sTag, unsigned int *uiOutValue)
		{
			int iResult = 0;
			char sInteger[64];
			if ((iResult = ToString(sTag, sInteger, sizeof(sInteger))) >= 0)
			{
				*uiOutValue = uiFormattedValue(sInteger);
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToInteger(const char *sTag, int *iOutValue)
		{
			int iResult = 0;
			char sInteger[64];
			if ((iResult = ToString(sTag, sInteger, sizeof(sInteger))) >= 0)
			{
				*iOutValue = iFormattedValue(sInteger);
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		double CXMLReader::ToDouble(const char *sTag, double dDefaultValue)
		{
			double dValue = 0;
			if (ToDouble(sTag, &dValue))
			{
				return dValue;
			}
			else return dDefaultValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		double CXMLReader::ToDouble(const char *sTag)
		{
			double dValue = 0;
			if (ToDouble(sTag, &dValue))
			{
				return dValue;
			}
			else return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned __int64 CXMLReader::ToU64(const char *sTag, unsigned __int64 i64DefaultValue)
		{
			unsigned __int64 i64Value = 0;
			if (ToU64(sTag, &i64Value))
			{
				return i64Value;
			}
			else return i64DefaultValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		__int64 CXMLReader::ToI64(const char *sTag, __int64 i64DefaultValue)
		{
			__int64 i64Value = 0;
			if (ToI64(sTag, &i64Value))
			{
				return i64Value;
			}
			else return i64DefaultValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned __int64 CXMLReader::ToU64(const char *sTag)
		{
			unsigned __int64 i64Value = 0;
			if (ToU64(sTag, &i64Value))
			{
				return i64Value;
			}
			else return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		__int64 CXMLReader::ToI64(const char *sTag)
		{
			__int64 i64Value = 0;
			if (ToI64(sTag, &i64Value))
			{
				return i64Value;
			}
			else return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToBoolean(const char *sTag, bool bDefaultValue)
		{
			bool bValue = 0;
			if (this->ToBoolean(sTag, &bValue))
			{
				return bValue;
			}
			else return bDefaultValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToBoolean(const char *sTag)
		{
			bool bValue = 0;
			if (ToBoolean(sTag, &bValue))
			{
				return bValue;
			}
			else return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned int CXMLReader::ToUInteger(const char *sTag, unsigned int uiDefaultValue)
		{
			unsigned int uiValue = 0;
			if (ToUInteger(sTag, &uiValue))
			{
				return uiValue;
			}
			else return uiDefaultValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		unsigned int CXMLReader::ToUInteger(const char *sTag)
		{
			unsigned int uiValue = 0;
			if (ToUInteger(sTag, &uiValue))
			{
				return uiValue;
			}
			else return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CXMLReader::ToInteger(const char *sTag, int iDefaultValue)
		{
			int iValue = 0;
			if (ToInteger(sTag, &iValue))
			{
				return iValue;
			}
			else return iDefaultValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CXMLReader::ToInteger(const char *sTag)
		{
			int iValue = 0;
			if (ToInteger(sTag, &iValue))
			{
				return iValue;
			}
			else return 0; //ToInteger failed.
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::Contains(const char *sTag)
		{
			if (this->State.iOriginalXMLSz <= 0)
			{
				//No XML.
				return false;
			}

			char sTagIn[255];
			char sTagOut[255];

			sprintf_s(sTagIn, sizeof(sTagIn), "<%s>", sTag);
			sprintf_s(sTagOut, sizeof(sTagOut), "</%s>", sTag);

			int iTagIn = 0;
			int iTagOut = 0;

			if ((iTagIn = InStrI(sTagIn, this->State.sOriginalXML, this->State.iOriginalXMLSz, 0)) >= 0)
			{
				if ((iTagOut = InStrI(sTagOut, this->State.sOriginalXML, this->State.iOriginalXMLSz, 0)) >= 0)
				{
					return true;
				}
				else {
					//Closing tag not found.
					return false;
				}
			}
			else {
				//Tag not found.
				return false;
			}

			//Unknown error.
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CXMLReader::AttributeLength(const char *sTag)
		{
			if (this->State.iOriginalXMLSz <= 0)
			{
				//No XML.
				return -1;
			}

			char sTagIn[255];
			char sTagOut[255];

			sprintf_s(sTagIn, sizeof(sTagIn), "<%s>", sTag);
			sprintf_s(sTagOut, sizeof(sTagOut), "</%s>", sTag);

			int iTagIn = 0;
			int iTagOut = 0;

			if ((iTagIn = InStrI(sTagIn, this->State.sOriginalXML, this->State.iOriginalXMLSz, 0)) >= 0)
			{
				if ((iTagOut = InStrI(sTagOut, this->State.sOriginalXML, this->State.iOriginalXMLSz, 0)) >= 0)
				{
					return (iTagOut - iTagIn) - (int)strlen(sTagIn);
				}
				else {
					//Closing tag not found.
					return -2;
				}
			}
			else {
				//Tag not found.
				return -3;
			}

			//Unknown error.
			return -4;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToReader(const char *sTag, const int iTag, CXMLReader *lpXMLReader)
		{
			char sFullTag[255];
			char sITag[64];
			if (_itoa_s(iTag, sITag, sizeof(sITag), 10) == 0)
			{
				strcpy_s(sFullTag, sizeof(sFullTag), sTag);
				strcat_s(sFullTag, sizeof(sFullTag), sITag);
				return this->ToReader(sFullTag, lpXMLReader);
			}

			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToReader(const char *sTag, CXMLReader *lpXMLReader)
		{
			int iAlloc = this->State.iOriginalXMLSz + 1;
			int iLength = 0;

#ifdef _USE_GLOBAL_MEMPOOL
			char *sBuf = (char *)pMem->Allocate(iAlloc, sizeof(char));
#else
			char *sBuf = (char *)calloc(iAlloc, sizeof(char));
#endif

			if ((iLength = this->ToString(sTag, sBuf, iAlloc)) >= 0)
			{
				memset(lpXMLReader, 0, sizeof(CXMLReader));
				lpXMLReader->SetPointer(sBuf, iLength);
				return true;
			}
			else {

#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(sBuf);
#else
				free(sBuf);
#endif

				lpXMLReader = NULL;

				return false;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CXMLReader::ToString(const char *sTag, char *sOutAttrib, int iMaxSz)
		{
			return this->ToString(sTag, sOutAttrib, iMaxSz, "");
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToString(const char *sTag, char *sOutAttrib, int iMaxSz, int *iReturnLength)
		{
			int iLength = ToString(sTag, sOutAttrib, iMaxSz, "");
			if (iLength >= 0)
			{
				*iReturnLength = iLength;
				return true;
			}
			else {
				*iReturnLength = 0;
				return false;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CXMLReader::ToString(const char *sTag, char *sOutAttrib, int iMaxSz, const char *sDefaultValue, int *iReturnLength)
		{
			int iLength = ToString(sTag, sOutAttrib, iMaxSz, sDefaultValue);
			if (iLength >= 0)
			{
				*iReturnLength = iLength;
				return true;
			}
			else {
				*iReturnLength = (int)strlen(sDefaultValue);
				return false;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CXMLReader::ToString(const char *sTag, StringBuilder *OutAttrib)
		{
			return this->ToString(sTag, OutAttrib, "");
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CXMLReader::ToString(const char *sTag, StringBuilder *OutAttrib, const char *sDefaultValue)
		{
			OutAttrib->Clear();

			if (this->State.iOriginalXMLSz <= 0)
			{
				OutAttrib->Append(sDefaultValue);
				return -1; //No XML.
			}

			char sTagIn[255];
			char sTagOut[255];

			sprintf_s(sTagIn, sizeof(sTagIn), "<%s>", sTag);
			sprintf_s(sTagOut, sizeof(sTagOut), "</%s>", sTag);

			int iTagInSz = (int)strlen(sTagIn);
			int iTagIn = 0;
			int iTagOut = 0;
			int iAttributeSz = 0;
			int iStartPosition = 0;

			if (this->State.bProgressiveScan)
			{
				iStartPosition = this->State.iScanPosition;
			}

			if ((iTagIn = InStrI(sTagIn, this->State.sOriginalXML, this->State.iOriginalXMLSz, iStartPosition)) >= 0)
			{
				if ((iTagOut = InStrI(sTagOut, this->State.sOriginalXML, this->State.iOriginalXMLSz, iTagIn + iTagInSz)) >= 0)
				{
					iAttributeSz = (iTagOut - iTagIn) - iTagInSz;

					OutAttrib->Append(this->State.sOriginalXML + iTagIn + iTagInSz, iAttributeSz);

					if (this->State.bProgressiveScan)
					{
						this->State.iScanPosition = iTagOut + iTagInSz + 1;
					}

					return iAttributeSz;
				}
				else {
					OutAttrib->Append(sDefaultValue);
					return -2; //Closing tag not found.
				}
			}
			else {
				OutAttrib->Append(sDefaultValue);
				return -3; //Tag not found.
			}

			OutAttrib->Append(sDefaultValue);
			return -4; //Unknown error.
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CXMLReader::ToString(const char *sTag, char *sOutAttrib, int iMaxSz, const char *sDefaultValue)
		{
			if (this->State.iOriginalXMLSz <= 0)
			{
				strcpy_s(sOutAttrib, iMaxSz, sDefaultValue);
				return -1; //No XML.
			}

			char sTagIn[255];
			char sTagOut[255];

			sprintf_s(sTagIn, sizeof(sTagIn), "<%s>", sTag);
			sprintf_s(sTagOut, sizeof(sTagOut), "</%s>", sTag);

			int iTagInSz = (int)strlen(sTagIn);
			int iTagIn = 0;
			int iTagOut = 0;
			int iAttributeSz = 0;
			int iStartPosition = 0;

			if (this->State.bProgressiveScan)
			{
				iStartPosition = this->State.iScanPosition;
			}

			if ((iTagIn = InStrI(sTagIn, this->State.sOriginalXML, this->State.iOriginalXMLSz, iStartPosition)) >= 0)
			{
				if ((iTagOut = InStrI(sTagOut, this->State.sOriginalXML, this->State.iOriginalXMLSz, iTagIn + iTagInSz)) >= 0)
				{
					iAttributeSz = (iTagOut - iTagIn) - iTagInSz;
					strncpy_s(sOutAttrib, iMaxSz, this->State.sOriginalXML + iTagIn + iTagInSz, iAttributeSz);

					if (this->State.bProgressiveScan)
					{
						this->State.iScanPosition = iTagOut + iTagInSz + 1;
					}

					return iAttributeSz;
				}
				else {
					strcpy_s(sOutAttrib, iMaxSz, sDefaultValue);
					return -2; //Closing tag not found.
				}
			}
			else {
				strcpy_s(sOutAttrib, iMaxSz, sDefaultValue);
				return -3; //Tag not found.
			}

			strcpy_s(sOutAttrib, iMaxSz, sDefaultValue);
			return -4; //Unknown error.
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			//Writes XML to a file.
		*/
		bool CXMLReader::ToFile(const char *sFileName)
		{
			HANDLE hTarget = NULL;

			hTarget = CreateFile(sFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hTarget == INVALID_HANDLE_VALUE)
			{
				return false; //File not found or access denied.
			}

			DWORD dwBytesWritten = 0;
			WriteFile(hTarget, this->State.sOriginalXML, this->State.iOriginalXMLSz, &dwBytesWritten, NULL);
			CloseHandle(hTarget);

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::XML
} //namespace::NSWFL
#endif
