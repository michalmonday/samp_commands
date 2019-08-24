#include "pch.h"
#include "Compatibility.h"
#include "Functions.h"
#include <string>

#define INITIAL_INVALID 255

SampVersion* Compability::sampVersion = nullptr;

SampVersion versions[] = {
//{name, DateTime_offset, expectedValue, hook_addrGetChatInputText, funcAddRecall, funcSetInputBoxText}
{"0.3.7 R1",	0x128,		0x5542F47A,			0x65D57,				0x65930,		0x80F60},
{"0.3.7 R2",	0x128,		0x59C30C94,			0x65e27,				0x65a00,		0x81000},
{"0.3.7 R3 1",	0x120,		0x5C0B4243,			0x69287,				0x68e60,		0x84e70},
{"0.3.DL R1",	0x128,		0x5A6A3130,			0x69437,				0x69010,		0x85000}
};

SampVersion * GetSampVersion(DWORD dwSAMP) {
	DWORD signatureBuffer;

	for (SampVersion& v : versions) {
		GetData((BYTE*)(dwSAMP + v.IMAGE_FILE_HEADER_DateTime_offset), &signatureBuffer, 4);
		if (signatureBuffer == v.expectedValue) {			
			//MessageBoxA(NULL, v.name, "samp_commands.asi - Recognized samp version", MB_OK | MB_TOPMOST);
			return &v;
		}
	}
	return nullptr;
}

void Compability::Init() {
	if (DWORD dwSAMP = (DWORD)GetModuleHandle("samp.dll")) {
		if (sampVersion = GetSampVersion(dwSAMP)) {
			sampVersion->funcAddRecall += dwSAMP;
			sampVersion->hook_addrGetChatInputText += dwSAMP;
			sampVersion->funcSetInputBoxText += dwSAMP;
			return;
		}
		MessageBoxA(NULL, "Samp version not recognized, probably crash incoming...", "samp_commands.asi - GetSampVersion", MB_OK | MB_TOPMOST);
		return;
	}

	MessageBoxA(NULL, "GetModuleHandle('samp.dll') failed.", "samp_commands.asi - Compability::Init", MB_OK | MB_TOPMOST);
}




//BYTE GetCleoVersion() {
//	if (DWORD dwCleo = (DWORD)GetModuleHandle("cleo.asi")) {
//		/*  Cleo 4.1 has the following bytes at 0x26FE offset:
//				ff 14 85 90 14 6d 50     call   DWORD PTR [eax*4+0x506d1490] */
//
//		DWORD signature = NULL;
//		GetData((void*)(dwCleo + 0x26FE), &signature, 2);
//
//		if (signature == 0x14FF) {
//			//MessageBoxA(NULL, "Cleo version = 4.1", "samp_commands.asi - GetCleoVersion()", MB_OK | MB_TOPMOST);
//			return CLEO_4_1;
//		}
//
//		/* Cleo 4.3 has the following bytes at 0x2289E offset:
//			FF D1 5D C2 */
//		GetData((void*)(dwCleo + 0x2289E), &signature, 4);
//
//		if (signature == 0xC25DD1FF) {
//			//MessageBoxA(NULL, "Cleo version = 4.3", "samp_commands.asi - GetCleoVersion()", MB_OK | MB_TOPMOST);
//			return CLEO_4_3;
//		}
//		
//		MessageBoxA(NULL, "Cleo version couldn't be determined.", "samp_commands.asi - GetCleoVersion()", MB_OK | MB_TOPMOST);
//		return NULL;
//
//	}
//	else {
//		MessageBoxA(NULL, "GetModuleHandle('cleo.asi') failed.", "samp_commands.asi - GetCleoVersion()", MB_OK | MB_TOPMOST);
//		return NULL;
//	}
//}