#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_URLEncoding_CPP_
#define _NSWFL_URLEncoding_CPP_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"

#ifdef _USE_GLOBAL_MEMPOOL
extern NSWFL::Memory::MemoryPool* pMem; //pMem must be defined and initalized elsewhere.
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Network {

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef VER_PRODUCTMAJORVERSION
#if VER_PRODUCTMAJORVERSION <= 6
		int legacy_inet_pton(int af, const WCHAR* src, void* dst)
		{
			struct sockaddr_storage ss;
			int size = sizeof(ss);
			WCHAR src_copy[INET6_ADDRSTRLEN + 1];

			ZeroMemory(&ss, sizeof(ss));
			wcsncpy_s(src_copy, sizeof(src_copy), src, INET6_ADDRSTRLEN + 1);
			src_copy[INET6_ADDRSTRLEN] = 0;

			if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr*)&ss, &size) == 0) {
				switch (af) {
				case AF_INET:
					*(struct in_addr*)dst = ((struct sockaddr_in*)&ss)->sin_addr;
					return 1;
				case AF_INET6:
					*(struct in6_addr*)dst = ((struct sockaddr_in6*)&ss)->sin6_addr;
					return 1;
				}
			}
			return 0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		const WCHAR* legacy_inet_ntop(int af, const void* src, WCHAR* dst, int size)
		{
			struct sockaddr_storage ss;
			unsigned long s = size;

			ZeroMemory(&ss, sizeof(ss));
			ss.ss_family = af;

			switch (af) {
			case AF_INET:
				((struct sockaddr_in*)&ss)->sin_addr = *(struct in_addr*)src;
				break;
			case AF_INET6:
				((struct sockaddr_in6*)&ss)->sin6_addr = *(struct in6_addr*)src;
				break;
			default:
				return NULL;
			}
			return (WSAAddressToString((struct sockaddr*)&ss, sizeof(ss), NULL, dst, &s) == 0) ?
				dst : NULL;
		}
#endif
#endif


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Network
} //namespace::NSWFL
#endif
