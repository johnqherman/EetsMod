#pragma once
#include "eets_engine.h"

namespace Eets { namespace UI {

namespace col {
	inline Colour black()     { return Colour(0, 0, 0, 255); }
	inline Colour shadow()    { return Colour(0, 0, 0, 110); }
	inline Colour panel()     { return Colour(205, 40, 35, 255); }
	inline Colour titlebar()  { return Colour(165, 22, 20, 255); }
	inline Colour title()     { return Colour(255, 232, 40, 255); }
	inline Colour btn()       { return Colour(230, 58, 48, 255); }
	inline Colour btnHover()  { return Colour(255, 120, 55, 255); }
	inline Colour textC()     { return Colour(255, 255, 255, 255); }
	inline Colour textHover() { return Colour(255, 232, 40, 255); }
	inline Colour label()     { return Colour(255, 255, 255, 255); }
	inline Colour cream()     { return Colour(255, 224, 193, 255); }
	inline Colour on()        { return Colour(255, 210, 40, 255); }
	inline Colour off()       { return Colour(120, 20, 18, 255); }
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
	FillRect(x + 6, y + 7, w, H, col::shadow());
	FillRect(x, y, w, H, col::panel());
	DrawRect(x, y, w, H, col::black(), 4.0f);
	if (title) {
		FillRect(x + 4, y + 4, w - 8, s.rowh + 2, col::titlebar());
		FillRect(x + 4, y + s.rowh + 4, w - 8, 3, col::black());
		DrawTextOutlined(x + s.pad, y + 7, title, FONT_BIG, col::title());
		s.cy = y + s.rowh + s.pad + 6;
	} else {
		s.cy = y + s.pad;
	}
}

inline bool Button(const char* label) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2*s.pad, h = s.rowh + 4;
	bool hov = hover(x, y, w, h);
	FillRect(x, y, w, h, hov ? col::btnHover() : col::btn());
	DrawRect(x, y, w, h, col::black(), 3.0f);
	DrawTextOutlined(x + 12, y + 7, label, FONT_NORMAL, hov ? col::textHover() : col::textC());
	s.cy += h + 8;
	bool c = s.clicked && hit(x, y, w, h);
	if (c) s.clicked = false;
	return c;
}

inline bool Toggle(const char* label, bool& value) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2*s.pad, h = s.rowh + 4, box = h - 8;
	bool hov = hover(x, y, w, h);
	FillRect(x, y + 4, box, box, value ? col::on() : col::cream());
	DrawRect(x, y + 4, box, box, col::black(), 3.0f);
	DrawTextOutlined(x + box + 12, y + 7, label, FONT_NORMAL, hov ? col::textHover() : col::textC());
	s.cy += h + 8;
	if (s.clicked && hit(x, y, w, h)) { s.clicked = false; value = !value; return value; }
	return value;
}

inline void Label(const char* text) {
	State& s = S();
	DrawTextOutlined(s.px + s.pad, s.cy + 3, text, FONT_SMALL, col::label());
	s.cy += s.rowh - 4;
}

inline float Slider(const char* label, float& value, float lo, float hi) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2*s.pad, h = s.rowh + 12;
	DrawTextOutlined(x, y, label, FONT_SMALL, col::label());
	int by = y + 20, bh = 14;
	FillRect(x, by, w, bh, col::off());
	float t = (value - lo) / (hi - lo); if (t < 0) t = 0; if (t > 1) t = 1;
	FillRect(x, by, (int)(w * t), bh, col::on());
	DrawRect(x, by, w, bh, col::black(), 3.0f);
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
