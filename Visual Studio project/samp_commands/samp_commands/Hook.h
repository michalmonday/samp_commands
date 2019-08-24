#pragma once
#include <Windows.h>

namespace Hook {
	bool Cleo_customOpcodeHandler(DWORD* originalAddr, DWORD newFunction);

	DWORD AttachRelative(DWORD dwPatchLoc, DWORD dwOurNewFunction);
	DWORD AttachAbsolute(DWORD dwPatchLoc, DWORD* dwOurNewFunction);
}
