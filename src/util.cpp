#include "util.hpp"
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