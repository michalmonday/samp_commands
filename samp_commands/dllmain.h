#pragma once

extern DWORD origChatInputHandler;
extern DWORD origOpcodeHandler;

extern int __stdcall Hooked_HandleText(const char* typed_command);
extern void Hooked_HandleCCustomOpcodeSystemInvoker();

