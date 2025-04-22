///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_H_
#define _NSWFL_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <Stdio.H>
#include <Stdlib.H>

//The memory pool class is used to track memory allocations to ease the task of memory leak detection. It can be removed from release code with preprocessors.
#include "NSWFL_MemoryPool.H"

//Types used by NSWFL, currently empty. /shrug
#include "NSWFL_Types.H"

//Type conversions and parsers. Dates, strings, int, float, boolean, you name it.
#include "NSWFL_Conversion.H"

//Getting, comparing and converting various date/times.
#include "NSWFL_DateTime.H"

//Random number an string generator.
#include "NSWFL_KeyGeneration.H"

//Functions for working with WinAPI list boxes.
#include "NSWFL_ListBox.H"

//Its math. What did you expect?
#include "NSWFL_Math.H"

//Memory operations, clear, set, copy, etc.
#include "NSWFL_Memory.H"

//Everything you need to access the registry.
#include "NSWFL_Registry.H"

//String manipulation.
#include "NSWFL_String.H"

//OS level functions, like get system name.
#include "NSWFL_System.H"

//Lots of functions for managing windows or anything with an HWND.
#include "NSWFL_Windows.H"

//File access functions.
#include "NSWFL_File.H"

//Functions for working with WinAPI menus.
#include "NSWFL_Menu.H"

//Functions for working with WinAPI list views.
#include "NSWFL_ListView.H"

//Graphics utilities, don't expect much.
#include "NSWFL_Graphics.H"

//Debugging functions/
#include "NSWFL_Debug.H"

//Functions to get around the stupid secure "_s" counterparts to the standard clib.
#include "NSWFL_InsecureInterface.H"

//It's a high precision timer.
#include "NSWFL_HighPrecisionTimer.H"

//Functions for parsing arguments passed to the command line.
#include "NSWFL_CommandLineParser.H"

//A reasonable string builder for C++, finally!
#include "NSWFL_StringBuilder.H"

//A array of reasonable string builders for C++ (see what I did there?)
#include "NSWFL_StringBuilderArray.H"

//Base64 to and from.
#include "NSWFL_Base64.H"

//A void* vector. Store what you will.
#include "NSWFL_VectorBinary.H"

//CRC32 (cyclic redundancy check) calculation.
#include "NSWFL_CRC32.H"

//SHA1 hashing functions.
#include "NSWFL_SHA1.H"

//A FIFO queue for C++
#include "NSWFL_Queue.H"

//A FIFO or FILO stack for C++
#include "NSWFL_Stack.H"

//A string vector class. Like NSWFL_VectorBinary is for void*, but for char*.
#include "NSWFL_VectorString.H"

//A vector template function.
#include "NSWFL_VectorT.H"

//Class for reading and parsing XML.
#include "NSWFL_XMLReader.H"

//Class for building and writing XML.
#include "NSWFL_XMLWriter.H"
#include "NSWFL_XML.H"

//HTTP URL encoding.
#include "NSWFL_URLEncoding.H"

//An attempt to create a better critical section for thread locking.
#include "NSWFL_IsolationLock.h"

//A dictionary template class.
#include "NSWFL_DictionaryT.H"

//A dictionary of string(char*) class.
#include "NSWFL_DictionaryString.H"

//INET/TCP/IP functions. If you really want more, see https://github.com/NTDLS/CSocketServer
#include "NSWFL_Network.H"

//A stack of integers.
#include "NSWFL_IntegerStack.H"

//A stack of unsigned integers.
#include "NSWFL_UIntegerStack.H"

//Used to keep track of a list of calls so that one can provide a stack trace.
#include "NSWFL_Trace.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
