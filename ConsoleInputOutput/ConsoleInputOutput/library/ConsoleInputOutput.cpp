#include "ConsoleInputOutput.h"

#ifdef _MSC_VER

#include <conio.h>

#else

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

#endif

namespace consoleio {

#ifndef _MSC_VER
	static struct termios old_termios;
	static bool initialized = false;
	static const int STDIN_DESCRIPTOR_ID = 0;

	static void restore() {
		tcsetattr(STDIN_DESCRIPTOR_ID, TCSANOW, &old_termios);
	}
	
	static void initialize() {
		if (!initialized) {
			initialized = true;

			// Get the file descriptor for standard input terminal
			tcgetattr(STDIN_DESCRIPTOR_ID, &old_termios);

			// Create a copy of the descriptor to work on
			struct termios new_termios;
			memcpy(&new_termios, &old_termios, sizeof(new_termios));

			// Restore old descriptor on exit
			atexit(restore);

			// Unset the ECHO and ICANON flags, and set the descriptor
			// Unsetting the ECHO flag disables the output of characters to terminal
			// when they are typed
			// Unsetting the ICANON flag makes the read function read characters directly
			// from console, without the need for a newline
			new_termios.c_lflag &= ~(ICANON | ECHO);
			tcsetattr(STDIN_DESCRIPTOR_ID, TCSANOW, &new_termios);
		}
	}
#endif

	bool keyboardHit() {
#ifdef _MSC_VER
		return _kbhit();
#else
		initialize();
		struct timeval tv = { 0L, 0L };
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(0, &fds);
		return select(1, &fds, NULL, NULL, &tv);
#endif
	}

	int getCharacter() {
#ifdef _MSC_VER
		return _getch();
#else 
		initialize();
		int r;
		unsigned char c;
		if ((r = read(0, &c, sizeof(c))) < 0) {
			return r;
		}
		else {
			return c;
		}
#endif
	}
}
