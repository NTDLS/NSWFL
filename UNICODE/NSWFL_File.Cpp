///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_FILE_CPP_
#define _NSWFL_FILE_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#include <IO.H>

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initialized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace File {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CorrectReversePath(WCHAR* sPath, int iMaxSz, bool bPossibleNetworkPath)
		{
			int iAlloc = (int)wcslen(sPath) + 1;
			if (iAlloc > 0)
			{
#ifdef _USE_GLOBAL_MEMPOOL
				WCHAR* sAltPath = (WCHAR*)pMem->Allocate(iAlloc, sizeof(WCHAR));
#else
				WCHAR* sAltPath = (WCHAR*)calloc(iAlloc, sizeof(WCHAR));
#endif

				bool bIsNetPath = bPossibleNetworkPath && (_wcsnicmp(sPath, _T("\\\\"), 2) == 0);

				NSWFL::String::ReplaceStrings(sPath, _T("/"), _T("\\"), sAltPath, iAlloc);

				while (NSWFL::String::ReplaceStrings(sAltPath, _T("\\\\"), _T("\\"), sPath, iMaxSz))
				{
					wcscpy_s(sAltPath, iAlloc, sPath);
				}

				if (bIsNetPath)
				{
					wcscpy_s(sAltPath, iAlloc, _T("\\"));
					wcscat_s(sAltPath, iAlloc, sPath);
					wcscpy_s(sPath, iMaxSz, sAltPath);
				}

#ifdef _USE_GLOBAL_MEMPOOL
				pMem->Free(sAltPath);
#else
				free(sAltPath);
#endif
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//This function replaces all ForwardSlashes with BackSlashs and removes duplicates.
		void CorrectReversePath(WCHAR* sPath, int iMaxSz)
		{
			return NSWFL::File::CorrectReversePath(sPath, iMaxSz, false);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//This function replaces all backslashes with ForwardSlashes and removes duplicates.
		void CorrectForwardPath(WCHAR* sPath, int iMaxSz)
		{
#ifdef _USE_GLOBAL_MEMPOOL
			WCHAR* sAltPath = (WCHAR*)pMem->Allocate(iMaxSz + 1, sizeof(WCHAR));
#else
			WCHAR* sAltPath = (WCHAR*)calloc(iMaxSz + 1, sizeof(WCHAR));
#endif

			NSWFL::String::ReplaceStrings(sPath, _T("\\"), _T("/"), sAltPath, iMaxSz + 1);

			while (NSWFL::String::ReplaceStrings(sAltPath, _T("//"), _T("/"), sPath, iMaxSz))
			{
				wcscpy_s(sAltPath, iMaxSz + 1, sPath);
			}

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sAltPath);
#else
			free(sAltPath);
#endif
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CleanFileName(WCHAR* sPath, int iMaxSz)
		{
			int iLength = (int)wcslen(sPath);
			WCHAR* sInvalid = _T("`~!@#$%^&*()=+<>/?;:\"\'[{}]|\\\0");
			for (int iPos = 0; iPos < iLength; iPos++)
			{
				for (int iInv = 0; sInvalid[iInv]; iInv++)
				{
					if (sPath[iPos] == sInvalid[iInv])
					{
						sPath[iPos] = '_';
					}
				}
			}

#ifdef _USE_GLOBAL_MEMPOOL
			WCHAR* sAltPath = (WCHAR*)pMem->Allocate(iMaxSz + 1, sizeof(WCHAR));
#else
			WCHAR* sAltPath = (WCHAR*)calloc(iMaxSz + 1, sizeof(WCHAR));
#endif

			NSWFL::String::ReplaceStrings(sPath, _T("__"), _T("_"), sAltPath, iMaxSz + 1);

			while (NSWFL::String::ReplaceStrings(sAltPath, _T("__"), _T("_"), sPath, iMaxSz))
			{
				wcscpy_s(sAltPath, iMaxSz + 1, sPath);
			}

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sAltPath);
#else
			free(sAltPath);
#endif
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsChildPath(const WCHAR* sInParent, const WCHAR* sInChild)
		{
			WCHAR sParent[MAX_PATH];
			wcscpy_s(sParent, _countof(sParent), sInParent);
			NSWFL::File::CorrectReversePath(sParent, sizeof(sParent));
			int sParentLen = (int)wcslen(sParent);
			if (sParent[sParentLen - 1] == '\\')
			{
				sParent[sParentLen - 1] = '\0';
				sParentLen--;
			}

			WCHAR sChild[MAX_PATH];
			wcscpy_s(sChild, _countof(sChild), sInChild);
			NSWFL::File::CorrectReversePath(sChild, sizeof(sChild));
			int iChildLen = (int)wcslen(sChild);
			if (sChild[iChildLen - 1] == '\\')
			{
				sChild[iChildLen - 1] = '\0';
				iChildLen--;
			}

			if (iChildLen < sParentLen)
			{
				return false;
			}

			for (int iRPos = 0; iRPos < sParentLen; iRPos++)
			{
				if (sInChild[iRPos] != sInParent[iRPos])
				{
					return false;
				}
			}

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int AppendPaths(WCHAR* sTarget, int iMaxTargetSize, const WCHAR* sPart1, const WCHAR* Part2)
		{
			swprintf_s(sTarget, iMaxTargetSize, _T("%s\\%s"), sPart1, Part2);
			NSWFL::File::CorrectReversePath(sTarget, iMaxTargetSize);
			return (int)wcslen(sTarget);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool ArePathsEqual(const WCHAR* sInPath1, const WCHAR* sInPath2)
		{
			WCHAR sPath1[MAX_PATH];
			wcscpy_s(sPath1, _countof(sPath1), sInPath1);
			NSWFL::File::CorrectReversePath(sPath1, _countof(sPath1));
			int iLength1 = (int)wcslen(sPath1);
			if (sPath1[iLength1 - 1] == '\\')
			{
				sPath1[iLength1 - 1] = '\0';
			}

			WCHAR sPath2[MAX_PATH];
			wcscpy_s(sPath2, _countof(sPath2), sInPath2);
			NSWFL::File::CorrectReversePath(sPath2, _countof(sPath2));
			int iLength2 = (int)wcslen(sPath2);
			if (sPath2[iLength2 - 1] == '\\')
			{
				sPath2[iLength2 - 1] = '\0';
			}

			return (NSWFL::String::StrEqlI(sPath1, sPath2));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GetFileDrive(const WCHAR* sFileNameAndPath, WCHAR* sOutDrive, int iMaxSz)
		{
			return(_wsplitpath_s(sFileNameAndPath,
				sOutDrive, iMaxSz,
				NULL, NULL,
				NULL, NULL,
				NULL, NULL) == 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GetModuleFilePath(WCHAR* sOutPath, int iMaxSz)
		{
			WCHAR fileandPath[MAX_PATH];
			memset(fileandPath, 0, sizeof(fileandPath));
			if (GetModuleFileName(NULL, fileandPath, sizeof(fileandPath)) > 0)
			{
				return GetFilePath(fileandPath, sOutPath, iMaxSz);
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GetFilePath(const WCHAR* sFileNameAndPath, WCHAR* sOutPath, int iMaxSz)
		{
			WCHAR sDrive[MAX_PATH];
			WCHAR sDir[MAX_PATH];

			_wsplitpath_s(sFileNameAndPath,
				sDrive, _countof(sDrive),
				sDir, _countof(sDir),
				NULL, NULL,
				NULL, NULL);

#ifdef _MSC_VER
			return(_wmakepath_s(sOutPath, iMaxSz, sDrive, sDir, NULL, NULL) == 0);
#else
			_wmakepath_s(sOutPath, iMaxSz, sDrive, sDir, NULL, NULL);
			return(true);
#endif
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GetFileName(const WCHAR* sFileNameAndPath, WCHAR* sOutPath, int iMaxSz)
		{
			WCHAR sFile[MAX_PATH];
			WCHAR sExt[MAX_PATH];

			_wsplitpath_s(sFileNameAndPath,
				NULL, NULL,
				NULL, NULL,
				sFile, _countof(sFile),
				sExt, _countof(sExt));

#ifdef _MSC_VER
			return(_wmakepath_s(sOutPath, iMaxSz, NULL, NULL, sFile, sExt) == 0);
#else
			_wmakepath_s(sOutPath, iMaxSz, NULL, NULL, sFile, sExt);
			return(true);
#endif
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GetFileNameOnly(const WCHAR* sFileNameAndPath, WCHAR* sOutFile, int iMaxSz)
		{
#ifdef _MSC_VER
			return(_wsplitpath_s(sFileNameAndPath, NULL, NULL, NULL, NULL, sOutFile, iMaxSz, NULL, NULL) == 0);
#else
			_wsplitpath_s(sFileNameAndPath, NULL, NULL, NULL, NULL, sOutFile, iMaxSz, NULL, NULL);
			return(true);
#endif	
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GetFileExtension(const WCHAR* sFileNameAndPath, WCHAR* sOutExt, int iMaxSz)
		{
#ifdef _MSC_VER
			return(_wsplitpath_s(sFileNameAndPath, NULL, NULL, NULL, NULL, NULL, NULL, sOutExt, iMaxSz) == 0);
#else
			_wsplitpath_s(sFileNameAndPath, NULL, NULL, NULL, NULL, NULL, NULL, sOutExt, iMaxSz);
			return(true);
#endif	
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_DesktopDirectory(WCHAR* sOut, DWORD dwMaxOut)
		{
			memset(sOut, 0, dwMaxOut);

			if (NSWFL::Registry::Get_StringRegistryValue(HKEY_CURRENT_USER, SHELL_FOLDERS_KEY, _T("Desktop"), sOut, dwMaxOut))
			{
				return (int)wcslen(sOut);
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_DocumentsDirectory(WCHAR* sOut, DWORD dwMaxOut)
		{
			memset(sOut, 0, dwMaxOut);

			//FIXFIX: May want to change this to: SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, localAppPath);

			if (NSWFL::Registry::Get_StringRegistryValue(HKEY_CURRENT_USER, SHELL_FOLDERS_KEY, _T("Personal"), sOut, dwMaxOut))
			{
				return (int)wcslen(sOut);
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_VideosDirectory(WCHAR* sOut, DWORD dwMaxOut)
		{
			memset(sOut, 0, dwMaxOut);

			if (NSWFL::Registry::Get_StringRegistryValue(HKEY_CURRENT_USER, SHELL_FOLDERS_KEY, _T("My Video"), sOut, dwMaxOut))
			{
				return (int)wcslen(sOut);
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_MusicDirectory(WCHAR* sOut, DWORD dwMaxOut)
		{
			memset(sOut, 0, dwMaxOut);

			if (NSWFL::Registry::Get_StringRegistryValue(HKEY_CURRENT_USER, SHELL_FOLDERS_KEY, _T("My Music"), sOut, dwMaxOut))
			{
				return (int)wcslen(sOut);
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_PicturesDirectory(WCHAR* sOut, DWORD dwMaxOut)
		{
			memset(sOut, 0, dwMaxOut);

			if (NSWFL::Registry::Get_StringRegistryValue(HKEY_CURRENT_USER, SHELL_FOLDERS_KEY, _T("My Pictures"), sOut, dwMaxOut))
			{
				return (int)wcslen(sOut);
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_CurrentDirectory(WCHAR* sOutVal, int iMaxSz)
		{
			return GetCurrentDirectory(iMaxSz, sOutVal);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_TempDirectory(WCHAR* sOutVal, int iMaxSz)
		{
			return GetTempPath(iMaxSz, sOutVal);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_WindowsDirectory(WCHAR* sOutVal, int iMaxSz)
		{
			return GetWindowsDirectory(sOutVal, iMaxSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_SystemDirectory(WCHAR* sOutVal, int iMaxSz)
		{
			return GetSystemDirectory(sOutVal, iMaxSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FileSizeFriendly(int iFileSize, WCHAR* sOut, int iMaxOutSz)
		{
			return NSWFL::File::FileSizeFriendly((__int64)iFileSize, sOut, iMaxOutSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FileSizeFriendly(int iFileSize, int iDecimalPlaces, WCHAR* sOut, int iMaxOutSz)
		{
			return NSWFL::File::FileSizeFriendly((__int64)iFileSize, iDecimalPlaces, sOut, iMaxOutSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FileSizeFriendly(unsigned __int64 i64FileSize, WCHAR* sOut, int iMaxOutSz)
		{
			return NSWFL::File::FileSizeFriendly(i64FileSize, -2, sOut, iMaxOutSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FileSizeFriendly(__int64 i64FileSize, WCHAR* sOut, int iMaxOutSz)
		{
			return NSWFL::File::FileSizeFriendly(i64FileSize, -2, sOut, iMaxOutSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FileSizeFriendly(__int64 i64FileSize, int iDecimalPlaces, WCHAR* sOut, int iMaxOutSz)
		{
			bool bAllowChange = false;

			if (iDecimalPlaces < 0)
			{
				bAllowChange = true;
				iDecimalPlaces = (iDecimalPlaces * -1);
			}

			__int64 i64DivideBy = 1;

			WCHAR* sSzStr = _T("");
			WCHAR sFormatted[128];
			WCHAR sFormat[10];

			if (i64FileSize >= ExaByte || i64FileSize <= -ExaByte) {
				i64DivideBy = ExaByte;
				sSzStr = _T("EB");
			}
			else if (i64FileSize >= PetaByte || i64FileSize <= -PetaByte) {
				i64DivideBy = PetaByte;
				sSzStr = _T("PB");
			}
			else if (i64FileSize >= TeraByte || i64FileSize <= -TeraByte) {
				i64DivideBy = TeraByte;
				sSzStr = _T("TB");
			}
			else if (i64FileSize >= GigaByte || i64FileSize <= -GigaByte) {
				i64DivideBy = GigaByte;
				sSzStr = _T("GB");
			}
			else if (i64FileSize >= MegaByte || i64FileSize <= -MegaByte) {
				i64DivideBy = MegaByte;
				sSzStr = _T("MB");
			}
			else if (i64FileSize >= KiloByte || i64FileSize <= -KiloByte) {
				i64DivideBy = KiloByte;
				sSzStr = _T("KB");
			}
			else {
				i64DivideBy = 1;
				if (bAllowChange)
				{
					iDecimalPlaces = 0;
				}
				sSzStr = _T("B");
			}

			swprintf_s(sFormat, _countof(sFormat), _T("%%.%df"), iDecimalPlaces);
			swprintf_s(sOut, iMaxOutSz, sFormat, (double)i64FileSize / i64DivideBy);

			NSWFL::Conversion::FormatFloatString(sFormatted, sizeof(sFormatted), sOut);

			swprintf_s(sOut, iMaxOutSz, _T("%s %s"), sFormatted, sSzStr);

			return sOut;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FileSizeFriendly(unsigned __int64 i64FileSize, int iDecimalPlaces, WCHAR* sOut, int iMaxOutSz)
		{
			bool bAllowChange = false;

			if (iDecimalPlaces < 0)
			{
				bAllowChange = true;
				iDecimalPlaces = (iDecimalPlaces * -1);
			}

			__int64 i64DivideBy = 1;

			WCHAR* sSzStr = _T("");
			WCHAR sFormatted[128];
			WCHAR sFormat[10];

			if (i64FileSize >= ExaByte) {
				i64DivideBy = ExaByte;
				sSzStr = _T("EB");
			}
			else if (i64FileSize >= PetaByte) {
				i64DivideBy = PetaByte;
				sSzStr = _T("PB");
			}
			else if (i64FileSize >= TeraByte) {
				i64DivideBy = TeraByte;
				sSzStr = _T("TB");
			}
			else if (i64FileSize >= GigaByte) {
				i64DivideBy = GigaByte;
				sSzStr = _T("GB");
			}
			else if (i64FileSize >= MegaByte) {
				i64DivideBy = MegaByte;
				sSzStr = _T("MB");
			}
			else if (i64FileSize >= KiloByte) {
				i64DivideBy = KiloByte;
				sSzStr = _T("KB");
			}
			else {
				i64DivideBy = 1;
				if (bAllowChange)
				{
					iDecimalPlaces = 0;
				}
				sSzStr = _T("B");
			}

			swprintf_s(sFormat, _countof(sFormat), _T("%%.%df"), iDecimalPlaces);
			swprintf_s(sOut, iMaxOutSz, sFormat, (double)i64FileSize / i64DivideBy);

			NSWFL::Conversion::FormatFloatString(sFormatted, sizeof(sFormatted), sOut);

			swprintf_s(sOut, iMaxOutSz, _T("%s %s"), sFormatted, sSzStr);

			return sOut;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FileSizeFriendly(double dFileSize, WCHAR* sOut, int iMaxOutSz)
		{
			return NSWFL::File::FileSizeFriendly(dFileSize, 2, sOut, iMaxOutSz);
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* FileSizeFriendly(double dFileSize, int iDecimalPlaces, WCHAR* sOut, int iMaxOutSz)
		{
			bool bAllowChange = false;

			if (iDecimalPlaces < 0)
			{
				bAllowChange = true;
				iDecimalPlaces = (iDecimalPlaces * -1);
			}

			double dDivideBy = 1;

			WCHAR* sSzStr = _T("");
			WCHAR sFormatted[128];
			WCHAR sFormat[10];

			if (NSWFL::Math::WithinPercent(dFileSize, 99, PetaByte) || NSWFL::Math::WithinPercent(dFileSize, 99, -PetaByte)) {
				dDivideBy = PetaByte;
				sSzStr = _T("PB");
			}
			else if (NSWFL::Math::WithinPercent(dFileSize, 99, TeraByte) || NSWFL::Math::WithinPercent(dFileSize, 99, -TeraByte)) {
				dDivideBy = TeraByte;
				sSzStr = _T("TB");
			}
			else if (NSWFL::Math::WithinPercent(dFileSize, 99, GigaByte) || NSWFL::Math::WithinPercent(dFileSize, 99, -GigaByte)) {
				dDivideBy = GigaByte;
				sSzStr = _T("GB");
			}
			else if (NSWFL::Math::WithinPercent(dFileSize, 99, MegaByte) || NSWFL::Math::WithinPercent(dFileSize, 99, -MegaByte)) {
				dDivideBy = MegaByte;
				sSzStr = _T("MB");
			}
			else if (NSWFL::Math::WithinPercent(dFileSize, 99, KiloByte) || NSWFL::Math::WithinPercent(dFileSize, 99, -KiloByte)) {
				dDivideBy = KiloByte;
				sSzStr = _T("KB");
			}
			else {
				dDivideBy = 1.0;
				if (bAllowChange)
				{
					iDecimalPlaces = 0;
				}
				sSzStr = _T("B");
			}

			swprintf_s(sFormat, _countof(sFormat), _T("%%.%df"), iDecimalPlaces);
			swprintf_s(sOut, iMaxOutSz, sFormat, dFileSize / dDivideBy);

			NSWFL::Conversion::FormatFloatString(sFormatted, sizeof(sFormatted), sOut);

			swprintf_s(sOut, iMaxOutSz, _T("%s %s"), sFormatted, sSzStr);

			return sOut;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			FILE_BEGIN 0
				The starting point is zero or the beginning of the file.
				If this flag is specified, then the liDistanceToMove parameter
					is interpreted as an unsigned value.

			FILE_CURRENT 1
				The start point is the current value of the file pointer.

			FILE_END 2
				The starting point is the current end-of-file position.
		*/
		__int64 FileSeek(HANDLE hFile, __int64 l64Distance, DWORD dwMoveMethod)
		{
			LARGE_INTEGER liDistance;
			LARGE_INTEGER liNewPosition;

			memset(&liDistance, 0, sizeof(liDistance));
			memset(&liNewPosition, 0, sizeof(liNewPosition));

			liDistance.QuadPart = l64Distance;

			if (!SetFilePointerEx(hFile, liDistance, &liNewPosition, dwMoveMethod))
			{
				//Fix me: Need to handle this error here.
			}

			return liNewPosition.QuadPart;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Returns the current file marker position.
		*/
		__int64 FileSeek(HANDLE hFile)
		{
			return FileSeek(hFile, 0, FILE_CURRENT);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Valid Modes:
				FAccess  - Check for read and write permission
				FRead    - Check for read permission
				FWrite   - Check for write permission
				FExecute - Execute (Ignored)
				FExist   - Check for existence of file
		*/
		bool FileAccess(const WCHAR* sFileName, short iMode)
		{
			if (_waccess(sFileName, iMode) == 0)
			{
				return true;
			}

			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			WARNING! - Will not handle files over 4GB.
		*/
		bool FileSize(const WCHAR* sFileName, unsigned long* lFileSize)
		{
			HANDLE hfSource = CreateFile(sFileName, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hfSource != INVALID_HANDLE_VALUE)
			{
				bool bResult = NSWFL::File::FileSize(hfSource, lFileSize);
				CloseHandle(hfSource);
				return bResult;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			WARNING! - Will not handle files over 4GB.
		*/
		bool FileSize(HANDLE hFile, unsigned long* lFileSize)
		{
			LARGE_INTEGER liFileSize;
			memset(&liFileSize, 0, sizeof(liFileSize));

#ifdef _MSC_VER
			if (!GetFileSizeEx(hFile, &liFileSize))
			{
				return false;
			}

			if (liFileSize.HighPart != 0)
			{
				return false;
			}

			*lFileSize = liFileSize.LowPart;
			return true;
#else
			if (FileSize(hFile, &liFileSize.QuadPart))
			{
				if (liFileSize.HighPart != 0)
				{
					return false;
				}
				return true;
			}

			return false;
#endif
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool FileSize(const WCHAR* sFileName, int* iFileSize)
		{
			HANDLE hfSource = CreateFile(sFileName, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			__int64 i64FileSize = 0;
			bool bResult = NSWFL::File::FileSize(hfSource, &i64FileSize);
			*iFileSize = (int)i64FileSize;
			CloseHandle(hfSource);
			return bResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool FileSize(const WCHAR* sFileName, __int64* i64FileSize)
		{
			HANDLE hfSource = CreateFile(sFileName, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			bool bResult = NSWFL::File::FileSize(hfSource, i64FileSize);
			CloseHandle(hfSource);
			return bResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool FileSize(HANDLE hFile, __int64* i64FileSize)
		{
#ifdef _MSC_VER
			LARGE_INTEGER liFileSize;
			memset(&liFileSize, 0, sizeof(liFileSize));

			if (!GetFileSizeEx(hFile, &liFileSize))
			{
				return false;
			}

			*i64FileSize = liFileSize.QuadPart;
			return true;
#else
			LARGE_INTEGER liFileSize;
			memset(&liFileSize, 0, sizeof(liFileSize));

			if ((liFileSize.LowPart = (DWORD)GetFileSize(hFile, (DWORD*)&liFileSize.HighPart)) == INVALID_FILE_SIZE)
			{
				return false;
			}

			*i64FileSize = liFileSize.QuadPart;

			return true;
#endif
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool BufferDataToFile(const WCHAR* sFile, const WCHAR* sBuf, const unsigned long ulBufSz)
		{
			WCHAR sBuffer[FILECHUNKSIZE + 1];

			unsigned long ulRPos = 0;
			unsigned long ulWPos = 0;

			FILE* hTarget = NULL;

			if (_wfopen_s(&hTarget, sFile, _T("wb")) != 0)
			{
				return false;
			}

			while (ulRPos < ulBufSz)
			{
				ulWPos = 0;

				//Fill our write buffer.
				while (ulWPos < FILECHUNKSIZE && ulRPos < ulBufSz)
				{
					sBuffer[ulWPos++] = sBuf[ulRPos++];
				}

				//If we have data.
				if (ulWPos > 0)
				{
					//Write the data.
					fwrite(sBuffer, sizeof(WCHAR), ulWPos, hTarget);
				}
			}

			fclose(hTarget);

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool DeleteFolderStructure(const WCHAR* sPath)
		{
			WCHAR sDir[MAX_PATH];
			int iNameSz = (int)wcslen(sPath);
			int iPos = 0;

			for (iPos = 0; iPos < iNameSz; iPos++)
			{
				if (sPath[iPos] == '\\' || sPath[iPos] == '/')
				{
					sDir[iPos] = '\0';
					if (RemoveDirectory(sDir) < 1)
					{
						return false;
					}
				}

				sDir[iPos] = sPath[iPos];
			}

			sDir[iPos] = '\0';
			return(RemoveDirectory(sDir) > 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CreateFolderStructure(const WCHAR* sPath)
		{
			WCHAR sDir[MAX_PATH];

			int iNameSz = (int)wcslen(sPath);
			int iPos = 0;

			for (iPos = 0; iPos < iNameSz; iPos++)
			{
				if (sPath[iPos] == '\\' || sPath[iPos] == '/')
				{
					sDir[iPos] = '\0';
					if (!NSWFL::File::IsDirectory(sDir))
					{
						if (!CreateDirectory(sDir, NULL))
						{
							return false;
						}
					}
				}

				sDir[iPos] = sPath[iPos];
			}

			sDir[iPos] = '\0';
			if (!NSWFL::File::IsDirectory(sDir))
			{
				if (!CreateDirectory(sDir, NULL))
				{
					return false;
				}
			}
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsDirectory(const WCHAR* sPath)
		{
			int Attrib = GetFileAttributes(sPath);
			if (Attrib != 0xFFFFFFFF)
			{
				if (Attrib & FILE_ATTRIBUTE_DIRECTORY)
				{
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool TruncateFile(HANDLE hFile, __int64 i64Size)
		{
			FileSeek(hFile, i64Size, FILE_BEGIN);
			return(SetEndOfFile(hFile) > 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool TruncateFile(const WCHAR* sFile, __int64 i64Size)
		{
			HANDLE hFile = CreateFile(
				sFile,
				GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			bool bResult = NSWFL::File::TruncateFile(hFile, i64Size);

			CloseHandle(hFile);

			return bResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WCHAR* MakeAttributeString(DWORD dwAttribs, WCHAR* sOut, int iMaxSz)
		{
			wcscpy_s(sOut, iMaxSz, _T(""));

			if (dwAttribs & FILE_ATTRIBUTE_ARCHIVE)
			{
				wcscat_s(sOut, iMaxSz, _T("A "));
			}

			if (dwAttribs & FILE_ATTRIBUTE_HIDDEN)
			{
				wcscat_s(sOut, iMaxSz, _T("H "));
			}

			if (dwAttribs & FILE_ATTRIBUTE_READONLY)
			{
				wcscat_s(sOut, iMaxSz, _T("R "));
			}

			if (dwAttribs & FILE_ATTRIBUTE_SYSTEM)
			{
				wcscat_s(sOut, iMaxSz, _T("S "));
			}

			int iLen = (int)wcslen(sOut);

			if (iLen > 0)
			{
				if (sOut[iLen - 1] == ' ')
				{
					sOut[iLen - 1] = '\0';
				}
			}

			return sOut;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::File
} //namespace::NSWFL
#endif
