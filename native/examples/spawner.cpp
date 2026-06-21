// spawner - spawn at cursor. left-click spawn, right-click explosion, wheel cycle, F2 label.
#include "eetsmod.h"
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
using namespace Eets;

static std::vector<std::string> g_blueprints = {
	"marsh_bud_happy", "marsh_bud_angry", "giant_marshmallow",
	"whale", "choco_cloud", "star", "butterfly", "cart_tnt",
};
static int  g_sel = 0;
static bool g_label = true;
static int  g_spawned = 0;

static bool in_game() { return !World_IsInMainMenu(); }

extern "C" void EetsMod_Init() {
	const char* list = Eets::ConfigGet("spawner", "blueprints", nullptr);
	if (list) {
		g_blueprints.clear();
		std::string s = list, cur;
		for (char ch : s) { if (ch == ',') { if (!cur.empty()) g_blueprints.push_back(cur); cur.clear(); } else if (ch != ' ') cur += ch; }
		if (!cur.empty()) g_blueprints.push_back(cur);
	}
	Eets::Log("spawner: ready - left-click spawns, wheel cycles, right-click booms (%zu blueprints)", g_blueprints.size());
}

extern "C" void EetsMod_OnWheel(int, int dy) {
	if (g_blueprints.empty()) return;
	g_sel = (g_sel + (dy > 0 ? 1 : -1) + (int)g_blueprints.size()) % (int)g_blueprints.size();
	Eets::Log("spawner: selected %s", g_blueprints[g_sel].c_str());
}

extern "C" void EetsMod_OnMouse(int x, int y, int button, int down) {
	if (!down || !in_game()) return;
	Vector2 p{(float)x, (float)y};
	if (button == EMBTN_LEFT && !g_blueprints.empty()) {
		World_CreateObject(g_blueprints[g_sel].c_str(), p);
		Eets::Log("spawner: spawned %s @ %d,%d", g_blueprints[g_sel].c_str(), x, y);
	} else if (button == EMBTN_RIGHT) {
		World_CreateExplosion(p, 200.0f);
	}
}

extern "C" void EetsMod_OnKey(int key, int, int down) {
	if (down && key == EKEY_F2) g_label = !g_label;
}

extern "C" void EetsMod_OnEvent(const char* name, void* /*a*/, void* /*b*/) {
	if (strcmp(name, "object_spawn") == 0) g_spawned++;
}

extern "C" void EetsMod_Update() {
	if (!g_label || !in_game() || g_blueprints.empty()) return;
	char buf[96];
	snprintf(buf, sizeof(buf), "spawn: %s  (%d total)", g_blueprints[g_sel].c_str(), g_spawned);
	DrawTextOutlined(MouseX() + 16, MouseY(), buf, FONT_NORMAL, Colour(255, 255, 0, 255));
}
