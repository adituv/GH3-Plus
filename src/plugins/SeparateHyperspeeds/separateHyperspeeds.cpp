#include "gh3\GH3Keys.h"
#include "core\Patcher.h"
#include "SeparateHyperspeeds.h"
#include <stdint.h>
#include <fstream>

static GH3P::Patcher g_patcher = GH3P::Patcher(__FILE__);

static void * const hyperspeedDetour = (void *)0x004259A6;
static float* const hyperspeedPlayer1 = (float*)0x00A12B90;
static float* const hyperspeedPlayer2 = (float*)0x00A12B94;

static float player2Setting = 1.0f;

__declspec(naked) void hyperspeedOverrideNaked() {
	static const uint32_t returnAddress = 0x004259B6;

	__asm {
		mov		eax, KEY_HAMMER_TIME_EARLY;
		push	edi;
		push	eax;
		movss	hyperspeedPlayer1, xmm0;
		movss   xmm0, player2Setting;
		movss   hyperspeedPlayer2, xmm0;
		jmp     returnAddress;
	}
}

void ApplyHack() {
	return;
}

/*
void ApplyHack() {
	std::ifstream ini;
	try {
		ini = std::ifstream("hyperspeed.dat");
		ini >> player2Setting;
		ini.close();

		g_patcher.WriteJmp(hyperspeedDetour, hyperspeedOverrideNaked);
	}
	catch (...) {
		ini.close();
	}
}
*/
