#include "pch.h"
#include "Functions.h"
#include <Windows.h>

void SetData(void* pAddress, void* pData, size_t size) {
	DWORD dwOldProtect;
	VirtualProtect(pAddress, size, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy(pAddress, pData, size);
	VirtualProtect(pAddress, size, dwOldProtect, &dwOldProtect);
}

void GetData(void* pAddress, void* pData, size_t size) {
	DWORD dwOldProtect;
	VirtualProtect(pAddress, size, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy(pData, pAddress, size);
	VirtualProtect(pAddress, size, dwOldProtect, &dwOldProtect);
}

bool StartsWith(const char* main_text, const char* starting_part) {
	while (*starting_part) {
		if (*main_text++ != *starting_part++) {
			return false;
		}
	}
	return true;
}

