///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_INSECUREINTERFACE_H_
#define _NSWFL_INSECUREINTERFACE_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace InsecureInterface {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef NSWFL_USE_ALT_SECURE_PROCS

#define strcpy_s(Dest, MaxSize, Source) strcpy(Dest, Source)
#define strcat_s(Dest, MaxSize, Source) strcat(Dest, Source)
#define strncat_s(Dest, MaxSize, Source, Count) strncat(Dest, Source, Count)
#define strncpy_s(Dest, MaxSize, Source, Count) strncpy(Dest, Source, Count)
#define memcpy_s(Dest, MaxSize, Source, Count) memcpy(Dest, Source, Count)
#define _ultoa_s(Number, Dest, MaxSize, Radix) _ultoa(Number, Dest, Radix)
#define _i64toa_s(Number, Dest, MaxSize, Radix) _i64toa(Number, Dest, Radix)
#define _itoa_s(Number, Dest, MaxSize, Radix) _itoa(Number, Dest, Radix)
#define _gcvt_s(Dest, MaxSize, Number, DecPlaces) _gcvt(Number, DecPlaces, Dest)
#define _splitpath_s(FileAndPath, Drive, DriveSz, Dir, DirSz, File, FileSz, Ext, ExtSz) _splitpath(FileAndPath, Drive, Dir, File, Ext)
#define _makepath_s(FileAndPath, FileAndPathSz, Drive, Dir, File, Ext) _makepath(FileAndPath, Drive, Dir, File, Ext)
#define fopen_s(Target, File, Mode) fopen_s_Ex(Target, File, Mode)

		short _fcvt_s(char* buffer, size_t sizeInBytes, double value, int count, int* dec, int* sign);
		bool fopen_s_Ex(FILE** Target, const char* sFile, const char* Mode);

#ifdef _MSC_VER
#define sprintf_s(Dest, MaxSize, Format, ...) sprintf(Dest, Format, ...)
#else
#define sprintf_s(Dest, MaxSize, Format, Args...) sprintf(Dest, Format, ##Args)

#if !defined(_WIN32_WINNT) && !defined(_CHICAGO_)
#define  _WIN32_WINNT   0x0600
#endif

		//#if(_WIN32_WINNT >= 0x0501)
#define SM_TABLETPC             86
#define SM_MEDIACENTER          87
#define SM_STARTER              88
#define SM_SERVERR2             89
//#endif /* _WIN32_WINNT >= 0x0501 */
#endif
#endif
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::InsecureInterface
} //namespace::NSWFL
#endif
