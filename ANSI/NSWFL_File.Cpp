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

		void CorrectReversePath(char* sPath, int iMaxSz, bool bPossibleNetworkPath)
		{
			int iAlloc = (int)strlen(sPath) + 1;
			if (iAlloc > 0)
			{
#ifdef _USE_GLOBAL_MEMPOOL
				char* sAltPath = (char*)pMem->Allocate(iAlloc, sizeof(char));
#else
				char* sAltPath = (char*)calloc(iAlloc, sizeof(char));
#endif

				bool bIsNetPath = bPossibleNetworkPath && (_strnicmp(sPath, "\\\\", 2) == 0);

				NSWFL::String::ReplaceStrings(sPath, "/", "\\", sAltPath, iAlloc);

				while (NSWFL::String::ReplaceStrings(sAltPath, "\\\\", "\\", sPath, iMaxSz))
				{
					strcpy_s(sAltPath, iAlloc, sPath);
				}

				if (bIsNetPath)
				{
					strcpy_s(sAltPath, iAlloc, "\\");
					strcat_s(sAltPath, iAlloc, sPath);
					strcpy_s(sPath, iMaxSz, sAltPath);
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
		void CorrectReversePath(char* sPath, int iMaxSz)
		{
			return NSWFL::File::CorrectReversePath(sPath, iMaxSz, false);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//This function replaces all backslashes with ForwardSlashes and removes duplicates.
		void CorrectForwardPath(char* sPath, int iMaxSz)
		{
#ifdef _USE_GLOBAL_MEMPOOL
			char* sAltPath = (char*)pMem->Allocate(iMaxSz + 1, sizeof(char));
#else
			char* sAltPath = (char*)calloc(iMaxSz + 1, sizeof(char));
#endif

			NSWFL::String::ReplaceStrings(sPath, "\\", "/", sAltPath, iMaxSz + 1);

			while (NSWFL::String::ReplaceStrings(sAltPath, "//", "/", sPath, iMaxSz))
			{
				strcpy_s(sAltPath, iMaxSz + 1, sPath);
			}

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sAltPath);
#else
			free(sAltPath);
#endif
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void CleanFileName(char* sPath, int iMaxSz)
		{
			int iLength = (int)strlen(sPath);
			char* sInvalid = "`~!@#$%^&*()=+<>/?;:\"\'[{}]|\\\0";
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
			char* sAltPath = (char*)pMem->Allocate(iMaxSz + 1, sizeof(char));
#else
			char* sAltPath = (char*)calloc(iMaxSz + 1, sizeof(char));
#endif

			NSWFL::String::ReplaceStrings(sPath, "__", "_", sAltPath, iMaxSz + 1);

			while (NSWFL::String::ReplaceStrings(sAltPath, "__", "_", sPath, iMaxSz))
			{
				strcpy_s(sAltPath, iMaxSz + 1, sPath);
			}

#ifdef _USE_GLOBAL_MEMPOOL
			pMem->Free(sAltPath);
#else
			free(sAltPath);
#endif
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool IsChildPath(const char* sInParent, const char* sInChild)
		{
			char sParent[MAX_PATH];
			strcpy_s(sParent, sizeof(sParent), sInParent);
			NSWFL::File::CorrectReversePath(sParent, sizeof(sParent));
			int sParentLen = (int)strlen(sParent);
			if (sParent[sParentLen - 1] == '\\')
			{
				sParent[sParentLen - 1] = '\0';
				sParentLen--;
			}

			char sChild[MAX_PATH];
			strcpy_s(sChild, sizeof(sChild), sInChild);
			NSWFL::File::CorrectReversePath(sChild, sizeof(sChild));
			int iChildLen = (int)strlen(sChild);
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

		int AppendPaths(char* sTarget, int iMaxTargetSize, const char* sPart1, const char* Part2)
		{
			sprintf_s(sTarget, iMaxTargetSize, "%s\\%s", sPart1, Part2);
			NSWFL::File::CorrectReversePath(sTarget, iMaxTargetSize);
			return (int)strlen(sTarget);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool ArePathsEqual(const char* sInPath1, const char* sInPath2)
		{
			char sPath1[MAX_PATH];
			strcpy_s(sPath1, sizeof(sPath1), sInPath1);
			NSWFL::File::CorrectReversePath(sPath1, sizeof(sPath1));
			int iLength1 = (int)strlen(sPath1);
			if (sPath1[iLength1 - 1] == '\\')
			{
				sPath1[iLength1 - 1] = '\0';
			}

			char sPath2[MAX_PATH];
			strcpy_s(sPath2, sizeof(sPath2), sInPath2);
			NSWFL::File::CorrectReversePath(sPath2, sizeof(sPath2));
			int iLength2 = (int)strlen(sPath2);
			if (sPath2[iLength2 - 1] == '\\')
			{
				sPath2[iLength2 - 1] = '\0';
			}

			return (NSWFL::String::StrEqlI(sPath1, sPath2));
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GetFileDrive(const char* sFileNameAndPath, char* sOutDrive, int iMaxSz)
		{
			return(_splitpath_s(sFileNameAndPath,
				sOutDrive, iMaxSz,
				NULL, NULL,
				NULL, NULL,
				NULL, NULL) == 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GetModuleFilePath(char* sOutPath, int iMaxSz)
		{
			char fileandPath[MAX_PATH];
			memset(fileandPath, 0, sizeof(fileandPath));
			if (GetModuleFileName(NULL, fileandPath, sizeof(fileandPath)) > 0)
			{
				return GetFilePath(fileandPath, sOutPath, iMaxSz);
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GetFilePath(const char* sFileNameAndPath, char* sOutPath, int iMaxSz)
		{
			char sDrive[MAX_PATH];
			char sDir[MAX_PATH];

			_splitpath_s(sFileNameAndPath,
				sDrive, sizeof(sDrive),
				sDir, sizeof(sDir),
				NULL, NULL,
				NULL, NULL);

#ifdef _MSC_VER
			return(_makepath_s(sOutPath, iMaxSz, sDrive, sDir, NULL, NULL) == 0);
#else
			_makepath_s(sOutPath, iMaxSz, sDrive, sDir, NULL, NULL);
			return(true);
#endif
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GetFileName(const char* sFileNameAndPath, char* sOutPath, int iMaxSz)
		{
			char sFile[MAX_PATH];
			char sExt[MAX_PATH];

			_splitpath_s(sFileNameAndPath,
				NULL, NULL,
				NULL, NULL,
				sFile, sizeof(sFile),
				sExt, sizeof(sExt));

#ifdef _MSC_VER
			return(_makepath_s(sOutPath, iMaxSz, NULL, NULL, sFile, sExt) == 0);
#else
			_makepath_s(sOutPath, iMaxSz, NULL, NULL, sFile, sExt);
			return(true);
#endif
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GetFileNameOnly(const char* sFileNameAndPath, char* sOutFile, int iMaxSz)
		{
#ifdef _MSC_VER
			return(_splitpath_s(sFileNameAndPath, NULL, NULL, NULL, NULL, sOutFile, iMaxSz, NULL, NULL) == 0);
#else
			_splitpath_s(sFileNameAndPath, NULL, NULL, NULL, NULL, sOutFile, iMaxSz, NULL, NULL);
			return(true);
#endif	
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool GetFileExtension(const char* sFileNameAndPath, char* sOutExt, int iMaxSz)
		{
#ifdef _MSC_VER
			return(_splitpath_s(sFileNameAndPath, NULL, NULL, NULL, NULL, NULL, NULL, sOutExt, iMaxSz) == 0);
#else
			_splitpath_s(sFileNameAndPath, NULL, NULL, NULL, NULL, NULL, NULL, sOutExt, iMaxSz);
			return(true);
#endif	
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_DesktopDirectory(char* sOut, DWORD dwMaxOut)
		{
			memset(sOut, 0, dwMaxOut);

			if (NSWFL::Registry::Get_StringRegistryValue(HKEY_CURRENT_USER, SHELL_FOLDERS_KEY, "Desktop", sOut, dwMaxOut))
			{
				return (int)strlen(sOut);
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_DocumentsDirectory(char* sOut, DWORD dwMaxOut)
		{
			memset(sOut, 0, dwMaxOut);

			//FIXFIX: May want to change this to: SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, localAppPath);

			if (NSWFL::Registry::Get_StringRegistryValue(HKEY_CURRENT_USER, SHELL_FOLDERS_KEY, "Personal", sOut, dwMaxOut))
			{
				return (int)strlen(sOut);
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_VideosDirectory(char* sOut, DWORD dwMaxOut)
		{
			memset(sOut, 0, dwMaxOut);

			if (NSWFL::Registry::Get_StringRegistryValue(HKEY_CURRENT_USER, SHELL_FOLDERS_KEY, "My Video", sOut, dwMaxOut))
			{
				return (int)strlen(sOut);
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_MusicDirectory(char* sOut, DWORD dwMaxOut)
		{
			memset(sOut, 0, dwMaxOut);

			if (NSWFL::Registry::Get_StringRegistryValue(HKEY_CURRENT_USER, SHELL_FOLDERS_KEY, "My Music", sOut, dwMaxOut))
			{
				return (int)strlen(sOut);
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_PicturesDirectory(char* sOut, DWORD dwMaxOut)
		{
			memset(sOut, 0, dwMaxOut);

			if (NSWFL::Registry::Get_StringRegistryValue(HKEY_CURRENT_USER, SHELL_FOLDERS_KEY, "My Pictures", sOut, dwMaxOut))
			{
				return (int)strlen(sOut);
			}

			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_CurrentDirectory(char* sOutVal, int iMaxSz)
		{
			return GetCurrentDirectory(iMaxSz, sOutVal);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_TempDirectory(char* sOutVal, int iMaxSz)
		{
			return GetTempPath(iMaxSz, sOutVal);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_WindowsDirectory(char* sOutVal, int iMaxSz)
		{
			return GetWindowsDirectory(sOutVal, iMaxSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DWORD Get_SystemDirectory(char* sOutVal, int iMaxSz)
		{
			return GetSystemDirectory(sOutVal, iMaxSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char* FileSizeFriendly(int iFileSize, char* sOut, int iMaxOutSz)
		{
			return NSWFL::File::FileSizeFriendly((__int64)iFileSize, sOut, iMaxOutSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char* FileSizeFriendly(int iFileSize, int iDecimalPlaces, char* sOut, int iMaxOutSz)
		{
			return NSWFL::File::FileSizeFriendly((__int64)iFileSize, iDecimalPlaces, sOut, iMaxOutSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char* FileSizeFriendly(unsigned __int64 i64FileSize, char* sOut, int iMaxOutSz)
		{
			return NSWFL::File::FileSizeFriendly(i64FileSize, -2, sOut, iMaxOutSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char* FileSizeFriendly(__int64 i64FileSize, char* sOut, int iMaxOutSz)
		{
			return NSWFL::File::FileSizeFriendly(i64FileSize, -2, sOut, iMaxOutSz);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char* FileSizeFriendly(__int64 i64FileSize, int iDecimalPlaces, char* sOut, int iMaxOutSz)
		{
			bool bAllowChange = false;

			if (iDecimalPlaces < 0)
			{
				bAllowChange = true;
				iDecimalPlaces = (iDecimalPlaces * -1);
			}

			__int64 i64DivideBy = 1;

			char* sSzStr = "";
			char sFormatted[128];
			char sFormat[10];

			if (i64FileSize >= ExaByte || i64FileSize <= -ExaByte) {
				i64DivideBy = ExaByte;
				sSzStr = "EB";
			}
			else if (i64FileSize >= PetaByte || i64FileSize <= -PetaByte) {
				i64DivideBy = PetaByte;
				sSzStr = "PB";
			}
			else if (i64FileSize >= TeraByte || i64FileSize <= -TeraByte) {
				i64DivideBy = TeraByte;
				sSzStr = "TB";
			}
			else if (i64FileSize >= GigaByte || i64FileSize <= -GigaByte) {
				i64DivideBy = GigaByte;
				sSzStr = "GB";
			}
			else if (i64FileSize >= MegaByte || i64FileSize <= -MegaByte) {
				i64DivideBy = MegaByte;
				sSzStr = "MB";
			}
			else if (i64FileSize >= KiloByte || i64FileSize <= -KiloByte) {
				i64DivideBy = KiloByte;
				sSzStr = "KB";
			}
			else {
				i64DivideBy = 1;
				if (bAllowChange)
				{
					iDecimalPlaces = 0;
				}
				sSzStr = "B";
			}

			sprintf_s(sFormat, sizeof(sFormat), "%%.%df", iDecimalPlaces);
			sprintf_s(sOut, iMaxOutSz, sFormat, (double)i64FileSize / i64DivideBy);

			NSWFL::Conversion::FormatFloatString(sFormatted, sizeof(sFormatted), sOut);

			sprintf_s(sOut, iMaxOutSz, "%s %s", sFormatted, sSzStr);

			return sOut;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char* FileSizeFriendly(unsigned __int64 i64FileSize, int iDecimalPlaces, char* sOut, int iMaxOutSz)
		{
			bool bAllowChange = false;

			if (iDecimalPlaces < 0)
			{
				bAllowChange = true;
				iDecimalPlaces = (iDecimalPlaces * -1);
			}

			__int64 i64DivideBy = 1;

			char* sSzStr = "";
			char sFormatted[128];
			char sFormat[10];

			if (i64FileSize >= ExaByte) {
				i64DivideBy = ExaByte;
				sSzStr = "EB";
			}
			else if (i64FileSize >= PetaByte) {
				i64DivideBy = PetaByte;
				sSzStr = "PB";
			}
			else if (i64FileSize >= TeraByte) {
				i64DivideBy = TeraByte;
				sSzStr = "TB";
			}
			else if (i64FileSize >= GigaByte) {
				i64DivideBy = GigaByte;
				sSzStr = "GB";
			}
			else if (i64FileSize >= MegaByte) {
				i64DivideBy = MegaByte;
				sSzStr = "MB";
			}
			else if (i64FileSize >= KiloByte) {
				i64DivideBy = KiloByte;
				sSzStr = "KB";
			}
			else {
				i64DivideBy = 1;
				if (bAllowChange)
				{
					iDecimalPlaces = 0;
				}
				sSzStr = "B";
			}

			sprintf_s(sFormat, sizeof(sFormat), "%%.%df", iDecimalPlaces);
			sprintf_s(sOut, iMaxOutSz, sFormat, (double)i64FileSize / i64DivideBy);

			NSWFL::Conversion::FormatFloatString(sFormatted, sizeof(sFormatted), sOut);

			sprintf_s(sOut, iMaxOutSz, "%s %s", sFormatted, sSzStr);

			return sOut;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char* FileSizeFriendly(double dFileSize, char* sOut, int iMaxOutSz)
		{
			return NSWFL::File::FileSizeFriendly(dFileSize, 2, sOut, iMaxOutSz);
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		char* FileSizeFriendly(double dFileSize, int iDecimalPlaces, char* sOut, int iMaxOutSz)
		{
			bool bAllowChange = false;

			if (iDecimalPlaces < 0)
			{
				bAllowChange = true;
				iDecimalPlaces = (iDecimalPlaces * -1);
			}

			double dDivideBy = 1;

			char* sSzStr = "";
			char sFormatted[128];
			char sFormat[10];

			if (NSWFL::Math::WithinPercent(dFileSize, 99, PetaByte) || NSWFL::Math::WithinPercent(dFileSize, 99, -PetaByte)) {
				dDivideBy = PetaByte;
				sSzStr = "PB";
			}
			else if (NSWFL::Math::WithinPercent(dFileSize, 99, TeraByte) || NSWFL::Math::WithinPercent(dFileSize, 99, -TeraByte)) {
				dDivideBy = TeraByte;
				sSzStr = "TB";
			}
			else if (NSWFL::Math::WithinPercent(dFileSize, 99, GigaByte) || NSWFL::Math::WithinPercent(dFileSize, 99, -GigaByte)) {
				dDivideBy = GigaByte;
				sSzStr = "GB";
			}
			else if (NSWFL::Math::WithinPercent(dFileSize, 99, MegaByte) || NSWFL::Math::WithinPercent(dFileSize, 99, -MegaByte)) {
				dDivideBy = MegaByte;
				sSzStr = "MB";
			}
			else if (NSWFL::Math::WithinPercent(dFileSize, 99, KiloByte) || NSWFL::Math::WithinPercent(dFileSize, 99, -KiloByte)) {
				dDivideBy = KiloByte;
				sSzStr = "KB";
			}
			else {
				dDivideBy = 1.0;
				if (bAllowChange)
				{
					iDecimalPlaces = 0;
				}
				sSzStr = "B";
			}

			sprintf_s(sFormat, sizeof(sFormat), "%%.%df", iDecimalPlaces);
			sprintf_s(sOut, iMaxOutSz, sFormat, dFileSize / dDivideBy);

			NSWFL::Conversion::FormatFloatString(sFormatted, sizeof(sFormatted), sOut);

			sprintf_s(sOut, iMaxOutSz, "%s %s", sFormatted, sSzStr);

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
		bool FileAccess(const char* sFileName, short iMode)
		{
			if (_access(sFileName, iMode) == 0)
			{
				return true;
			}

			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			WARNING! - Will not handle files over 4GB.
		*/
		bool FileSize(const char* sFileName, unsigned long* lFileSize)
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

		bool FileSize(const char* sFileName, int* iFileSize)
		{
			HANDLE hfSource = CreateFile(sFileName, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			__int64 i64FileSize = 0;
			bool bResult = NSWFL::File::FileSize(hfSource, &i64FileSize);
			*iFileSize = (int)i64FileSize;
			CloseHandle(hfSource);
			return bResult;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool FileSize(const char* sFileName, __int64* i64FileSize)
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

		bool BufferDataToFile(const char* sFile, const char* sBuf, const unsigned long ulBufSz)
		{
			char sBuffer[FILECHUNKSIZE + 1];

			unsigned long ulRPos = 0;
			unsigned long ulWPos = 0;

			FILE* hTarget = NULL;

			if (fopen_s(&hTarget, sFile, "wb") != 0)
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
					fwrite(sBuffer, sizeof(char), ulWPos, hTarget);
				}
			}

			fclose(hTarget);

			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool DeleteFolderStructure(const char* sPath)
		{
			char sDir[MAX_PATH];
			int iNameSz = (int)strlen(sPath);
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

		bool CreateFolderStructure(const char* sPath)
		{
			char sDir[MAX_PATH];

			int iNameSz = (int)strlen(sPath);
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

		bool IsDirectory(const char* sPath)
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

		bool TruncateFile(const char* sFile, __int64 i64Size)
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

		char* MakeAttributeString(DWORD dwAttribs, char* sOut, int iMaxSz)
		{
			strcpy_s(sOut, iMaxSz, "");

			if (dwAttribs & FILE_ATTRIBUTE_ARCHIVE)
			{
				strcat_s(sOut, iMaxSz, "A ");
			}

			if (dwAttribs & FILE_ATTRIBUTE_HIDDEN)
			{
				strcat_s(sOut, iMaxSz, "H ");
			}

			if (dwAttribs & FILE_ATTRIBUTE_READONLY)
			{
				strcat_s(sOut, iMaxSz, "R ");
			}

			if (dwAttribs & FILE_ATTRIBUTE_SYSTEM)
			{
				strcat_s(sOut, iMaxSz, "S ");
			}

			int iLen = (int)strlen(sOut);

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
