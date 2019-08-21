#pragma once
#include <vector>

#include "Cleo.h"


struct Command {
	char* registered_prefix;
	DWORD label_address; // equivalent to: 0B34: samp register_client_command "str" @label_address
	CLEO_RunningScript* cleo;

	DWORD return_address;

	Command(char* s, DWORD la, CLEO_RunningScript* c, DWORD ra) {
		registered_prefix = (char*)malloc(strlen(s) + 1);
		strcpy(registered_prefix, s);
		label_address = la;
		cleo = c;
		return_address = ra;
	}

	~Command() {
		free(registered_prefix);
	}
};

namespace Commands {
	extern char* last_params;
	extern bool last_had_params;
	extern Command* last;
	extern bool last_requested_return;
	extern std::vector<Command*> vect;
	extern void Init();
	extern void SaveParams(const char* typed_command, const char* registered_prefix);
}


