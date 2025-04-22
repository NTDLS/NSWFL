///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_FILE_H_
#define _NSWFL_FILE_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace File {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define FReadWrite 6   // Check for read and write permission
#define FRead      4   // Check for read permission
#define FWrite     2   // Check for write permission
#define FExist     0   // Check for existence of file

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define FILECHUNKSIZE 1024 // NON-Public

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KiloByte 1024
#define MegaByte 1048576
#define GigaByte 1073741824
#define TeraByte 1099511627776LL
#define PetaByte 1125899906842624LL
#define ExaByte  1152921504606847000LL

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		__int64 FileSeek(HANDLE hFile);
		__int64 FileSeek(HANDLE hFile, __int64 l64Distance, DWORD dwMoveMethod);

		void CorrectForwardPath(WCHAR* sPath, int iMaxSz);
		void CorrectReversePath(WCHAR* sPath, int iMaxSz);
		void CorrectReversePath(WCHAR* sPath, int iMaxSz, bool bPossibleNetworkPath);
		void CleanFileName(WCHAR* sPath, int iMaxSz);

		WCHAR* FileSizeFriendly(__int64 i64FileSize, WCHAR* sOut, int iMaxOutSz);
		WCHAR* FileSizeFriendly(__int64 i64FileSize, int iDecimalPlaces, WCHAR* sOut, int iMaxOutSz);

		WCHAR* FileSizeFriendly(unsigned __int64 i64FileSize, WCHAR* sOut, int iMaxOutSz);
		WCHAR* FileSizeFriendly(unsigned __int64 i64FileSize, int iDecimalPlaces, WCHAR* sOut, int iMaxOutSz);

		WCHAR* FileSizeFriendly(int iFileSize, WCHAR* sOut, int iMaxOutSz);
		WCHAR* FileSizeFriendly(int iFileSize, int iDecimalPlaces, WCHAR* sOut, int iMaxOutSz);

		WCHAR* FileSizeFriendly(double dFileSize, WCHAR* sOut, int iMaxOutSz);
		WCHAR* FileSizeFriendly(double dFileSize, int iDecimalPlaces, WCHAR* sOut, int iMaxOutSz);

		bool FileAccess(const WCHAR* sFileName, short iMode);

		bool FileSize(const WCHAR* sFileName, __int64* i64FileSize);
		bool FileSize(HANDLE hFile, __int64* i64FileSize);
		bool FileSize(const WCHAR* sFileName, unsigned long* lFileSize);
		bool FileSize(HANDLE hFile, unsigned long* lFileSize);
		bool FileSize(const WCHAR* sFileName, int* iFileSize);

		bool TruncateFile(HANDLE hFile, __int64 i64Size);
		bool TruncateFile(const WCHAR* sFile, __int64 i64Size);

		bool BufferDataToFile(const WCHAR* sFile, const WCHAR* sBuf, const unsigned long ulBufSz);

		bool CreateFolderStructure(const WCHAR* sPath);
		bool DeleteFolderStructure(const WCHAR* sPath);
		bool IsDirectory(const WCHAR* sPath);

		DWORD Get_CurrentDirectory(WCHAR* sOutVal, int iMaxSz);
		DWORD Get_TempDirectory(WCHAR* sOutVal, int iMaxSz);
		DWORD Get_WindowsDirectory(WCHAR* sOutVal, int iMaxSz);
		DWORD Get_SystemDirectory(WCHAR* sOutVal, int iMaxSz);
		DWORD Get_DesktopDirectory(WCHAR* sOut, DWORD dwMaxOut);
		DWORD Get_DocumentsDirectory(WCHAR* sOut, DWORD dwMaxOut);
		DWORD Get_PicturesDirectory(WCHAR* sOut, DWORD dwMaxOut);
		DWORD Get_MusicDirectory(WCHAR* sOut, DWORD dwMaxOut);
		DWORD Get_VideosDirectory(WCHAR* sOut, DWORD dwMaxOut);

		bool GetFileExtension(const WCHAR* sFileNameAndPath, WCHAR* sOutExt, int iMaxSz);
		bool GetFileNameOnly(const WCHAR* sFileNameAndPath, WCHAR* sOutFile, int iMaxSz);
		bool GetFileName(const WCHAR* sFileNameAndPath, WCHAR* sOutPath, int iMaxSz);
		bool GetFilePath(const WCHAR* sFileNameAndPath, WCHAR* sOutPath, int iMaxSz);
		bool GetFileDrive(const WCHAR* sFileNameAndPath, WCHAR* sOutDrive, int iMaxSz);

		bool GetModuleFilePath(WCHAR* sOutPath, int iMaxSz);

		bool ArePathsEqual(const WCHAR* sInPath1, const WCHAR* sInPath2);
		bool IsChildPath(const WCHAR* sInParent, const WCHAR* sInChild);
		int AppendPaths(WCHAR* sTarget, int iMaxTargetSize, const WCHAR* sPart1, const WCHAR* Part2);

		WCHAR* MakeAttributeString(DWORD dwAttribs, WCHAR* sOut, int iMaxSz);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::File
} //namespace::NSWFL
#endif
