#pragma once

void SetData(void* pAddress, void* pData, size_t size);
void GetData(void* pAddress, void* pData, size_t size);
bool StartsWith(const char* main_text, const char* starting_part);
size_t GetParamsLength(const char* typed_command, const char* registered_prefix);