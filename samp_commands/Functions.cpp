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

size_t GetParamsLength(const char* typed_command, const char* registered_prefix) {
	size_t s1 = strlen(typed_command);
	size_t s2 = strlen(registered_prefix);
	size_t params_size = s1 - s2 - 1; // 1 beause space must separate command from params

	if (s1 < s2) return 0;
	if (typed_command[s2] != ' ') return 0;

	return (params_size > 0 ? params_size : 0);
}