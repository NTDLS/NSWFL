///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_Trace_CPP_
#define _NSWFL_Trace_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

using namespace NSWFL::String;
using namespace NSWFL::File;
using namespace NSWFL::ListView;
using namespace NSWFL::DateTime;
using namespace NSWFL::Collections;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace Debug {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CTrace::CTrace(const WCHAR*sApplicationName, const WCHAR*sApplicationVersion)
		{
			this->_ApplicationName = _wcsdup(sApplicationName);
			this->_ApplicationVersion = _wcsdup(sApplicationVersion);

			WCHAR sModule[MAX_PATH];
			WCHAR sPath[MAX_PATH];
			if (GetModuleFileName(NULL, sModule, _countof(sModule)))
			{
				if (GetFilePath(sModule, sPath, _countof(sPath)))
				{
					this->_LogPath = _wcsdup(sPath);
				}
				else {
					__debugbreak();
				}
			}
			else {
				__debugbreak();
			}

			memset(&this->_CS, 0, sizeof(this->_CS));
			InitializeCriticalSection(&this->_CS);
			this->_Stack = NULL;
			this->_TracingEnabled = false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CTrace::~CTrace(void)
		{
			this->Stop();
			DeleteCriticalSection(&this->_CS);

			free(this->_ApplicationName);
			free(this->_ApplicationVersion);
			free(this->_LogPath);

			this->_Stack = NULL;
			this->_TracingEnabled = false;
			memset(&this->_CS, 0, sizeof(this->_CS));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CTrace::IsTracingEnabled(void)
		{
			return this->_TracingEnabled;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CTrace::Lock(void)
		{
			EnterCriticalSection(&this->_CS);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CTrace::TryLock(void)
		{
			return(TryEnterCriticalSection(&this->_CS) != 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CTrace::UnLock(void)
		{
			LeaveCriticalSection(&this->_CS);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CTrace::Start(int iType)
		{
			this->Lock();

			this->_Type = iType;

			if (this->_TracingEnabled)
			{
				this->_Stack->Clear();
			}
			else {
				this->_Stack = new Stack();
				this->_TracingEnabled = true;
			}

			this->UnLock();
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CTrace::Start(void)
		{
			this->Lock();

			this->_Type = TRACE_TYPE_GENERIC;

			if (this->_TracingEnabled)
			{
				this->_Stack->Clear();
			}
			else {
				this->_Stack = new Stack();
				this->_TracingEnabled = true;
			}

			this->UnLock();
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CTrace::Trace(const WCHAR*sText)
		{
			this->Lock();

			if (this->_TracingEnabled)
			{
				WCHAR sDate[255];
				WCHAR sTime[255];

				if (Get_ShortDate(sDate, sizeof(sDate)) && Get_Time(sTime, sizeof(sTime), 0))
				{
					int iAllocate = (int)(wcslen(sText) + wcslen(sDate) + wcslen(sTime) + 10);
					WCHAR*sMessage = (WCHAR*)calloc(iAllocate, sizeof(WCHAR));
					if (sMessage)
					{
						swprintf_s(sMessage, iAllocate, _T("(%s %s) - %s"), sDate, sTime, sText);
						this->_Stack->Push(sMessage);
						free(sMessage);
					}
					else {
						//Low on memory or the heap is corrupt, either way, I have money on things going badly!
						this->_Stack->Push(sText);
					}
				}
			}

			this->UnLock();
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CTrace::Stop(void)
		{
			return this->Stop(NULL);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CTrace::Stop(StringBuilder *pOutText)
		{
			StringBuilder sLastResort;

			int iErrorCount = 0;

			this->Lock();

			if (this->_TracingEnabled)
			{
				if ((iErrorCount = (int)this->_Stack->StackSize()) > 0)
				{
					WCHAR sDate[255];
					if (Get_ShortDate(sDate, sizeof(sDate)))
					{
						WCHAR sPath[MAX_PATH];
						WCHAR sFileName[MAX_PATH];
						_wmakepath_s(sPath, _countof(sPath), NULL, this->_LogPath, _T("Trace"), NULL);
						CorrectReversePath(sPath, _countof(sPath), true);
						CreateFolderStructure(sPath);

						_wmakepath_s(sFileName, _countof(sFileName), NULL, sPath, sDate, _T(".txt"));
						CorrectReversePath(sFileName, sizeof(sFileName), true);

						FILE *hLogFile = NULL;
						if (_wfopen_s(&hLogFile, sFileName, _T("ab")) != 0)
						{
							hLogFile = NULL;
						}

						for (unsigned int iExp = 0; iExp < this->_Stack->StackSize(); iExp++)
						{
							if (pOutText)
							{
								if (pOutText->Length > 0)
								{
									pOutText->Append(_T("\r\n"));
								}
								pOutText->Append((WCHAR*)this->_Stack->Peek(iExp)->Value);
							}
							if (hLogFile)
							{
								fwprintf_s(hLogFile, _T("%ws\r\n"), (WCHAR*)this->_Stack->Peek(iExp)->Value);
							}
							if (!pOutText && !hLogFile)
							{
								if (sLastResort.Length == 0)
								{
									sLastResort.AppendF(_T("This message is being raised by %s version %s.\r\n")
										_T("This message is not normal and reflects a case where the service")
											_T(" was unable to write this information to a log file or prompt the")
												_T(" user in any other manner. The messages are as follows:\r\n\r\n"),
										this->_ApplicationName, this->_ApplicationVersion);
								}
								sLastResort.AppendF(_T("%ws\r\n"), (WCHAR*)this->_Stack->Peek(iExp)->Value);
							}
						}

						if (hLogFile)
						{
							fclose(hLogFile);
						}
					}
				}
				this->_TracingEnabled = false;
				delete this->_Stack;
			}

			this->UnLock();

			if (sLastResort.Length > 0)
			{
				MessageBox(NULL, sLastResort.Buffer, this->_ApplicationName, MB_ICONERROR | MB_TASKMODAL);
			}

			return iErrorCount;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CTrace::Type(void)
		{
			return this->_Type;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Debug
} //namespace::NSWFL
#endif
