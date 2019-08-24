#include "pch.h"
#include "Commands.h"

#include <algorithm>    // std::find_if

size_t GetParamsLength(const char* typed_command, const char* registered_prefix);



namespace Commands {
	char* last_params = nullptr;
	bool last_had_params = false;
	Command* last = nullptr;
	bool last_requested_return = false;

	std::vector<Command*> vect;

	void Init() {
		last_params = (char*)calloc(1, 1000);
	}

	void SaveParams(const char* typed_command, const char* registered_prefix) {
		last_had_params = false;
		if (size_t params_length = GetParamsLength(typed_command, registered_prefix)) {
			last_had_params = true;
			//last_params = (char*)realloc(last_params, params_length + 1);
			strcpy(last_params, typed_command + strlen(registered_prefix) + 1);
		}
		else {
			*last_params = '\0';
		}
	}

	char cleo_name_if_registered[8];
	bool IsRegistered(const char* prefix) {
		auto result =  std::find_if(vect.begin(), vect.end(),
			[&p = prefix](Command * c) -> bool {
				return !strcmp(c->registered_prefix, p);			// lambda function
			});

		WORD index = result - vect.begin();
		if (index < vect.size()) {
			strncpy(cleo_name_if_registered, vect[index]->cleo->Name, 8);
			return true;
		}
		*cleo_name_if_registered = '\0';
		return false;
	}

	char* GetCleoNameWhoRegisteredFirst() {
		return cleo_name_if_registered;
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





