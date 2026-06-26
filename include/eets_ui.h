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
	inline Color gloss()     { return Color(255, 255, 255, 46); }   // glossy top highlight on buttons
}

// vanilla's menu button typeface = BradBunR ("brady"): upright, bubbly, cartoony - matches the in-game
// OPTIONS/QUIT/PUZZLE-MAKER buttons (confirmed by pixel comparison). The komika styles are slanted
// (wrong), and STYLE_GEEK renders '.' '+' '(' ')' as stars (mangles version strings). Brady handles
// punctuation fine.
constexpr int UI_FONT = STYLE_BRADY;

// FontPrintSizes -> pixel height, for vertically centering text within a row
inline int fontPx(int size) {
	switch (size) {
		case FONT_TINY:   return 13;
		case FONT_SMALL:  return 14;
		case FONT_BIG:    return 28;
		case FONT_HUGE:   return 35;
		default:          return 20;   // FONT_NORMAL
	}
}

// filled rounded rectangle: center cross of rects + a disc at each corner (cross-platform; FillCircle
// is scanline-built on Win). r is clamped to half the smaller side.
inline void FillRoundRect(int x, int y, int w, int h, int r, Color c) {
	if (r < 0) r = 0;
	if (r > w / 2) r = w / 2;
	if (r > h / 2) r = h / 2;
	if (r == 0) { FillRect(x, y, w, h, c); return; }
	FillRect(x + r, y, w - 2 * r, h, c);            // full-height center band
	FillRect(x, y + r, r, h - 2 * r, c);            // left edge
	FillRect(x + w - r, y + r, r, h - 2 * r, c);    // right edge
	FillCircle(x + r, y + r, (float)r, c);
	FillCircle(x + w - r - 1, y + r, (float)r, c);
	FillCircle(x + r, y + h - r - 1, (float)r, c);
	FillCircle(x + w - r - 1, y + h - r - 1, (float)r, c);
}

// puffy vanilla-style panel: thick black rounded border with a colored rounded fill inset, plus an
// optional glossy highlight across the top (gives the cartoony "inflated" button look)
inline void RoundPanel(int x, int y, int w, int h, int r, Color fill, int border = 4, bool glossy = false) {
	FillRoundRect(x, y, w, h, r, col::black());
	int ir = r - border; if (ir < 1) ir = 1;
	FillRoundRect(x + border, y + border, w - 2 * border, h - 2 * border, ir, fill);
	if (glossy) {
		int gh = (h - 2 * border) / 3; if (gh < 3) gh = 3;
		FillRoundRect(x + border + 2, y + border + 1, w - 2 * border - 4, gh, ir > 2 ? ir - 1 : 1, col::gloss());
	}
}

// vertically center a single text line of the given font within a row of height rowH, returning the
// top y the text helpers expect (pos.y = text top)
inline int centerY(int rowTop, int rowH, int size) { return rowTop + (rowH - fontPx(size)) / 2; }

// horizontally-centered text with a THICK uniform black outline (vanilla cartoon-button look, e.g. the
// QUIT/PLAY labels): the fill is stamped in `outline` color across a filled disc of radius t, then the
// fill color on top. t=3 gives a chunky ~3px outline. More draw calls than DrawTextCenteredOutlined's
// single drop-shadow, so use it for headline labels (buttons/titles), not body text.
inline void DrawTextCenteredHeavy(int cx, int y, const char* text, int size, Color fill,
                                  Color outline = col::black(), int t = 3, int style = UI_FONT) {
	for (int dy = -t; dy <= t; ++dy)
		for (int dx = -t; dx <= t; ++dx)
			if (dx * dx + dy * dy <= t * t) DrawTextCentered(cx + dx, y + dy, text, size, outline, style);
	DrawTextCentered(cx, y, text, size, fill, style);
}

// same chunky-outline centered text but at an ARBITRARY pixel size (px), so it scales smoothly for hover
// (e.g. px*1.1) and past the 35px enum ceiling - matching vanilla's big button labels. Uses DrawTextPx
// (the engine's own arbitrary-size path); if that's unresolved (Win until RE'd) it falls back to the
// nearest enum size so the button still draws.
inline void DrawTextPxHeavy(int cx, int y, const char* text, int px, Color fill,
                            Color outline = col::black(), int t = 3, int style = UI_FONT) {
	bool ok = false;
	for (int dy = -t; dy <= t; ++dy)
		for (int dx = -t; dx <= t; ++dx)
			if (dx * dx + dy * dy <= t * t) ok = DrawTextPx(cx + dx, y + dy, text, px, outline, true, style);
	if (ok) { DrawTextPx(cx, y, text, px, fill, true, style); return; }
	DrawTextCenteredHeavy(cx, y, text, px >= 32 ? FONT_HUGE : FONT_BIG, fill, outline, t, style);  // enum fallback
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
	FillRoundRect(x + 6, y + 8, w, H, 16, col::shadow());        // drop shadow
	RoundPanel(x, y, w, H, 16, col::panel(), 5);                 // puffy bordered panel
	if (title) {
		int tbH = s.rowh + 6;
		FillRoundRect(x + 6, y + 6, w - 12, tbH, 11, col::titlebar());
		DrawTextCenteredOutlined(x + w / 2, centerY(y + 6, tbH, FONT_BIG), title, FONT_BIG,
		                         col::title(), Color(0, 0, 0, 220), UI_FONT);
		s.cy = y + tbH + s.pad + 4;
	} else {
		s.cy = y + s.pad + 2;
	}
}

inline bool Button(const char* label) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2*s.pad, h = s.rowh + 10;
	bool hov = hover(x, y, w, h);
	if (hov) noteHover(x, y);
	RoundPanel(x, y, w, h, 12, hov ? col::btnHover() : col::btn(), 4, true);
	DrawTextCenteredOutlined(x + w / 2, centerY(y, h, FONT_NORMAL), label, FONT_NORMAL,
	                         hov ? col::textHover() : col::textC(), Color(0, 0, 0, 220), UI_FONT);
	s.cy += h + 9;
	bool c = s.clicked && hit(x, y, w, h);
	if (c) { s.clicked = false; if (s.clickSound) PlaySound(s.clickSound); }
	return c;
}

inline bool Toggle(const char* label, bool& value) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2*s.pad, h = s.rowh + 10, box = h - 12;
	bool hov = hover(x, y, w, h);
	if (hov) noteHover(x, y);
	RoundPanel(x, y, w, h, 12, hov ? col::btnHover() : col::btn(), 4, true);
	int bx = x + 8, by = y + (h - box) / 2;
	RoundPanel(bx, by, box, box, 6, value ? col::on() : col::cream(), 3);
	DrawTextOutlined(bx + box + 10, centerY(y, h, FONT_NORMAL), label, FONT_NORMAL,
	                 hov ? col::textHover() : col::textC(), Color(0, 0, 0, 220), UI_FONT);
	s.cy += h + 9;
	if (s.clicked && hit(x, y, w, h)) { s.clicked = false; value = !value; if (s.clickSound) PlaySound(s.clickSound); return value; }
	return value;
}

inline void Label(const char* text) {
	State& s = S();
	int rowH = s.rowh - 2;
	DrawTextOutlined(s.px + s.pad, centerY(s.cy, rowH, FONT_SMALL), text, FONT_SMALL,
	                 col::label(), Color(0, 0, 0, 200), UI_FONT);
	s.cy += rowH;
}

inline float Slider(const char* label, float& value, float lo, float hi) {
	State& s = S();
	int x = s.px + s.pad, y = s.cy, w = s.pw - 2*s.pad, h = s.rowh + 14;
	DrawTextOutlined(x, y, label, FONT_SMALL, col::label(), Color(0, 0, 0, 200), UI_FONT);
	int by = y + 22, bh = 14;
	RoundPanel(x, by, w, bh, 7, col::off(), 3);
	float t = (value - lo) / (hi - lo); if (t < 0) t = 0; if (t > 1) t = 1;
	int fw = (int)(w * t);
	if (fw > 4) FillRoundRect(x + 3, by + 3, fw - 6 > 1 ? fw - 6 : 1, bh - 6, 4, col::on());
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
	DrawTextOutlined(s.px + s.pad, s.cy, label, FONT_NORMAL, col::title(), Color(0, 0, 0, 220), UI_FONT);
	s.cy += 24;
	FillRoundRect(s.px + s.pad, s.cy, s.pw - 2 * s.pad, 3, 1, col::black());
	s.cy += 8;
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
