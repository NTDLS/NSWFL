///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_SYSTEM_CPP_
#define _NSWFL_SYSTEM_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool *pMem; //pMem must be defined and initalized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace System {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void SetApplicationPriority(HANDLE hProcess, HANDLE hThread,
			int iPriorityClass, short iThreadPriority,
			bool bProcessBoost, bool bThreadBoost)
		{
			if (iPriorityClass == 1) SetPriorityClass(hProcess, IDLE_PRIORITY_CLASS);
			if (iPriorityClass == 2) SetPriorityClass(hProcess, NORMAL_PRIORITY_CLASS);
			if (iPriorityClass == 3) SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
			if (iPriorityClass == 4) SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);

			if (iThreadPriority == 1) SetThreadPriority(hThread, THREAD_PRIORITY_IDLE);
			if (iThreadPriority == 2) SetThreadPriority(hThread, THREAD_PRIORITY_LOWEST);
			if (iThreadPriority == 3) SetThreadPriority(hThread, THREAD_PRIORITY_BELOW_NORMAL);
			if (iThreadPriority == 4) SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
			if (iThreadPriority == 5) SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			if (iThreadPriority == 6) SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
			if (iThreadPriority == 7) SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);

			SetProcessPriorityBoost(hProcess, !bProcessBoost);
			SetThreadPriorityBoost(hThread, !bThreadBoost);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Get_MachineName(char *sOutVal, int iMaxSz)
		{
			DWORD dwMaxSz = (DWORD)iMaxSz;
			//Could be as much as MAX_COMPUTERNAME_LENGTH characters.
			return (GetComputerName(sOutVal, &dwMaxSz) != 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Get_UserName(char *sOutVal, int iMaxSz)
		{
			DWORD dwMaxSz = (DWORD)iMaxSz;
			return (GetUserName(sOutVal, &dwMaxSz) != 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Get_OsVersion(char *sOutVal, int iMaxSz)
		{
			DWORD dwMaxSize = iMaxSz;
			memset(sOutVal, 0, iMaxSz);
			if (Registry::Get_StringRegistryValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName", sOutVal, dwMaxSize))
			{
				return true;
			}

			return false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/* uFlags can be any combination of the following
		EWX_FORCE
			Forces processes to terminate.
			When this flag is set, Windows does not send the messages WM_QUERYENDSESSION and WM_ENDSESSION to the
			applications currently running in the system. This can cause the applications to lose data.
			Therefore, you should only use this flag in an emergency.

		EWX_LOGOFF
			Shuts down all processes running in the security context of the process that called the ExitWindowsEx function.
			Then it logs the user off.

		EWX_POWEROFF
			Shuts down the system and turns off the power.
			The system must support the power-off feature.
			Windows NT/2K/XP: The calling process must have the SE_SHUTDOWN_NAME privilege.
			Windows 95/98/ME: Security privileges are not supported or required.

		EWX_REBOOT
			Shuts down the system and then restarts the system.
			Windows NT/2K/XP: The calling process must have the SE_SHUTDOWN_NAME privilege.
			Windows 95/98/ME: Security privileges are not supported or required.

		EWX_SHUTDOWN
			Shuts down the system to a point at which it is safe to turn off the power.
			All file buffers have been flushed to disk, and all running processes have stopped.
			Windows NT/2K/XP: The calling process must have the SE_SHUTDOWN_NAME privilege.
			Windows 95/98/ME: Security privileges are not supported or required.
		*/

		bool ResetWindowsSys(unsigned int iFlags)
		{
			if (iFlags &EWX_POWEROFF || iFlags &EWX_REBOOT || iFlags &EWX_SHUTDOWN)
			{
				HANDLE hToken;
				TOKEN_PRIVILEGES tkp;

				if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
				{
					return false;
				}

				LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

				tkp.PrivilegeCount = 1;
				tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

				AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

				if (GetLastError() != ERROR_SUCCESS)
				{
					return false;
				}
			}

			return (ExitWindowsEx(iFlags, 0) != 0);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Hang(void)
		{
			while (true)
			{
				Sleep(1);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CreateGenericSecurityDescriptor(SECURITY_DESCRIPTOR *pSD, SECURITY_ATTRIBUTES *pSA)
		{
			memset(pSD, 0, sizeof(SECURITY_DESCRIPTOR));
			memset(pSA, 0, sizeof(SECURITY_ATTRIBUTES));

			if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
			{
				return false;
			}

			if (!SetSecurityDescriptorDacl(pSD, TRUE, 0, FALSE))
			{
				return false;
			}

			pSA->nLength = sizeof(SECURITY_ATTRIBUTES);
			pSA->lpSecurityDescriptor = pSD;
			pSA->bInheritHandle = FALSE;

			return true;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::System
} //namespace::NSWFL
#endif