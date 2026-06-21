// eets_ui.h - tiny immediate-mode UI for custom menus in Eets mods.
//
// Header-only; each mod that includes it gets its own UI state. Build menus in
// EetsMod_Update(), and forward mouse events from EetsMod_OnMouse():
//
//   #include "eets_ui.h"
//   static bool g_open = true, g_god = false;
//   extern "C" void EetsMod_OnMouse(int x,int y,int b,int down){ Eets::UI::FeedMouse(x,y,b,down); }
//   extern "C" void EetsMod_Update() {
//       if (!g_open) return;
//       Eets::UI::Begin(40, 40, 220, "My Menu");
//       if (Eets::UI::Button("Spawn whale")) Eets::World_CreateObject("whale", {500,300});
//       Eets::UI::Toggle("God mode", g_god);
//       Eets::UI::Label("hello");
//       Eets::UI::End();
//   }
#pragma once
#include "eets_engine.h"

namespace Eets { namespace UI {

struct State {
	int   px = 0, py = 0, pw = 0;     // panel origin + width
	int   cy = 0;                     // current widget y
	int   pad = 8, rowh = 26;
	int   mx = 0, my = 0;             // mouse (render space)
	bool  down = false, prevDown = false;
	bool  clicked = false;            // left-press edge this frame
	int   clickX = 0, clickY = 0;
};
inline State& S() { static State s; return s; }

// Call from EetsMod_OnMouse. button EMBTN_LEFT press latches a click edge.
inline void FeedMouse(int x, int y, int button, int down) {
	State& s = S();
	s.mx = x; s.my = y;
	if (button == EMBTN_LEFT) {
		bool d = down != 0;
		if (d && !s.prevDown) { s.clicked = true; s.clickX = x; s.clickY = y; }
		s.prevDown = d; s.down = d;
	}
}

inline bool hit(int x, int y, int w, int h) {
	State& s = S();
	return s.clickX >= x && s.clickX < x + w && s.clickY >= y && s.clickY < y + h;
}
inline bool hover(int x, int y, int w, int h) {
	State& s = S();
	return s.mx >= x && s.mx < x + w && s.my >= y && s.my < y + h;
}

// Begin a panel at (x,y) width w with a title bar.
inline void Begin(int x, int y, int w, const char* title = nullptr) {
	State& s = S();
	s.px = x; s.py = y; s.pw = w; s.cy = y + s.pad;
	// (height is unknown up-front; draw a generous backing then title)
	FillRect(x, y, w, 400, Colour(20, 20, 28, 210));
	DrawRect(x, y, w, 400, Colour(90, 90, 120, 255), 2.0f);
	if (title) {
		FillRect(x, y, w, s.rowh, Colour(50, 50, 80, 235));
		DrawTextOutlined(x + s.pad, y + 5, title, FONT_NORMAL, Colour(255, 255, 255, 255));
		s.cy = y + s.rowh + s.pad;
	}
}

inline bool Button(const char* label) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2 * s.pad, h = s.rowh;
	bool hov = hover(x, y, w, h);
	FillRect(x, y, w, h, hov ? Colour(80, 120, 200, 255) : Colour(55, 60, 80, 255));
	DrawRect(x, y, w, h, Colour(120, 130, 160, 255), 1.0f);
	DrawTextOutlined(x + 8, y + 5, label, FONT_NORMAL, Colour(255, 255, 255, 255));
	s.cy += h + 6;
	bool clicked = s.clicked && hit(x, y, w, h);
	if (clicked) s.clicked = false;   // consume
	return clicked;
}

// Toggle: flips `value` when clicked; returns the (possibly changed) value.
inline bool Toggle(const char* label, bool& value) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2 * s.pad, h = s.rowh, box = h - 8;
	bool hov = hover(x, y, w, h);
	if (hov) FillRect(x, y, w, h, Colour(45, 50, 70, 180));
	FillRect(x + 2, y + 4, box, box, value ? Colour(90, 220, 120, 255) : Colour(60, 60, 70, 255));
	DrawRect(x + 2, y + 4, box, box, Colour(150, 150, 170, 255), 1.0f);
	DrawTextOutlined(x + box + 10, y + 5, label, FONT_NORMAL, Colour(255, 255, 255, 255));
	s.cy += h + 6;
	if (s.clicked && hit(x, y, w, h)) { s.clicked = false; value = !value; return value; }
	return value;
}

inline void Label(const char* text, Colour c = Colour(220, 220, 220, 255)) {
	State& s = S();
	DrawTextOutlined(s.px + s.pad, s.cy + 3, text, FONT_NORMAL, c);
	s.cy += s.rowh - 4;
}

// Horizontal slider in [lo,hi]; drag with left button held. Returns value.
inline float Slider(const char* label, float& value, float lo, float hi) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2 * s.pad, h = s.rowh;
	DrawTextOutlined(x, y, label, FONT_TINY, Colour(220, 220, 220, 255));
	int by = y + 14, bh = 8;
	FillRect(x, by, w, bh, Colour(40, 40, 55, 255));
	float t = (value - lo) / (hi - lo); if (t < 0) t = 0; if (t > 1) t = 1;
	FillRect(x, by, (int)(w * t), bh, Colour(90, 160, 220, 255));
	if (s.down && hover(x, by - 6, w, bh + 12)) {
		float nt = (float)(s.mx - x) / (float)w; if (nt < 0) nt = 0; if (nt > 1) nt = 1;
		value = lo + nt * (hi - lo);
	}
	s.cy += h + 6;
	return value;
}

inline void End() { S().clicked = false; }   // drop any unconsumed click

}} // namespace Eets::UI
