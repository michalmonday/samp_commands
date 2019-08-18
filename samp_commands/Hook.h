#pragma once

namespace Hook {
	void Attach();
	void Detach();
}

extern DWORD origChatInputHandler;