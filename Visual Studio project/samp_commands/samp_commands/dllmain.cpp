#include "pch.h"
#include <string>

#include "Cleo.h"
#include "Functions.h"
#include "Commands.h"
#include "Exports.h" // contains functions that can be called from cleo mods to interact with this code/project
#include "Compatibility.h"

DWORD origChatInputHandler = NULL;
DWORD origOpcodeHandler = NULL;

wchar_t* GetInputText(DWORD inputPtr) {
	return *(wchar_t **)(inputPtr + 0x4D);
}

char * Hooked_GetChatInputText(char* param_1, char* chatInput, DWORD maxSize) {
	__asm pushfd // no idea why exactly but preserving the flags prevents the mouse cursor from not being disabled after processing input
	static CInput * inputPtr = NULL;
	__asm mov [inputPtr], esi

	//char msg[200];
	//sprintf(msg, "param_1 = 0x%X\nchat_input = %s\nmax_size = %d\nsomeObject = 0x%X", (DWORD)param_1, chatInput, maxSize, (DWORD)inputPtr);
	//MessageBoxA(NULL, msg, "samp_commands.asi - Hooked_HandleText", MB_OK | MB_TOPMOST);

	// modified behaviour
	for (Command* cmd : Commands::vect) {
		if (StartsWith(chatInput, cmd->registered_prefix)) {
			Commands::SaveParams(chatInput, cmd->registered_prefix);
			cmd->return_address = (DWORD)cmd->cleo->CurrentIP;
			cmd->cleo->CurrentIP = (BYTE*)cmd->label_address;
			Commands::last = cmd;

			stInputBox *pInputBox = (stInputBox *)inputPtr->m_pEditbox;

			/*
			wchar_t* wideStr = GetInputText(pInputBox);
			char* str = WideStringToAscii(wideStr);
		
			// clear chat input text
			memset(wideStr, 0, lstrlenW(wideStr) * 2);

			// set cursor to the begining
			*(BYTE*)(pInputBox + 0x11e) = 0;

			// set marked text to the begining
			*(BYTE*)(pInputBox + 0x119) = 0;
			*/

			// add recall (so when you open chat again and press up-arrow it will show your last typed command)
			((void(__thiscall*)(CInput*, const char*))Compability::sampVersion->funcAddRecall)(inputPtr, chatInput);

			((void(__thiscall*)(stInputBox*, const char*, bool))Compability::sampVersion->funcSetInputBoxText)(pInputBox, "", true);
			__asm popfd
			return 0;
		}
	}
	
	//static char trololo[129] = "trololo";
	//return ((char *(*)(char*, char*, DWORD))origChatInputHandler)(param_1, trololo, maxSize);

	__asm popfd
	return ((char* (*)(char*, char*, DWORD))origChatInputHandler)(param_1, chatInput, maxSize);
}

void __stdcall SetCleoInstructionPointerToCmdReturn(CLEO_RunningScript* cleo_thread) {
	if (Commands::last_requested_return && 
		Commands::last && 
		Commands::last->return_address && 
		cleo_thread->BaseIP == Commands::last->cleo->BaseIP) 
	{
		cleo_thread->CurrentIP = (BYTE*)Commands::last->return_address;
		Commands::last->return_address = NULL;
		Commands::last_requested_return = false;
	}
}

/* Original function from: https://github.com/cleolibrary/CLEO4/blob/master/source/CCustomOpcodeSystem.cpp
OpcodeResult __fastcall customOpcodeHandler(CRunningScript* thread, int dummy, unsigned short opcode)
{
	last_custom_opcode = opcode;
	last_script = thread;
	return customOpcodeHandlers[opcode - 0x0A8C](thread);
}*/

int __fastcall Hooked_customOpcodeHandler(CLEO_RunningScript* cleo_thread, int dummy, unsigned short opcode) {
	int return_value = ((int (__fastcall*)(CLEO_RunningScript*, int, unsigned short))origOpcodeHandler)(cleo_thread, 0, opcode); // call original function
	if (opcode == 0x0AB2) {
		SetCleoInstructionPointerToCmdReturn(cleo_thread);
	}
	return return_value;
}


BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved){
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// A process is loading the DLL.

		Commands::Init();
		break;

	case DLL_THREAD_ATTACH:
		// A process is creating a new thread.
		break;

	case DLL_THREAD_DETACH:
		// A thread exits normally.
		break;

	case DLL_PROCESS_DETACH:
		// A process unloads the DLL.
		break;
	}
	return TRUE;
}


