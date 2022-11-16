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
			char **_sArray;
			int _iCount;
			int *_iLength;

		public:
			CommandLineParser();
			~CommandLineParser();

			const char *Item(int iIndex)
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

			bool AddCommand(const char *sCommandLine);

			int IsCommand(const int iArg, const char *sCommandName);
			int IsCommand(const int iArg, const char *sCommandName, char *sOutCmdData, int iMaxOutSize);
			int GetArgumentData(const int iArg, const int iSkpLen, char *sOutBuf, int iMaxSz);
			int GetArgumentData(const int iArg, const int iSkpLen, int *iValue);

		private:
			bool ParseCmd(char *sCmdLine, int *iOutMaxLen, bool bEval);
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::CommandLine
} //namespace::NSWFL
#endif