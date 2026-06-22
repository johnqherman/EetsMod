// drawtest - validate EString -> TextPrinter_DrawString (sized/styled text, SSO + heap paths). CTRL+G toggles.
#include "eetsmod.h"
using namespace Eets;

static bool g_on = true;

extern "C" void EetsMod_Init() { Eets::Log("drawtest: CTRL+G toggles sized-text canary"); }
extern "C" void EetsMod_OnKey(int key, int, int down) { if (down && (key=='g'||key=='G')) g_on = !g_on; }

extern "C" void EetsMod_Update() {
	if (!g_on || !GraphicsEngine_i()) return;
	FillRect(40, 40, 360, 200, Color(0, 0, 0, 160));
	// SSO path (<16 chars) at four sizes
	DrawTextSized(50,  50, "tiny",   FONT_TINY,   Color(255,255,255,255));
	DrawTextSized(50,  70, "small",  FONT_SMALL,  Color(120,255,120,255));
	DrawTextSized(50,  95, "normal", FONT_NORMAL, Color(120,200,255,255));
	DrawTextSized(50, 125, "BIG",    FONT_BIG,    Color(255,232,40,255));
	// heap path (>=16 chars -> exercises the _Ptr branch) + outline + style
	DrawTextOutlined(50, 165, "outlined long string >16 chars", FONT_NORMAL, Color(255,255,255,255));
	DrawTextSized(50, 195, "geek style typeface here", FONT_NORMAL, Color(255,160,200,255), STYLE_GEEK);
}
