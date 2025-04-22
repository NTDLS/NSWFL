///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_REGISTRY_H_
#define _NSWFL_REGISTRY_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Registry {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int CreateRegKeyStructure(HKEY hKey, LPCWSTR sPath);
		bool CreateRegistryKey(HKEY hKeyRoot, LPCWSTR pszSubKey);
		bool DeleteRegistryKey(HKEY hKeyRoot, LPCWSTR pszSubKey);
		bool DeleteRegistryValue(HKEY hKeyRoot, LPCWSTR pszSubKey, LPCWSTR pszValue);
		bool Get_BinaryRegistryValue(HKEY hKeyRoot, LPCWSTR pszSubKey, LPCWSTR pszValue, PVOID pBuffer, DWORD& rdwSize);
		bool Get_BOOLRegistryValue(HKEY hKeyRoot, LPCWSTR pszSubKey, LPCWSTR pszValue);
		bool Get_DWORDRegistryValue(HKEY hKeyRoot, LPCWSTR pszSubKey, LPCWSTR pszValue, unsigned long& ulBuff);
		bool Get_DWORDRegistryValue(HKEY hKeyRoot, LPCWSTR pszSubKey, LPCWSTR pszValue, unsigned __int64& u64Buff);
		bool Get_StringRegistryValue(HKEY hKeyRoot, LPCWSTR pszSubKey, LPCWSTR pszValue, LPTSTR pszBuffer, DWORD& rdwSize);
		bool Set_BinaryRegistryValue(HKEY hKeyRoot, LPCWSTR pszSubKey, LPCWSTR pszValue, PVOID pData, DWORD dwSize);
		bool Set_BOOLRegistryValue(HKEY hKeyRoot, LPCWSTR pszSubKey, LPCWSTR pszValue, bool bVal);
		bool Set_DWORDRegistryValue(HKEY hKeyRoot, LPCWSTR pszSubKey, LPCWSTR pszValue, unsigned long ulValue);
		bool Set_DWORDRegistryValue(HKEY hKeyRoot, LPCWSTR pszSubKey, LPCWSTR pszValue, unsigned __int64 u64Buff);
		bool Set_StringRegistryValue(HKEY hKeyRoot, LPCWSTR pszSubKey, LPCWSTR pszValue, LPCWSTR pszString);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Registry
} //namespace::NSWFL
#endif
