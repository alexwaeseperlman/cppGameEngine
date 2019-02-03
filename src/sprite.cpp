#include "sprite.hpp"
#include "shader.hpp"

#include <IL/il.h>
#include <OpenGL/glu.h>
#include <iostream>

SpriteSheet::SpriteSheet(std::string filename, float gridWidth, float gridHeight) {
	ILuint imgID = 0;
	ilGenImages(1, &imgID);
	ILboolean success = ilLoadImage(filename.c_str());

	// Image loaded successfully
	if (success == IL_TRUE) {
		// Convert image to RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (success == IL_TRUE) {
			// Create texture from file pixels
			glGenTextures(1, &this->texture);
			glBindTexture(GL_TEXTURE_2D, this->texture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLuint) ilGetInteger(IL_IMAGE_WIDTH),
			             (GLuint) ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLuint *) ilGetData());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		// Delete file from memory
		ilDeleteImages(1, &imgID);
	}

	// Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("Error loading texture from %p pixels! %s\n", ilGetData(), gluErrorString(error));
	}

	this->gridWidth = gridWidth;
	this->gridHeight = gridHeight;
	std::cout << this->gridWidth << std::endl;
}

Sprite::Sprite(Quad *quad, int spriteSheetID) : Quad::Quad(quad->x, quad->y, quad->w, quad->h) {
	this->spriteSheetIndex = spriteSheetID;
}

Sprite::Sprite(float x, float y, float w, float h, int spriteSheetID) : Quad::Quad(x, y, w, h) {
	this->spriteSheetIndex = spriteSheetID;
}

Shader *SpriteRenderer::spriteShader = Shader::loadShader("assets/SpriteSheet");

GLint SpriteRenderer::spriteNumberUniform = 0;
GLint SpriteRenderer::gridSizeUniform = 0;
GLint SpriteRenderer::spriteSheetUniform = 0;

SpriteRenderer::SpriteRenderer(SpriteSheet *sheet, Shader *shader) {

	SpriteRenderer::spriteNumberUniform = glGetUniformLocation(spriteShader->getProgram(), "spriteNumber");
	SpriteRenderer::gridSizeUniform = glGetUniformLocation(spriteShader->getProgram(), "gridSize");
	SpriteRenderer::spriteSheetUniform = glGetUniformLocation(spriteShader->getProgram(), "spriteSheet");
	this->spriteSheet = sheet;
	this->sprites;
}

Sprite *SpriteRenderer::addSprite(Sprite *sprite) {
	this->sprites.insert(this->sprites.end(), sprite);
	return sprite;
}

Sprite *SpriteRenderer::addSprite(Quad *quad, int spriteID) {
	Sprite *sprite = new Sprite(quad, spriteID);
	return this->addSprite(sprite);
}

Sprite *SpriteRenderer::addSprite(float x, float y, int spriteID) {
	Sprite *sprite = new Sprite(x, y, this->spriteSheet->getGridWidth(), this->spriteSheet->getGridHeight(), spriteID);
	return this->addSprite(sprite);
}

Sprite *SpriteRenderer::addSprite(float x, float y, float w, float h, int spriteID) {
	Sprite *sprite = new Sprite(x, y, w, h, spriteID);
	return this->addSprite(sprite);
}

void SpriteRenderer::display() {
	SpriteRenderer::spriteShader->bind();
	glUniform2f(gridSizeUniform, this->spriteSheet->getGridWidth(), this->spriteSheet->getGridHeight());
	glUniform1i(spriteSheetUniform, this->spriteSheet->getTexture());
	// Iterate sprites and draw them all
	for (Sprite *sprite : this->sprites) {
		glUniform1i(spriteNumberUniform, sprite->spriteSheetIndex);
	}
}