#pragma once
#include <Windows.h>

namespace Hook {
	bool SampHandleText(DWORD* originalAddr, DWORD newFunction);
	bool CleoCCustomOpcodeSystemInvoker(DWORD* originalAddr, DWORD newFunction);
	
	//DWORD AttachRelative(DWORD dwPatchLoc, DWORD dwOurNewFunction);
	//DWORD AttachAbsolute(DWORD dwPatchLoc, DWORD * dwOurNewFunction);
	//bool Detach(DWORD dwPatchLoc, DWORD dwOurNewFunction); // dwOurNewFunction is passed to verify that it's actually there
}
