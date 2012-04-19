#ifndef PTY_H
#define PTY_H

#include <string>
#include <ostream>
using namespace std;

class Options;

class PTY
{
public:
	enum { NO_DATA = -100 };

	PTY(Options const& options, const string terminal="vinterm");
	~PTY();

	int Get() const;
	void Send(const char c);
	void Send(string s);
	void Resize(uint16_t w, uint16_t h);

private:
	void CopyStartupFile() const;
	void OpenPTY(string const& terminal);
	void Debug(char c, bool sending) const;

	int fd;
	mutable bool active;
	Options const& options;
	const bool debug;
	mutable int debug_ct;
};

#endif
