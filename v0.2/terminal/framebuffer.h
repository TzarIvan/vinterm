#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <set>
#include <vector>
#include <istream>
using namespace std;

#include "terminal/charattr.h"
class Options;

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;
typedef enum { 
	NONE, HIGHLIGHT, UNDERLINE, BLINK, REVERSE, DIM, INVISIBLE
} AttrType;

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	// write on the screen
	void Put(const char c, bool ignore_insert_mode=true);
	void Put(const char c, const int x, const int y, 
			bool ignore_insert_mode=true);
	void Put(const char c, Attribute attr, const int x, const int y,
			bool ignore_insert_mode=true);

	// methods that move the cursor
	void AdvanceCursorY();
	void RecedeCursorY();
	void CarriageReturn();
	void Tab();
	void Backspace();
	void MoveCursor(Direction dir, int moves);
	void SetCursorPosition(int x, int y);

	// scrolling methods
	void ScrollUp();
	void ScrollDown();
	void SetScrollingRegion(int top, int bottom);

	// add text
	void AddLinesBelowCursor(int n);

	// clear text
	void ClearRow(bool upto_cursor, int y=-1);
	void DeleteLines(int n);
	void DeleteChars(int n);
	void EraseChars(int n);

	// save/restore screen
	void SaveScreen();
	void RestoreScreen();

	// insert
	void InsertChars(int n);

	// bells
	void Flash(bool reverse=true);
	bool Flashing() const { return flashing; }

	// information about the framebuffer
	inline int W() const { return w; }
	inline int H() const { return h; }
	inline int CursorX() const { return cursor_x; }
	inline int CursorY() const { return cursor_y; }
	inline Char Ch(int x, int y) const { return chars[x+(y*W())]; }

	// attributes
	void RegisterBlinks() const;
	void SetAttr(AttrType attr, bool value);

	mutable set<int>* dirty;
	int InsertMode;

private:
	void ValidateCursorPosition();

	Attribute current_attr;
	int w, h;
	int cursor_x, cursor_y;
	int scroll_top, scroll_bottom;
	vector<Char> chars, saved_screen;
	int saved_x, saved_y;
	set<int> tabs;
	bool flashing;
};

#endif
