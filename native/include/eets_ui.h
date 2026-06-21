// eets_ui.h - tiny immediate-mode UI for custom menus in Eets mods.
// Styled to match the in-game GUI: geekabyte font, dropshadow text, warm panel
// with a cream border and a maroon title bar, yellow/green highlights.
//
//   #include "eets_ui.h"
//   static bool g_open = true, g_god = false;
//   extern "C" void EetsMod_OnMouse(int x,int y,int b,int down){ Eets::UI::FeedMouse(x,y,b,down); }
//   extern "C" void EetsMod_Update() {
//       if (!g_open) return;
//       Eets::UI::Begin(40, 40, 240, "My Menu");
//       if (Eets::UI::Button("Spawn whale")) Eets::World_CreateObject("whale", {500,300});
//       Eets::UI::Toggle("God mode", g_god);
//       Eets::UI::End();
//   }
#pragma once
#include "eets_engine.h"

namespace Eets { namespace UI {

// ---- Eets palette ----------------------------------------------------------
namespace col {
	inline Colour shadow()    { return Colour(0, 0, 0, 130); }
	inline Colour panel()     { return Colour(48, 32, 40, 230); }
	inline Colour border()    { return Colour(235, 205, 150, 255); }   // cream/gold
	inline Colour titlebar()  { return Colour(150, 70, 90, 245); }     // berry
	inline Colour title()     { return Colour(255, 255, 0, 255); }     // yellow
	inline Colour btn()       { return Colour(70, 50, 60, 255); }
	inline Colour btnHover()  { return Colour(150, 90, 70, 255); }
	inline Colour btnBorder() { return Colour(200, 170, 120, 255); }
	inline Colour textC()     { return Colour(255, 255, 255, 255); }
	inline Colour textHover() { return Colour(255, 255, 0, 255); }     // yellow
	inline Colour label()     { return Colour(235, 225, 210, 255); }
	inline Colour on()        { return Colour(90, 220, 120, 255); }    // green
	inline Colour off()       { return Colour(60, 55, 60, 255); }
}

struct State {
	int   px = 0, py = 0, pw = 0;
	int   cy = 0, top = 0;
	int   pad = 10, rowh = 30;
	int   lastH = 80;                 // measured last frame (for the bg)
	int   mx = 0, my = 0;
	bool  down = false, prevDown = false;
	bool  clicked = false;
	int   clickX = 0, clickY = 0;
};
inline State& S() { static State s; return s; }

inline void FeedMouse(int x, int y, int button, int down) {
	State& s = S();
	s.mx = x; s.my = y;
	if (button == EMBTN_LEFT) {
		bool d = down != 0;
		if (d && !s.prevDown) { s.clicked = true; s.clickX = x; s.clickY = y; }
		s.prevDown = d; s.down = d;
	}
}
inline bool hit(int x, int y, int w, int h)  { State& s = S(); return s.clickX>=x && s.clickX<x+w && s.clickY>=y && s.clickY<y+h; }
inline bool hover(int x, int y, int w, int h){ State& s = S(); return s.mx>=x && s.mx<x+w && s.my>=y && s.my<y+h; }

inline void Begin(int x, int y, int w, const char* title = nullptr) {
	State& s = S();
	s.px = x; s.py = y; s.pw = w; s.top = y;
	int H = s.lastH;
	FillRect(x + 5, y + 6, w, H, col::shadow());          // drop shadow
	FillRect(x, y, w, H, col::panel());                   // panel
	DrawRect(x, y, w, H, col::border(), 2.0f);            // border
	if (title) {
		FillRect(x + 2, y + 2, w - 4, s.rowh, col::titlebar());
		DrawTextOutlined(x + s.pad, y + 6, title, FONT_NORMAL, col::title());
		s.cy = y + s.rowh + s.pad;
	} else {
		s.cy = y + s.pad;
	}
}

inline bool Button(const char* label) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2*s.pad, h = s.rowh;
	bool hov = hover(x, y, w, h);
	FillRect(x, y, w, h, hov ? col::btnHover() : col::btn());
	DrawRect(x, y, w, h, col::btnBorder(), 1.0f);
	DrawTextOutlined(x + 10, y + 6, label, FONT_NORMAL, hov ? col::textHover() : col::textC());
	s.cy += h + 8;
	bool c = s.clicked && hit(x, y, w, h);
	if (c) s.clicked = false;
	return c;
}

inline bool Toggle(const char* label, bool& value) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2*s.pad, h = s.rowh, box = h - 10;
	bool hov = hover(x, y, w, h);
	if (hov) FillRect(x, y, w, h, Colour(150, 90, 70, 120));
	FillRect(x + 2, y + 5, box, box, value ? col::on() : col::off());
	DrawRect(x + 2, y + 5, box, box, col::btnBorder(), 1.0f);
	DrawTextOutlined(x + box + 12, y + 6, label, FONT_NORMAL, hov ? col::textHover() : col::textC());
	s.cy += h + 8;
	if (s.clicked && hit(x, y, w, h)) { s.clicked = false; value = !value; return value; }
	return value;
}

inline void Label(const char* text) {
	State& s = S();
	DrawTextOutlined(s.px + s.pad, s.cy + 3, text, FONT_SMALL, col::label());
	s.cy += s.rowh - 6;
}

inline float Slider(const char* label, float& value, float lo, float hi) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2*s.pad, h = s.rowh + 8;
	DrawTextOutlined(x, y, label, FONT_SMALL, col::label());
	int by = y + 18, bh = 10;
	FillRect(x, by, w, bh, col::off());
	float t = (value - lo) / (hi - lo); if (t < 0) t = 0; if (t > 1) t = 1;
	FillRect(x, by, (int)(w * t), bh, Colour(230, 180, 90, 255));
	DrawRect(x, by, w, bh, col::btnBorder(), 1.0f);
	if (s.down && hover(x, by - 8, w, bh + 16)) {
		float nt = (float)(s.mx - x) / (float)w; if (nt < 0) nt = 0; if (nt > 1) nt = 1;
		value = lo + nt * (hi - lo);
	}
	s.cy += h + 6;
	return value;
}

inline void End() {
	State& s = S();
	s.lastH = (s.cy - s.top) + s.pad;   // remember height for next frame's bg
	s.clicked = false;
}

}} // namespace Eets::UI
