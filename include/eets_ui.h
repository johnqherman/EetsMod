#pragma once
#include "eets_engine.h"

namespace Eets { namespace UI {

namespace col {
	inline Color black()     { return Color(0, 0, 0, 255); }
	inline Color shadow()    { return Color(0, 0, 0, 110); }
	inline Color panel()     { return Color(205, 40, 35, 255); }
	inline Color titlebar()  { return Color(165, 22, 20, 255); }
	inline Color title()     { return Color(255, 232, 40, 255); }
	inline Color btn()       { return Color(230, 58, 48, 255); }
	inline Color btnHover()  { return Color(255, 120, 55, 255); }
	inline Color textC()     { return Color(255, 255, 255, 255); }
	inline Color textHover() { return Color(255, 232, 40, 255); }
	inline Color label()     { return Color(255, 255, 255, 255); }
	inline Color cream()     { return Color(255, 224, 193, 255); }
	inline Color on()        { return Color(255, 210, 40, 255); }
	inline Color off()       { return Color(120, 20, 18, 255); }
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
	// column layout (BeginColumns/NextColumn/EndColumns)
	int   colCount = 0, colIdx = 0, colGap = 8, colW = 0;
	int   colOrigPx = 0, colOrigPw = 0, colStartY = 0, colMaxY = 0;
	const char* clickSound = nullptr;   // played when a Button/Toggle is activated (null = silent)
	const char* hoverSound = nullptr;   // played once when the mouse enters a Button/Toggle (null = silent)
	int   hoverId = -1, curHoverId = -1;   // widget under the mouse last frame / this frame (position-derived id)
};
inline State& S() { static State s; return s; }

// set the sound played on a Button/Toggle click (a game sfx name, e.g. "GUI Click 1"); null = silent
inline void SetClickSound(const char* name) { S().clickSound = name; }
// set the sound played once when the mouse first enters a Button/Toggle (e.g. "GUI MouseOver"); null = silent
inline void SetHoverSound(const char* name) { S().hoverSound = name; }
// call when a widget is hovered: plays the hover sfx on enter (id differs from last frame's hovered widget)
inline void noteHover(int x, int y) {
	State& s = S(); int id = y * 4096 + x; s.curHoverId = id;
	if (s.hoverSound && id != s.hoverId) PlaySound(s.hoverSound);
}

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
	s.hoverId = s.curHoverId; s.curHoverId = -1;   // roll over hover tracking for this frame
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
	if (hov) noteHover(x, y);
	FillRect(x, y, w, h, hov ? col::btnHover() : col::btn());
	DrawRect(x, y, w, h, col::black(), 3.0f);
	DrawTextOutlined(x + 12, y + 7, label, FONT_NORMAL, hov ? col::textHover() : col::textC());
	s.cy += h + 8;
	bool c = s.clicked && hit(x, y, w, h);
	if (c) { s.clicked = false; if (s.clickSound) PlaySound(s.clickSound); }
	return c;
}

inline bool Toggle(const char* label, bool& value) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2*s.pad, h = s.rowh + 4, box = h - 8;
	bool hov = hover(x, y, w, h);
	if (hov) noteHover(x, y);
	FillRect(x, y + 4, box, box, value ? col::on() : col::cream());
	DrawRect(x, y + 4, box, box, col::black(), 3.0f);
	DrawTextOutlined(x + box + 12, y + 7, label, FONT_NORMAL, hov ? col::textHover() : col::textC());
	s.cy += h + 8;
	if (s.clicked && hit(x, y, w, h)) { s.clicked = false; value = !value; if (s.clickSound) PlaySound(s.clickSound); return value; }
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

// a section header with an underline rule - groups related rows
inline void Section(const char* label) {
	State& s = S();
	s.cy += 6;
	DrawTextOutlined(s.px + s.pad, s.cy, label, FONT_NORMAL, col::title());
	s.cy += 22;
	FillRect(s.px + s.pad, s.cy, s.pw - 2 * s.pad, 2, col::black());
	s.cy += 7;
}

// a thin divider rule
inline void Separator() {
	State& s = S();
	s.cy += 4;
	FillRect(s.px + s.pad, s.cy, s.pw - 2 * s.pad, 2, col::shadow());
	s.cy += 6;
}

// split the current width into N side-by-side columns (rows added between Begin/Next/End stack per
// column). Keeps a tall menu on-screen. Always pair with matching NextColumn()/EndColumns().
inline void BeginColumns(int n, int gap = 10) {
	State& s = S();
	if (n < 1) n = 1;
	s.colCount = n; s.colIdx = 0; s.colGap = gap;
	s.colOrigPx = s.px; s.colOrigPw = s.pw; s.colStartY = s.cy; s.colMaxY = s.cy;
	int inner = s.pw - 2 * s.pad;
	s.colW = (inner - (n - 1) * gap) / n;
	s.pw = s.colW + 2 * s.pad;          // widget width becomes one column wide
}
inline void NextColumn() {
	State& s = S();
	if (s.cy > s.colMaxY) s.colMaxY = s.cy;
	if (s.colIdx < s.colCount - 1) s.colIdx++;
	s.px = s.colOrigPx + s.colIdx * (s.colW + s.colGap);
	s.cy = s.colStartY;
}
inline void EndColumns() {
	State& s = S();
	if (s.cy > s.colMaxY) s.colMaxY = s.cy;
	s.px = s.colOrigPx; s.pw = s.colOrigPw; s.cy = s.colMaxY;
	s.colCount = 0;
}

inline void End() {
	State& s = S();
	s.lastH = (s.cy - s.top) + s.pad;   // remember height for next frame's bg
	s.clicked = false;
}

}} // namespace Eets::UI
