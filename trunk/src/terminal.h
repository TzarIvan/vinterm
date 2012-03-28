#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <set>
#include "SDL.h"
using namespace std;

#include "font.h"
#include "options.h"
#include "console.h"

typedef struct CharAttr {
	bool Reverse    : 1;
	bool Underline  : 1;
	bool Highlight  : 1;
	bool Blink      : 1;
	operator long int() const { return (Reverse << 3) + (Underline << 2) + (Highlight << 1) + Blink; }
	const int Max() const { return 0xf; }
} CharAttr;
const static CharAttr NORMAL = { 0, 0, 0, 0 };


typedef struct TerminalChar {
	TerminalChar() : ch(' '), attr(NORMAL), cursor(false) { }
	uint8_t ch;
	CharAttr attr;
	bool cursor;
} TerminalChar;


class Terminal
{
public:
	explicit Terminal(Options const& options, string const& term="console");
	virtual ~Terminal();

	bool Process();

	TerminalChar Ch(int x, int y) const { return ch[x][y]; }
	bool BlinkOn() const { return blink_on; }

	const int w, h;
	mutable set<int> dirty;

protected:
	virtual void ExecuteEscapeSequence(string const& s);
	void InvalidEscapeSequence(string const& s);
	void UpdateCursorPosition();
	void SetChar(const int x, const int y, 
			const uint8_t c, const CharAttr attr);
	void ScrollDown();
	void ScrollUp();

	int cursor_x, cursor_y;
	CharAttr current_attr;

private:
	bool ConsoleInput();
	bool ConsoleOutput();

	void KeyPress(uint16_t key);
	void PrintString(string const& s);
	void PrintChar(const uint8_t c);
	void AddEscapeChar(const uint8_t c);
	void AdvanceCursorX();
	void AdvanceCursorY(bool update=true);
	void Blink();

	Options const& options;
	Console* const console;
	TerminalChar** const ch;
	int old_cursor_x, old_cursor_y;
	bool blink_on;
	uint32_t last_blink;

	bool escape_mode;
	string escape_sequence;
};

#endif
