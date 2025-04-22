///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_COMMANDLINEPARSER_CPP_
#define _NSWFL_COMMANDLINEPARSER_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace CommandLine {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CommandLineParser::CommandLineParser(void)
		{
			this->_sArray = NULL;
			this->_iCount = 0;
			this->_iLength = 0;

			ParseCmd(GetCommandLine(), NULL, false);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CommandLineParser::~CommandLineParser(void)
		{
			int iArg = 0;

			while (iArg < _iCount)
			{
#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(this->_sArray[iArg]);
#else
				free(this->_sArray[iArg]);
#endif
				this->_sArray[iArg] = NULL;

				iArg++;
			}

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(this->_sArray);
			pMem->Free(this->_iLength);
#else
			free(this->_sArray);
			free(this->_iLength);
#endif

			this->_sArray = NULL;
			this->_iCount = 0;
			this->_iLength = 0;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CommandLineParser::IsCommand(const int iArg, const WCHAR* sCommandName, WCHAR* sOutCmdData, int iMaxOutSize)
		{
			int iArgSz = (int)wcslen(sCommandName);
			if (_wcsnicmp(this->_sArray[iArg], sCommandName, iArgSz) == 0)
			{
				return this->GetArgumentData(iArg, iArgSz, sOutCmdData, iMaxOutSize);
			}
			else {
				return 0;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CommandLineParser::IsCommand(const int iArg, const WCHAR* sCommandName)
		{
			int iArgSz = (int)wcslen(sCommandName);
			if (_wcsnicmp(this->_sArray[iArg], sCommandName, iArgSz) == 0)
			{
				return iArgSz;
			}
			else {
				return 0;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CommandLineParser::GetArgumentData(const int iArg, const int iSkpLen, int* iValue)
		{
			WCHAR sValue[64];
			int iReturnValue = GetArgumentData(iArg, iSkpLen, sValue, sizeof(sValue));
			*iValue = _wtol(sValue);
			return iReturnValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CommandLineParser::GetArgumentData(const int iArg, const int iSkpLen, WCHAR* sOutBuf, int iMaxSz)
		{
			int iWPos = 0;
			int iRPos = iSkpLen;

			if (((this->_iLength[iArg] - iSkpLen) + 1) > iMaxSz)
			{
				// Returns the number of bytes required as a negitive number.
				return (0 - (this->_iLength[iArg] - iSkpLen));
			}

			while (iRPos < this->_iLength[iArg])
			{
				if (sOutBuf && iMaxSz > 0)
				{
					sOutBuf[iWPos++] = this->_sArray[iArg][iRPos++];
				}
			}

			if (sOutBuf && iMaxSz > 0)
			{
				sOutBuf[iWPos] = '\0';
			}

			return iWPos; //The length of the string returned in sOutBuf.
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CommandLineParser::AddCommand(const WCHAR* sCommandLine)
		{
#ifdef _USE_GLOBAL_MEMPOOL
			this->_sArray = (WCHAR**)pMem->ReAllocate(this->_sArray, sizeof(WCHAR*), _iCount + 1);
			this->_iLength = (int*)pMem->ReAllocate(this->_iLength, sizeof(int), _iCount + 1);
			this->_sArray[_iCount] = pMem->CloneString(sCommandLine);
			this->_iLength[_iCount] = (int)wcslen(this->_sArray[_iCount]);
#else
			this->_sArray = (WCHAR**)realloc(this->_sArray, sizeof(WCHAR*) * _iCount + 1);
			this->_iLength = (int*)realloc(this->_iLength, sizeof(int) * _iCount + 1);
			this->_sArray[_iCount] = _wcsdup(sCommandLine);
			this->_iLength[_iCount] = (int)wcslen(this->_sArray[_iCount]);
#endif

			this->_iCount++;

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CommandLineParser::ParseCmd(WCHAR* sCmdLine, int* iOutMaxLen, bool bEval)
		{
			bool bInQuote = false;

			WCHAR* sBuf = NULL;

			int iCmdSz = (int)wcslen(sCmdLine);
			int iMaxLen = 0;
			int iRPos = 0;
			int iWPos = 0;

			if (!bEval)
			{
				if (!this->ParseCmd(sCmdLine, &iMaxLen, true))
				{
					return false;
				}

#ifdef _USE_GLOBAL_MEMPOOL
				sBuf = (WCHAR*)pMem->Allocate(sizeof(WCHAR), iMaxLen + 1);
				this->_sArray = (WCHAR**)pMem->Allocate(sizeof(WCHAR*), _iCount);
				this->_iLength = (int*)pMem->Allocate(sizeof(int), _iCount);
#else
				this->_sArray = (WCHAR**)calloc(sizeof(WCHAR*), _iCount);
				sBuf = (WCHAR*)calloc(sizeof(WCHAR), iMaxLen + 1);
				this->_iLength = (int*)calloc(sizeof(int), _iCount);
#endif

				_iCount = 0;
			}

			while (iRPos < iCmdSz)
			{
				if (sCmdLine[iRPos] == '\"')
				{
					bInQuote = !bInQuote;
					iRPos++;
					continue;
				}

				if (sCmdLine[iRPos] == ' ' && bInQuote == false)
				{
					if (!bEval)
					{
						sBuf[iWPos] = '\0';

						this->_iLength[_iCount] = iWPos;
#ifdef _USE_GLOBAL_MEMPOOL
						this->_sArray[_iCount] = (WCHAR*)pMem->Allocate(sizeof(WCHAR), iWPos + 1);
#else
						this->_sArray[_iCount] = (WCHAR*)calloc(sizeof(WCHAR), iWPos + 1);
#endif
						wcscpy_s(this->_sArray[_iCount], iWPos + 1, sBuf);
					}
					else {
						if (iWPos > *iOutMaxLen)
						{
							*iOutMaxLen = iWPos;
						}
					}

					//Skip white spaces.
					while (iRPos < iCmdSz && sCmdLine[iRPos] == ' ')
					{
						iRPos++;
					}

					_iCount++;

					iWPos = 0;
				}
				else {
					if (!bEval)
					{
						sBuf[iWPos++] = sCmdLine[iRPos++];
					}
					else {
						iWPos++;
						iRPos++;
					}
				}
			}

			if (iWPos > 0)
			{
				if (!bEval)
				{
					sBuf[iWPos] = '\0';

					this->_iLength[_iCount] = iWPos;
#ifdef _USE_GLOBAL_MEMPOOL
					this->_sArray[_iCount] = (WCHAR*)pMem->Allocate(sizeof(WCHAR), iWPos + 1);
#else
					this->_sArray[_iCount] = (WCHAR*)calloc(sizeof(WCHAR), iWPos + 1);
#endif
					wcscpy_s(this->_sArray[_iCount], iWPos + 1, sBuf);
				}
				else {
					if (iWPos > *iOutMaxLen)
					{
						*iOutMaxLen = iWPos;
					}
				}

				_iCount++;
			}

			if (!bEval)
			{
#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(sBuf);
#else
				free(sBuf);
#endif
			}

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::CommandLine
} //namespace::NSWFL
#endif
