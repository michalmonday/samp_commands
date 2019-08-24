#include "pch.h"
#include "Hook.h"
#include "Functions.h"
#include "dllmain.h"
#include "Compatibility.h"

//#include <map>

// Credits: 0x688
// http://ugbase.eu/index.php?threads/do-your-own-sa-mp-commands.18694/

//std::map<DWORD, DWORD> mpSavedOriginalAddresses; (no detaching implemented/needed, so it's not used)

DWORD AttachRelative(DWORD dwPatchLoc, DWORD dwOurNewFunction);
DWORD AttachAbsolute(DWORD dwPatchLoc, DWORD * dwOurNewFunction);

bool Hook::Cleo_customOpcodeHandler(DWORD* originalAddr, DWORD newFunction) {
	if (DWORD dwCleo = (DWORD)GetModuleHandle("cleo.asi")) {
		DWORD ptrToStoredHandlerPtr = NULL;
		GetData((void*)(0x469FEB + 3), &ptrToStoredHandlerPtr, 4);	
		ptrToStoredHandlerPtr += 0x1B * 4; // there's seems to be a whole table of opcode handlers (0x1B*4 points to the one used by cleo "CustomOpcodes", other offsets can be used to hook other opcodes too) 
		*originalAddr = AttachAbsolute(ptrToStoredHandlerPtr, (DWORD*)newFunction);
		return true;
	}
	
	MessageBoxA(NULL, "GetModuleHandle('cleo.asi') failed.", "samp_commands.asi - Hook::CleoCCustomOpcodeSystemInvoker", MB_OK);
	return false;
}

DWORD Hook::AttachRelative(DWORD dwPatchLoc, DWORD dwOurNewFunction){	
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

DWORD Hook::AttachAbsolute(DWORD dwPatchLoc, DWORD * dwOurNewFunction) {
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
