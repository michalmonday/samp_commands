#pragma once
#include "Cleo.h"

extern DWORD origChatInputHandler;
extern DWORD origOpcodeHandler;
extern DWORD origSendCommandToServer;


/* CInput stuff */
#define MAX_CLIENT_CMDS 144
#define MAX_CMD_LENGTH 32

class IDirect3DDevice9;
class CDXUTDialog;
class CRect;

typedef void(__cdecl* CMDPROC)(const char*);

class stInputBox;

#pragma pack(push, 1) // makes sure that the correct offsets are added when struct members are accessed

/*
struct stInputBox
{
	void* pUnknown; // 0
	BYTE	bIsChatboxOpen; // 4
	BYTE	bIsMouseInChatbox; // 5
	BYTE	bMouseClick_related; // 6
	BYTE	unk; // 7 
	DWORD	dwPosChatInput[2]; // 8

	//BYTE	unk2[263]; // 0x10
	BYTE unk_[61]; // 0x10
	wchar_t* inputBoxText; // 0x4D (gets compiled as 0x50 without #pragma pack)
	BYTE unk_2[198]; // 0x51

	int		iCursorPosition; // 0x10f
	BYTE	unk3; // 0x113
	int		iMarkedText_startPos; // 0x114   
	union {
		BYTE	unk4[20]; 
	};


	int		iMouseLeftButton; // 0x130
};
*/

/*
void __thiscall FUN_10080f60(stInputBox * iParm1, undefined4 param_1)
{
  undefined4 uVar1;

  FUN_10097930(param_1);
  *(undefined4 *)(iParm1 + 0x122) = 0;
  uVar1 = (*(code *)0xeacf4)(*(undefined4 *)(iParm1 + 0x4d));
  FUN_10080e50(uVar1);
  if ((char)param_1 != '\0') {
	*(undefined4 *)(iParm1 + 0x11e) = 0;
	return;
  }
  *(undefined4 *)(iParm1 + 0x11e) = *(undefined4 *)(iParm1 + 0x119);
  return;
}

*/

class CInput {
public:
	IDirect3DDevice9* m_pDevice;
	CDXUTDialog* m_pGameUi;
	stInputBox* m_pEditbox;
	CMDPROC				m_pCommandProc[MAX_CLIENT_CMDS];
	char					m_szCommandName[MAX_CLIENT_CMDS][MAX_CMD_LENGTH + 1];
	int					m_nCommandCount;
	BOOL					m_bEnabled;
	char					m_szInput[129];
	char					m_szRecallBufffer[10][129];
	char					m_szCurrentBuffer[129];
	int					m_nCurrentRecall;
	int					m_nTotalRecall;
	CMDPROC				m_pDefaultCommand;

	CInput(IDirect3DDevice9* pDevice);

	void GetRect(CRect* pRect);
	void Open();
	void Close();
	void AddRecall(const char* szString);
	void RecallUp();
	void RecallDown();
	void EnableCursor();
	CMDPROC GetCommandHandler(const char* szName);
	void SetDefaultCommand(CMDPROC handler);
	void AddCommand(const char* szName, CMDPROC handler);
	BOOL MsgProc(int uMsg, int wParam, int lParam);
	void ResetDialogControls(CDXUTDialog* pGameUi);
	void Send(const char* szString);
	void ProcessInput();
};


#pragma pack(pop)




//extern void Hooked_HandleText();

extern char* Hooked_GetChatInputText(char* param_1, char* param_2, DWORD param_3);
//extern int __stdcall Hooked_HandleText(const char* typed_command);
extern int __fastcall Hooked_customOpcodeHandler(CLEO_RunningScript* cleo_thread, int dummy, unsigned short opcode);


extern int __stdcall SendCommandToServer(const char* szCmd);