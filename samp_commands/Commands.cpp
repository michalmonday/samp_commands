#include "pch.h"
#include "Commands.h"

size_t GetParamsLength(const char* typed_command, const char* registered_prefix);

namespace Commands {
	char* last_params = nullptr;
	bool last_had_params = false;
	Command* last = nullptr;
	bool last_requested_return = false;

	std::vector<Command*> vect;


	void Init() {
		last_params = (char*)malloc(300);
	}

	void SaveParams(const char* typed_command, const char* registered_prefix) {
		last_had_params = false;
		if (size_t params_length = GetParamsLength(typed_command, registered_prefix)) {
			last_had_params = true;
			last_params = (char*)realloc(last_params, params_length + 1);
			strcpy(last_params, typed_command + strlen(registered_prefix) + 1);
		}
	}
}


size_t GetParamsLength(const char* typed_command, const char* registered_prefix) {
	size_t s1 = strlen(typed_command);
	size_t s2 = strlen(registered_prefix);
	size_t params_size = s1 - s2 - 1; // 1 beause space must separate command from params

	if (s1 < s2) return 0;
	if (typed_command[s2] != ' ') return 0;

	return (params_size > 0 ? params_size : 0);
}





