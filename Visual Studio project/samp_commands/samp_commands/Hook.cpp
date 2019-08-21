#include "pch.h"
#include "Hook.h"
#include "Functions.h"
#include "dllmain.h"

#include <map>

// Credits: 0x688
// http://ugbase.eu/index.php?threads/do-your-own-sa-mp-commands.18694/


DWORD dwPatchLoc = NULL;

//std::map<DWORD, DWORD> mpSavedOriginalAddresses; (no detaching implemented/needed, so it's not used)

DWORD AttachRelative(DWORD dwPatchLoc, DWORD dwOurNewFunction);
DWORD AttachAbsolute(DWORD dwPatchLoc, DWORD * dwOurNewFunction);

bool Hook::SampHandleText(DWORD* originalAddr, DWORD newFunction) {
	if (DWORD dwSAMP = (DWORD)GetModuleHandle("samp.dll")) {
		*originalAddr = AttachRelative(1 + dwSAMP + 0x6492A, newFunction);
	}
	else {
		MessageBoxA(NULL, "GetModuleHandle('samp.dll') failed.", "samp_commands.asi - Hook::SampHandleText", MB_OK);
		return false;
	}
	return true;

}

bool Hook::CleoCCustomOpcodeSystemInvoker(DWORD* originalAddr, DWORD newFunction) {
	if (DWORD dwCleo = (DWORD)GetModuleHandle("cleo.asi")) {
		/*  Change from:
				ff 14 85 90 14 6d 50     call   DWORD PTR [eax*4+0x506d1490]
			Into:
				90 ff 15 AA AA AA AA     nop + call   DWORD PTR ds:0xAAAAAAAA
			(where AAAAAAAA is address of Hooked_HandleCCustomOpcodeSystemInvoker)
			The new function will check/monitor "0AB2 ret" and set instruction pointer afterwards*/

		DWORD instructionBeforeAddress = 0x15FF90;
		SetData((void*)(dwCleo + 0x26fe), &instructionBeforeAddress, 3);

		static DWORD newFunction_staticCopy = newFunction; // must be static since ff15 reads address and then calls the function at value
		*originalAddr = AttachAbsolute(3 + dwCleo + 0x26fe, &newFunction_staticCopy);


	}
	else {
		MessageBoxA(NULL, "GetModuleHandle('cleo.asi') failed.", "samp_commands.asi - Hook::CleoCCustomOpcodeSystemInvoker", MB_OK);
		return false;
	}
	return true;
}

DWORD AttachRelative(DWORD dwPatchLoc, DWORD dwOurNewFunction){	
	DWORD dwOriginalAddr = NULL;
	GetData((void*)(dwPatchLoc), &dwOriginalAddr, 4);
	dwOriginalAddr = dwOriginalAddr + (dwPatchLoc + 4);
	//mpSavedOriginalAddresses.emplace(std::make_pair(dwOurNewFunction, dwOriginalAddr));

	DWORD dwRelativeAddr = dwOurNewFunction - (dwPatchLoc + 4); // calculate the relative address
	SetData((void*)(dwPatchLoc), &dwRelativeAddr, 4); // set our relative address (remember +1 there to skip the call opcode!)

	//char msg[200];
	//sprintf(msg, "dwPatchLoc = 0x%X\ndwOurNewFunction = 0x%X\ndwOriginalAddr = 0x%X", dwPatchLoc, dwOurNewFunction, dwOriginalAddr);
	//MessageBoxA(NULL, msg, "samp_commands.asi - AttachRelative", MB_OK);
	
	return dwOriginalAddr;
}

DWORD AttachAbsolute(DWORD dwPatchLoc, DWORD * dwOurNewFunction) {
	DWORD dwOriginalAddr = NULL;
	GetData((void*)(dwPatchLoc), &dwOriginalAddr, 4);
	//mpSavedOriginalAddresses.emplace(std::make_pair(dwOurNewFunction, dwOriginalAddr));

	SetData((void*)(dwPatchLoc), &dwOurNewFunction, 4); // set our relative address (remember +1 there to skip the call opcode!)

	//char msg[100];
	//sprintf(msg, "originalAddr = 0x%X\nnewFunction = 0x%X\n&newFunction = 0x%X", dwOriginalAddr, dwOurNewFunction, &dwOurNewFunction);
	//MessageBoxA(NULL, msg, "samp_commands.asi - AttachAbsolute", MB_OK);
	
	return dwOriginalAddr;
}


/*
// not tested yet, not really needed
bool Hook::Detach(DWORD dwPatchLoc, DWORD dwOurNewFunction) {
	
	DWORD dwCurrentAddressAtPatchLoc = NULL;
	GetData((void*)(dwPatchLoc), &dwCurrentAddressAtPatchLoc, 4);

	if (dwCurrentAddressAtPatchLoc == dwOurNewFunction) {
			
		if (mpSavedOriginalAddresses.count(dwOurNewFunction) > 0) {
			SetData((void*)(dwPatchLoc), &mpSavedOriginalAddresses[dwOurNewFunction], 4);
			mpSavedOriginalAddresses.erase(dwOurNewFunction);
			return true;
		}

		char msg[200];
		sprintf(msg, "dwOurNewFunction(0x%X) was not found in mpSavedOriginalAddresses", dwOurNewFunction);
		MessageBoxA(NULL, msg, "Hook::Detach error", MB_OK);
		return false;
	}

	char msg[200];
	sprintf(msg, "currentAddressAtPatchLoc(0x%X) is not equal to dwOurNewFunction(0x%X)", dwCurrentAddressAtPatchLoc, dwOurNewFunction);
	MessageBoxA(NULL, msg, "Hook::Detach error", MB_OK);
	return false;
}
*/
