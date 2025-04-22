///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_XMLWriter_CPP_
#define _NSWFL_XMLWriter_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

using namespace NSWFL::String;

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

namespace NSWFL {
	namespace XML {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		XMLWriter::XMLWriter(const WCHAR* sChildName)
		{
			memset(&this->State, 0, sizeof(this->State));
			this->Initialize(sChildName);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		XMLWriter::~XMLWriter(void)
		{
			this->Destroy();
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		XMLWriter::XMLWriter(void)
		{
			memset(&this->State, 0, sizeof(this->State));
			this->State.pBuilder = NULL;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		XMLReader* XMLWriter::ToReader(XMLReader* lpXMLReader)
		{
			int iLength = this->Length();

#ifdef _USE_GLOBAL_MEMPOOL
			WCHAR* sText = (WCHAR*)pMem->Allocate(iLength + 1, sizeof(WCHAR));
#else
			WCHAR* sText = (WCHAR*)calloc(iLength + 1, sizeof(WCHAR));
#endif

			if (sText)
			{
				iLength = this->Text(sText, iLength);
				if (lpXMLReader->Initialize(sText, iLength))
				{
#ifdef _USE_GLOBAL_MEMPOOL
					pMem->Free(sText);
#else
					free(sText);
#endif
					return lpXMLReader;
				}
#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(sText);
#else
				free(sText);
#endif
			}
			return NULL;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int XMLWriter::GetName(WCHAR* sOutName, int iMaxOut)
		{
			wcscpy_s(sOutName, iMaxOut, this->State.sName);
			return (int)wcslen(this->State.sName);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::ChangeName(const WCHAR* sChildName)
		{
			wcscpy_s(this->State.sName, sizeof(this->State.sName), sChildName);
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Initialize(const WCHAR* sChildName)
		{
			if (sChildName)
			{
				if ((int)wcslen(sChildName) > 0)
				{
					wcscpy_s(this->State.sName, sizeof(this->State.sName), sChildName);
				}
				else {
					memset(this->State.sName, 0, sizeof(this->State.sName));
				}
			}
			else {
				memset(this->State.sName, 0, sizeof(this->State.sName));
			}

#ifdef _USE_GLOBAL_MEMPOOL
			this->State.pBuilder = (StringBuilder*)pMem->TrackMemory(new StringBuilder);
#else
			this->State.pBuilder = new StringBuilder;
#endif

			return true;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Initialize(void)
		{
			return this->Initialize(NULL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void XMLWriter::Destroy(void)
		{
			if (this->State.pBuilder)
			{
#ifdef _USE_GLOBAL_MEMPOOL
				delete (StringBuilder*)pMem->UnTrackMemory(this->State.pBuilder);
#else
				delete this->State.pBuilder;
#endif
			}

			memset(&this->State, 0, sizeof(this->State));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(XMLWriter* XML)
		{
			int iAlloc = XML->Length() + 1;
			int iLength = 0;

#ifdef _USE_GLOBAL_MEMPOOL
			WCHAR* sBuf = (WCHAR*)pMem->Allocate(iAlloc, sizeof(WCHAR));
#else
			WCHAR* sBuf = (WCHAR*)calloc(iAlloc, sizeof(WCHAR));
#endif

			if (sBuf == NULL)
			{
				return false;
			}

			if ((iLength = XML->Text(sBuf, iAlloc)) > 0)
			{
				bool bResult = this->State.pBuilder->Append(sBuf, iLength);

#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(sBuf);
#else
				free(sBuf);
#endif

				return bResult;
			}

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sBuf);
#else
			free(sBuf);
#endif

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(XMLReader* XML)
		{
			return this->State.pBuilder->Append(XML->Text(), XML->Length());
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(const WCHAR* sAttributeName, XMLWriter* XML)
		{
			return this->Add(sAttributeName, XML->State.pBuilder->Buffer, XML->State.pBuilder->Length);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(const WCHAR* sAttributeName, XMLReader* XML)
		{
			return this->Add(sAttributeName, XML->Text(), XML->Length());
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(const WCHAR* sAttributeName, const WCHAR* sValue)
		{
			if (sValue)
			{
				return this->Add(sAttributeName, sValue, (int)wcslen(sValue));
			}
			else {
				return this->Add(sAttributeName, _T(""), 0);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(const WCHAR* sAttributeName, const WCHAR* sValue, const int iValueSz)
		{
			WCHAR sTag[255];

			swprintf_s(sTag, sizeof(sTag), _T("<%s>"), sAttributeName);
			this->State.pBuilder->Append(sTag);

			this->State.pBuilder->Append(sValue, iValueSz);

			swprintf_s(sTag, sizeof(sTag), _T("</%s>"), sAttributeName);
			this->State.pBuilder->Append(sTag);

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(const WCHAR* sAttributeName, const double dValue, const int iDecimalPlaces)
		{
			WCHAR sFormat[16];
			swprintf_s(sFormat, sizeof(sFormat), _T("<%%s>%%.%df</%%s>"), iDecimalPlaces);
			this->State.pBuilder->AppendF(sFormat, sAttributeName, dValue, sAttributeName);
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(const WCHAR* sAttributeName, const double dValue)
		{
			this->State.pBuilder->AppendF(_T("<%s>%f</%s>"), sAttributeName, dValue, sAttributeName);
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(const WCHAR* sAttributeName, const signed int iValue)
		{
			this->State.pBuilder->AppendF(_T("<%s>%d</%s>"), sAttributeName, iValue, sAttributeName);
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(const WCHAR* sAttributeName, const DWORD dwValue)
		{
			this->State.pBuilder->AppendF(_T("<%s>%d</%s>"), sAttributeName, dwValue, sAttributeName);
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(const WCHAR* sAttributeName, const unsigned int iValue)
		{
			this->State.pBuilder->AppendF(_T("<%s>%d</%s>"), sAttributeName, iValue, sAttributeName);
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(const WCHAR* sAttributeName, const unsigned __int64 i64Value)
		{
			this->State.pBuilder->AppendF(_T("<%s>%llu</%s>"), sAttributeName, i64Value, sAttributeName);
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool XMLWriter::Add(const WCHAR* sAttributeName, const __int64 i64Value)
		{
			this->State.pBuilder->AppendF(_T("<%s>%I64d</%s>"), sAttributeName, i64Value, sAttributeName);
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Could not overload due to conflicts with other functions.
		*/
		bool XMLWriter::AddBool(const WCHAR* sAttributeName, const bool bValue)
		{
			return this->Add(sAttributeName, bValue ? _T("Yes") : _T("No"));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			The returned pointer must be freed externally.
		*/
#ifdef _XML_USE_COMPRESSION
		WCHAR* XMLWriter::Compress(int* iOutLength)
		{
			return this->Compress(iOutLength, 9);
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			The returned pointer must be freed externally.
		*/
#ifdef _XML_USE_COMPRESSION
		WCHAR* XMLWriter::Compress(int* iOutLength, int iCompressionLevel)
		{
			return CompressXML(this, iOutLength, iCompressionLevel);
		}
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			The returned pointer must be freed externally.
		*/
		WCHAR* XMLWriter::Text(void)
		{
			int iOutSz = 0;
			return this->Text(&iOutSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			The string builder must have been externally allocated.
		*/
		void XMLWriter::Text(StringBuilder* textOut)
		{
			int iRequiredLength = this->Length();
			textOut->Resize(iRequiredLength + 10);
			textOut->Length = this->Text(textOut->Buffer, textOut->Alloc);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			This function is used to easily free a pointer which was returned which needs manual freeing. For example, the pointer returned from Text().
		*/
		void XMLWriter::FreePointer(void* pPointer)
		{
#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(pPointer);
#else
			free(pPointer);
#endif
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			The returned pointer must be freed externally.
		*/

		WCHAR* XMLWriter::Text(int* iOutSz)
		{
			int iMaxSize = this->Length() + 1;
			if (iMaxSize > 0)
			{
#ifdef _USE_GLOBAL_MEMPOOL
				WCHAR* sXML = (WCHAR*)pMem->Allocate(iMaxSize, sizeof(WCHAR));
#else
				WCHAR* sXML = (WCHAR*)calloc(iMaxSize, sizeof(WCHAR));
#endif
				if (sXML)
				{
					*iOutSz = this->Text(sXML, iMaxSize);
					return sXML;
				}
			}

			return NULL;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int XMLWriter::Text(WCHAR* sOutXML, int iMaxOutSz)
		{
			this->State.pBuilder->Terminate();

			WCHAR sTag[255];
			bool bHasName = ((int)wcslen(this->State.sName) > 0);

			if (bHasName)
			{
				swprintf_s(sTag, sizeof(sTag), _T("<%s>"), this->State.sName);
				wcscpy_s(sOutXML, iMaxOutSz, sTag);
			}

			wcscat_s(sOutXML, iMaxOutSz, this->State.pBuilder->Buffer);

			if (bHasName)
			{
				swprintf_s(sTag, sizeof(sTag), _T("</%s>"), this->State.sName);
				wcscat_s(sOutXML, iMaxOutSz, sTag);
			}

			return this->Length() - 1; //Return the actual size, excluding the null terminator.
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//The minimum ammount of RAM required to store the XML (including the NULL terminator).
		int XMLWriter::Length(void)
		{
			int iTotalSize = 0;

			if ((iTotalSize = (int)wcslen(this->State.sName)) > 0)
			{
				iTotalSize *= 2; //One name in the opening tag, and on in the closing tag.
				iTotalSize += 6; // Extra space for "<>", "</>" and the NULL terminator.
			}

			return iTotalSize + this->State.pBuilder->Length;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		StringBuilder* XMLWriter::GetBuilder(void)
		{
			return this->State.pBuilder;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			//Writes XML to a file.
		*/
		bool XMLWriter::ToFile(const WCHAR* sFileName)
		{
			HANDLE hTarget = NULL;

			hTarget = CreateFile(sFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hTarget == INVALID_HANDLE_VALUE)
			{
				return false; //File not found or access denied.
			}

			DWORD dwBytesWritten = 0;

			int iNameLen = (int)wcslen(this->State.sName);

			if (iNameLen > 0)
			{
				WriteFile(hTarget, "<", 1, &dwBytesWritten, NULL);
				WriteFile(hTarget, this->State.sName, iNameLen, &dwBytesWritten, NULL);
				WriteFile(hTarget, ">", 1, &dwBytesWritten, NULL);
			}

			WriteFile(hTarget, this->State.pBuilder->Buffer, this->State.pBuilder->Length, &dwBytesWritten, NULL);

			if (iNameLen > 0)
			{
				WriteFile(hTarget, "</", 2, &dwBytesWritten, NULL);
				WriteFile(hTarget, this->State.sName, iNameLen, &dwBytesWritten, NULL);
				WriteFile(hTarget, ">", 1, &dwBytesWritten, NULL);
			}

			CloseHandle(hTarget);

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::XML
} //namespace::NSWFL
#endif
