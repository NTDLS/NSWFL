///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
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

		void CorrectForwardPath(char *sPath, int iMaxSz);
		void CorrectReversePath(char *sPath, int iMaxSz);
		void CorrectReversePath(char *sPath, int iMaxSz, bool bPossibleNetworkPath);
		void CleanFileName(char *sPath, int iMaxSz);

		char *FileSizeFriendly(__int64 i64FileSize, char *sOut, int iMaxOutSz);
		char *FileSizeFriendly(__int64 i64FileSize, int iDecimalPlaces, char *sOut, int iMaxOutSz);

		char *FileSizeFriendly(unsigned __int64 i64FileSize, char *sOut, int iMaxOutSz);
		char *FileSizeFriendly(unsigned __int64 i64FileSize, int iDecimalPlaces, char *sOut, int iMaxOutSz);

		char *FileSizeFriendly(int iFileSize, char *sOut, int iMaxOutSz);
		char *FileSizeFriendly(int iFileSize, int iDecimalPlaces, char *sOut, int iMaxOutSz);

		char *FileSizeFriendly(double dFileSize, char *sOut, int iMaxOutSz);
		char *FileSizeFriendly(double dFileSize, int iDecimalPlaces, char *sOut, int iMaxOutSz);

		bool FileAccess(const char *sFileName, short iMode);

		bool FileSize(const char *sFileName, __int64 *i64FileSize);
		bool FileSize(HANDLE hFile, __int64 *i64FileSize);
		bool FileSize(const char *sFileName, unsigned long *lFileSize);
		bool FileSize(HANDLE hFile, unsigned long *lFileSize);
		bool FileSize(const char *sFileName, int *iFileSize);

		bool TruncateFile(HANDLE hFile, __int64 i64Size);
		bool TruncateFile(const char *sFile, __int64 i64Size);

		bool BufferDataToFile(const char *sFile, const char *sBuf, const unsigned long ulBufSz);

		bool CreateFolderStructure(const char *sPath);
		bool DeleteFolderStructure(const char *sPath);
		bool IsDirectory(const char *sPath);

		DWORD Get_CurrentDirectory(char *sOutVal, int iMaxSz);
		DWORD Get_TempDirectory(char *sOutVal, int iMaxSz);
		DWORD Get_WindowsDirectory(char *sOutVal, int iMaxSz);
		DWORD Get_SystemDirectory(char *sOutVal, int iMaxSz);
		DWORD Get_DesktopDirectory(char *sOut, DWORD dwMaxOut);
		DWORD Get_DocumentsDirectory(char *sOut, DWORD dwMaxOut);
		DWORD Get_PicturesDirectory(char *sOut, DWORD dwMaxOut);
		DWORD Get_MusicDirectory(char *sOut, DWORD dwMaxOut);
		DWORD Get_VideosDirectory(char *sOut, DWORD dwMaxOut);

		bool GetFileExtension(const char *sFileNameAndPath, char *sOutExt, int iMaxSz);
		bool GetFileNameOnly(const char *sFileNameAndPath, char *sOutFile, int iMaxSz);
		bool GetFileName(const char *sFileNameAndPath, char *sOutPath, int iMaxSz);
		bool GetFilePath(const char *sFileNameAndPath, char *sOutPath, int iMaxSz);
		bool GetFileDrive(const char *sFileNameAndPath, char *sOutDrive, int iMaxSz);

		bool GetModuleFilePath(char *sOutPath, int iMaxSz);

		bool ArePathsEqual(const char *sInPath1, const char *sInPath2);
		bool IsChildPath(const char *sInParent, const char *sInChild);
		int AppendPaths(char *sTarget, int iMaxTargetSize, const char *sPart1, const char *Part2);

		char *MakeAttributeString(DWORD dwAttribs, char *sOut, int iMaxSz);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::File
} //namespace::NSWFL
#endif
