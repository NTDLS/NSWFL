#ifndef _ENTRY_CPP
#define _ENTRY_CPP
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <ShlObj.H>
#include <Stdio.H>
#include <Stdlib.H>

#include "../NSWFL.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace NSWFL::Conversion;
using namespace NSWFL::DateTime;
using namespace NSWFL::File;
using namespace NSWFL::KeyGeneration;
using namespace NSWFL::ListBox;
using namespace NSWFL::ListView;
using namespace NSWFL::Math;
using namespace NSWFL::Memory;
using namespace NSWFL::Menu;
using namespace NSWFL::Registry;
using namespace NSWFL::String;
using namespace NSWFL::System;
using namespace NSWFL::Types;
using namespace NSWFL::Windows;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main(void)
{
	MsgBoxS("Current up time is: %dms.", GetTickCount());

	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
