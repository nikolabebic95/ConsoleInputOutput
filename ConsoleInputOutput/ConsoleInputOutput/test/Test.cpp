#include "../library/ConsoleInputOutput.h"
#include <iostream>

using namespace consoleio;

int main() {
	while (true) {
		// ReSharper disable once CppPossiblyErroneousEmptyStatements
		while (!keyboardHit());
		char c = getCharacter();
		if (c == 'q') return 0;
		std::cout << "Input character: " << c << std::endl;
	}
}
