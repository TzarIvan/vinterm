#ifndef TERMINAL_H
#define TERMINAL_H

#include <iconv.h>
#include <string>
using namespace std;

class Framebuffer;
class Screen;
class PTY;
class Options;

class Terminal
{
public:
	Terminal(Framebuffer& fb, PTY& pty, Options const& options);
	virtual ~Terminal();

	void Input();
	void Output(Screen& screen);
	void Resize(int new_w, int new_h);
	void SetEncoding(string const& encoding);

	virtual void ExecuteEscapeSequence(string const& sequence);
	virtual void KeyPressed(int key);

	bool Active() const { return active; }

protected:
	Framebuffer& fb;
	PTY& pty;
	Options const& options;

private:
	void InputChar(const char c);
	void InputEscapeChar(const char c);
	char ConvertByte(const char c);

	bool active;
	bool escape_mode;
	string escape_sequence;
	string encoding;

	iconv_t cd_in, cd_out;
	char* inbuf;
	size_t inbuf_pos;
};

#endif