///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
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
using namespace NSWFL::Memory;
extern MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
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

		int CommandLineParser::IsCommand(const int iArg, const char *sCommandName, char *sOutCmdData, int iMaxOutSize)
		{
			int iArgSz = (int)strlen(sCommandName);
			if (_strnicmp(this->_sArray[iArg], sCommandName, iArgSz) == 0)
			{
				return this->GetArgumentData(iArg, iArgSz, sOutCmdData, iMaxOutSize);
			}
			else {
				return 0;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CommandLineParser::IsCommand(const int iArg, const char *sCommandName)
		{
			int iArgSz = (int)strlen(sCommandName);
			if (_strnicmp(this->_sArray[iArg], sCommandName, iArgSz) == 0)
			{
				return iArgSz;
			}
			else {
				return 0;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CommandLineParser::GetArgumentData(const int iArg, const int iSkpLen, int *iValue)
		{
			char sValue[64];
			int iReturnValue = GetArgumentData(iArg, iSkpLen, sValue, sizeof(sValue));
			*iValue = atol(sValue);
			return iReturnValue;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CommandLineParser::GetArgumentData(const int iArg, const int iSkpLen, char *sOutBuf, int iMaxSz)
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

		bool CommandLineParser::AddCommand(const char *sCommandLine)
		{
		#ifdef _USE_GLOBAL_MEMPOOL
			this->_sArray = (char **)pMem->ReAllocate(this->_sArray, sizeof(char *), _iCount + 1);
			this->_iLength = (int *)pMem->ReAllocate(this->_iLength, sizeof(int), _iCount + 1);
			this->_sArray[_iCount] = pMem->StrDup(sCommandLine);
			this->_iLength[_iCount] = (int)strlen(this->_sArray[_iCount]);
		#else
			this->_sArray = (char **)realloc(this->_sArray, sizeof(char *) * _iCount + 1);
			this->_iLength = (int *)realloc(this->_iLength, sizeof(int) * _iCount + 1);
			this->_sArray[_iCount] = _strdup(sCommandLine);
			this->_iLength[_iCount] = (int)strlen(this->_sArray[_iCount]);
		#endif

			this->_iCount++;

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CommandLineParser::ParseCmd(char *sCmdLine, int *iOutMaxLen, bool bEval)
		{
			bool bInQuote = false;

			char *sBuf = NULL;

			int iCmdSz = (int)strlen(sCmdLine);
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
				sBuf = (char *)pMem->Allocate(sizeof(char), iMaxLen + 1);
				this->_sArray = (char **)pMem->Allocate(sizeof(char *), _iCount);
				this->_iLength = (int *)pMem->Allocate(sizeof(int), _iCount);
		#else
				this->_sArray = (char **)calloc(sizeof(char *), _iCount);
				sBuf = (char *)calloc(sizeof(char), iMaxLen + 1);
				this->_iLength = (int *)calloc(sizeof(int), _iCount);
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
						this->_sArray[_iCount] = (char *)pMem->Allocate(sizeof(char), iWPos + 1);
		#else
						this->_sArray[_iCount] = (char *)calloc(sizeof(char), iWPos + 1);
		#endif
						strcpy_s(this->_sArray[_iCount], iWPos + 1, sBuf);
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
					this->_sArray[_iCount] = (char *)pMem->Allocate(sizeof(char), iWPos + 1);
		#else
					this->_sArray[_iCount] = (char *)calloc(sizeof(char), iWPos + 1);
		#endif
					strcpy_s(this->_sArray[_iCount], iWPos + 1, sBuf);
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
