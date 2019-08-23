#pragma once
//#include "dllmain.h"

//BYTE GetCleoVersion();


struct SampVersion {
	const char* name;
	DWORD IMAGE_FILE_HEADER_DateTime_offset;
	DWORD expectedValue; // at IMAGE_FILE_HEADER_DateTime_offset
	DWORD hook_addrGetChatInputText;
	DWORD funcAddRecall;
	DWORD funcSetInputBoxText;
};

namespace Compability {
	void Init();
	extern SampVersion* sampVersion;
	
}