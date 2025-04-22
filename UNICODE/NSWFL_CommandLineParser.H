///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_COMMANDLINEPARSER_H_
#define _NSWFL_COMMANDLINEPARSER_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace CommandLine {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class CommandLineParser {
		private:
			WCHAR** _sArray;
			int _iCount;
			int* _iLength;

		public:
			CommandLineParser();
			~CommandLineParser();

			const WCHAR* Item(int iIndex)
			{
				return _sArray[iIndex];
			}

			const int Count()
			{
				return _iCount;
			}

			const int Length(int iIndex)
			{
				return _iLength[iIndex];
			}

			bool AddCommand(const WCHAR* sCommandLine);

			int IsCommand(const int iArg, const WCHAR* sCommandName);
			int IsCommand(const int iArg, const WCHAR* sCommandName, WCHAR* sOutCmdData, int iMaxOutSize);
			int GetArgumentData(const int iArg, const int iSkpLen, WCHAR* sOutBuf, int iMaxSz);
			int GetArgumentData(const int iArg, const int iSkpLen, int* iValue);

		private:
			bool ParseCmd(WCHAR* sCmdLine, int* iOutMaxLen, bool bEval);
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::CommandLine
} //namespace::NSWFL
#endif