#include "util.hpp"
#include <IL/il.h>
#include <IL/ilu.h>
#include <fstream>
#include <sstream>
#include <string>

#include "shader.hpp"
#include "sprite.hpp"

#ifdef __linux__
#include <GL/glu.h>
#else
#include <OpenGL/glu.h>
#endif

#include <iostream>
std::string loadFile(std::string filename) {
	// Open the specified file
	std::ifstream in(filename);
	// Load the file into the string stream
	std::stringstream sstr;
	sstr << in.rdbuf();
	return sstr.str();
}

bool initLibraries() {
	/*SpriteRenderer::spriteShader = Shader::loadShader("assets/SpriteSheet");
	SpriteRenderer::spriteNumberUniform =
	    glGetUniformLocation(SpriteRenderer::spriteShader->getProgram(), "spriteNumber");
	SpriteRenderer::gridSizeUniform = glGetUniformLocation(SpriteRenderer::spriteShader->getProgram(), "gridSize");
	SpriteRenderer::spriteSheetUniform = glGetUniformLocation(SpriteRenderer::spriteShader->getProgram(), "spriteSheet");
	std::cout << "Uniform locations: " << gluErrorString(glGetError()) << std::endl << std::endl;*/

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