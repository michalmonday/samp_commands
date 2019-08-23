#pragma once
#include <Windows.h>

namespace Hook {
	//bool Samp_GetChatInputText(DWORD* originalAddr, DWORD newFunction);
	bool Cleo_customOpcodeHandler(DWORD* originalAddr, DWORD newFunction);
	//bool Samp_SendCommandToServer(DWORD* originalAddr, DWORD newFunction);

	DWORD AttachRelative(DWORD dwPatchLoc, DWORD dwOurNewFunction);
	DWORD AttachAbsolute(DWORD dwPatchLoc, DWORD* dwOurNewFunction);
}
