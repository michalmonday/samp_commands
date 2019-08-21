#include "pch.h"
#include <string>

#include "Cleo.h"
#include "Functions.h"
#include "Commands.h"
#include "Exports.h" // contains functions that can be called from cleo mods to interact with this code/project

/*
If you're copying the code instead of opening/modifying the original project then go to:
Project -> Properties (at the bottom) -> C/C++ -> Preprocessor -> Preprocessor Definitions -> <Edit>

And add the following line in the window that opened:
_CRT_SECURE_NO_WARNINGS

Otherwise it won't compile well (because of strcpy)
*/

DWORD origChatInputHandler = NULL;
DWORD origOpcodeHandler = NULL;

// samp.dll + 0x6492A
int __stdcall Hooked_HandleText(const char* typed_command) {
	for (Command* cmd : Commands::vect) {
		if (StartsWith(typed_command, cmd->registered_prefix)) {
			Commands::SaveParams(typed_command, cmd->registered_prefix);
			cmd->return_address = (DWORD)cmd->cleo->CurrentIP;
			//char msg[200];
			//sprintf(msg, "cmd->return_address = 0x%X\nCleo = 0x%X\nLabel address = 0x%X\norigChatInputHandler=0x%X", cmd->return_address, (DWORD)cmd->cleo, cmd->label_address, origChatInputHandler);
			//MessageBoxA(NULL, msg, "Hooked_HandleText", MB_OK);
			cmd->cleo->CurrentIP = (BYTE*)cmd->label_address;
			Commands::last = cmd;
			return 1;
		}
	}

	__asm {
		mov eax, [ebp + 8]
		push eax
		call [origChatInputHandler]
	}

	return 1;
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

/*
void __stdcall NotifyAboutCurrentOpcode(int opcode) {
	char msg[100];
	sprintf(msg, "opcode(eax) = 0x%X", opcode);
	MessageBoxA(NULL, msg, "OrigBehaviour", MB_OK);
}
*/

//cleo.asi + 0x26fe
__declspec(naked) void Hooked_HandleCCustomOpcodeSystemInvoker() {
	static CLEO_RunningScript* cleo_thread;
	static int opcode = NULL;

	_asm {
		check_if_EAX_is_0AB2:
			// upon function entrance eax = opcode value (e.g. 0x0AB2), here we save it to static var
			mov[opcode], eax

			// opcodes below call NotifyAboutCurrentOpcode function
			/*pushad
			pushfd
			push[opcode]
			call[NotifyAboutCurrentOpcode]
			popfd
			popad*/

			cmp eax, 0x0AB2
			jnz original_behaviour // if opcode != 0AB2 then don't modify what happens after the call

		modified_behaviour:
			mov [cleo_thread], ecx // save cleo_thread for after the call (in case if it gets modified inside the original function
			call [original_behaviour]
			push [cleo_thread]
			call [SetCleoInstructionPointerToCmdReturn]
			ret

		original_behaviour: // equivalent to : call dword ptr[eax * 4 + origOpcodeHandler] (but that added pointer of origOpcodeHandler instead of its' value
			mov edx, 4
			mul edx
			mov edx, [origOpcodeHandler]
			add eax, edx
			call [eax]
			ret
	}
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


