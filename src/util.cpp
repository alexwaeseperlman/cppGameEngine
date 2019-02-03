#include "util.hpp"
#include <IL/il.h>
#include <IL/ilu.h>
#include <fstream>
#include <sstream>
#include <string>
std::string loadFile(std::string filename) {
	// Open the specified file
	std::ifstream in(filename);
	// Load the file into the string stream
	std::stringstream sstr;
	sstr << in.rdbuf();
	return sstr.str();
}

bool initLibraries() {
	// Initialize DevIL
	ilInit();
	ilClearColour(255, 255, 255, 000);

	// Check for error
	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR) {
		printf("Error initializing DevIL! %s\n", iluErrorString(ilError));
		return false;
	}
}