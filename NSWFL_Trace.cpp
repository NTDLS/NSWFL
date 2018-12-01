///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
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

		CTrace::CTrace(const char *sApplicationName, const char *sApplicationVersion)
		{
			this->_ApplicationName = _strdup(sApplicationName);
			this->_ApplicationVersion = _strdup(sApplicationVersion);

			char sModule[MAX_PATH];
			char sPath[MAX_PATH];
			if (GetModuleFileName(NULL, sModule, sizeof(sModule)))
			{
				if (GetFilePath(sModule, sPath, sizeof(sPath)))
				{
					this->_LogPath = _strdup(sPath);
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

		void CTrace::Trace(const char *sText)
		{
			this->Lock();

			if (this->_TracingEnabled)
			{
				char sDate[255];
				char sTime[255];

				if (Get_ShortDate(sDate, sizeof(sDate)) && Get_Time(sTime, sizeof(sTime), 0))
				{
					int iAllocate = (int)(strlen(sText) + strlen(sDate) + strlen(sTime) + 10);
					char *sMessage = (char *)calloc(iAllocate, sizeof(char));
					if (sMessage)
					{
						sprintf_s(sMessage, iAllocate, "(%s %s) - %s", sDate, sTime, sText);
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
					char sDate[255];
					if (Get_ShortDate(sDate, sizeof(sDate)))
					{
						char sPath[MAX_PATH];
						char sFileName[MAX_PATH];
						_makepath_s(sPath, sizeof(sPath), NULL, this->_LogPath, "Trace", NULL);
						CorrectReversePath(sPath, sizeof(sPath), true);
						CreateFolderStructure(sPath);

						_makepath_s(sFileName, sizeof(sFileName), NULL, sPath, sDate, ".txt");
						CorrectReversePath(sFileName, sizeof(sFileName), true);

						FILE *hLogFile = NULL;
						if (fopen_s(&hLogFile, sFileName, "ab") != 0)
						{
							hLogFile = NULL;
						}

						for (unsigned int iExp = 0; iExp < this->_Stack->StackSize(); iExp++)
						{
							if (pOutText)
							{
								if (pOutText->Length > 0)
								{
									pOutText->Append("\r\n");
								}
								pOutText->Append((char *)this->_Stack->Peek(iExp)->Value);
							}
							if (hLogFile)
							{
								fprintf_s(hLogFile, "%s\r\n", (char *)this->_Stack->Peek(iExp)->Value);
							}
							if (!pOutText && !hLogFile)
							{
								if (sLastResort.Length == 0)
								{
									sLastResort.AppendF("This message is being raised by %s version %s.\r\n"
										"This message is not normal and reflects a case where the service"
										" was unable to write this information to a log file or prompt the"
										" user in any other manner. The messages are as follows:\r\n\r\n",
										this->_ApplicationName, this->_ApplicationVersion);
								}
								sLastResort.AppendF("%s\r\n", (char *)this->_Stack->Peek(iExp)->Value);
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
