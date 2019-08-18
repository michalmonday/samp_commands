#include "pch.h"
#include <string>
#include <vector>

#include "Structures.h"
#include "Functions.h"
#include "Hook.h"

/*
If you're copying the code instead of opening/modifying the original project then go to:
Project -> Properties (at the bottom) -> C/C++ -> Preprocessor -> Preprocessor Definitions -> <Edit>

And add the following line in the window that opened:
_CRT_SECURE_NO_WARNINGS

Otherwise it won't compile well (because of strcpy)
*/


std::vector<Command*> commands;
char* last_cmd_params = nullptr;
bool last_cmd_had_params = false;
Command * last_cmd = nullptr;

bool init = false;
extern "C" __declspec(dllexport) void _stdcall RegisterCommand(char * prefix, DWORD label_address, CLEO_RunningScript * cleo_thread_pointer) {
	commands.push_back(new Command(
		prefix, 
		label_address, 
		cleo_thread_pointer,
		NULL 
		));

	if (!init) {
		init = true;
		Hook::Attach();
		last_cmd_params = (char*)malloc(300); // https://san-andreas-multiplayer-samp.fandom.com/wiki/Limits
	}
}

extern "C" __declspec(dllexport) char* _stdcall GetLastCommandParams() {
	return last_cmd_params;
}

// callback
int __stdcall HandleText(const char* typed_command){
	for (Command * cmd : commands) {
		if (StartsWith(typed_command, cmd->registered_prefix)) {

			if (size_t params_length = GetParamsLength(typed_command, cmd->registered_prefix)) {
				last_cmd_had_params = true;
				last_cmd_params = (char*)realloc(last_cmd_params, params_length + 1);

				strcpy(last_cmd_params, typed_command + strlen(cmd->registered_prefix));
			} else {
				last_cmd_had_params = false;
			}

			cmd->return_address = (DWORD)cmd->cleo->CurrentIP;
			
			char msg[200];
			sprintf(msg, "cmd->return_address = 0x%X\nCleo = 0x%X\nLabel address = 0x%X", cmd->return_address, (DWORD)cmd->cleo, cmd->label_address);
			MessageBoxA(NULL, msg, "HandleText", MB_OK);

			cmd->cleo->CurrentIP = (BYTE*)cmd->label_address;

			last_cmd = cmd;
			return 1;
		}
	}

	__asm mov eax, [ebp + 8]
	__asm push eax
	__asm call[origChatInputHandler]

	return 1;
}

extern "C" __declspec(dllexport) void _stdcall CmdRet() {
	if (last_cmd->return_address) {
		last_cmd->cleo->CurrentIP = (BYTE*)last_cmd->return_address;
		

		char msg[200];
		sprintf(msg, "cmd->return_address = 0x%X\nCleo = 0x%X", last_cmd->return_address, (DWORD)last_cmd->cleo);
		MessageBoxA(NULL, msg, "CmdRet()", MB_OK);		
		last_cmd->return_address = NULL;	
	}
	else {
		MessageBoxA(NULL, "null return address", "CmdRet", MB_OK);
	}
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

