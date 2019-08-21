#pragma once

/* cleo_thread_pointer is a pointer to CRunningScript object from: https://github.com/cleolibrary/CLEO4/blob/master/source/CTheScripts.h
This mod is all based on assumption that the CRunningScript class (at least the first few members of the class)
didn't change between 4.1 and 4.3 versions.
*/

struct CLEO_RunningScript
{
	CLEO_RunningScript* Previous;			// +0x0
	CLEO_RunningScript* Next;				// +0x4
	char Name[8];						// +0x8
	void* BaseIP;						// +0x10
	BYTE* CurrentIP;					// +0x14
	BYTE* Stack[8];						// +0x18
	WORD SP;							// +0x38
	int LocalVar[32];					// +0x3C (changed from SCRIPT_VAR to int)
	DWORD Timers[2];					// +0xBC
	bool bIsActive;						// +0xC4
	bool bCondResult;					// +0xC5
	bool bUseMissionCleanup;			// +0xC6
	bool bIsExternal;					// +0xC7
	bool bTextBlockOverride;			// +0xC8
	BYTE bExternalType;					// +0xC9
	DWORD WakeTime;						// +0xCC
	WORD LogicalOp;						// +0xD0
	bool NotFlag;						// +0xD2
	bool bWastedBustedCheck;			// +0xD3
	bool bWastedOrBusted;				// +0xD4
	void* SceneSkipIP;					// +0xD8
	bool bIsMission;					// +0xDC
};

