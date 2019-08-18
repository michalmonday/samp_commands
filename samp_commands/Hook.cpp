#include "pch.h"
#include "Hook.h"
#include "Functions.h"
#include "dllmain.h"


// Credits: 0x688
// http://ugbase.eu/index.php?threads/do-your-own-sa-mp-commands.18694/


DWORD dwPatchLoc = NULL;
DWORD origChatInputHandler = NULL; 

void Hook::Attach(){
	DWORD dwSAMP = (DWORD)GetModuleHandle("samp.dll");
	if (dwSAMP && !origChatInputHandler)
	{
		dwPatchLoc = dwSAMP + 0x6492A;
		GetData((void*)(dwPatchLoc + 1), &origChatInputHandler, 4);
		origChatInputHandler = origChatInputHandler + dwPatchLoc + 5;

		DWORD dwRelativeAddr = ((DWORD)HandleText) - (dwPatchLoc + 5); // calculate the relative address
		SetData((void*)(dwPatchLoc + 1), &dwRelativeAddr, 4); // set our relative address (remember +1 there to skip the call opcode!)
	
	}
}

void Hook::Detach() {
	if (origChatInputHandler && dwPatchLoc) {
		SetData((void*)(dwPatchLoc + 1), &origChatInputHandler, 4);
		origChatInputHandler = NULL;
	}
}
