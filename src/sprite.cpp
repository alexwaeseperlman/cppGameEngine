#include "sprite.hpp"
#include "shader.hpp"

#include <OpenGL/glu.h>
#include <iostream>

SpriteSheet::SpriteSheet(std::string filename, float gridWidth, float gridHeight) {
	/* TODO: Add error checking
	SDL_Surface *Surface = IMG_Load(filename.c_str());

	//glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	int mode = GL_RGB;

	if (Surface->format->BytesPerPixel == 4) {
	  mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, mode, Surface->w, Surface->h, 0, mode, GL_UNSIGNED_BYTE, Surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
	this->gridWidth = gridWidth;
	this->gridHeight = gridHeight;
	std::cout << this->gridWidth << std::endl;
}

SpriteRenderer::SpriteRenderer(SpriteSheet *sheet, Shader *shader) {
	this->spriteSheet = sheet;
	this->renderShader = shader;
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
}

Quad *SpriteRenderer::addSprite(Quad *sprite, int spriteID) {
	sprite->bind();
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	this->numberOfSprites++;

	// TODO: Implement sprite numbers in another vertex object

	return sprite;
}

Quad *SpriteRenderer::addSprite(float x, float y, int spriteID) {
	Quad *quad = new Quad(x, y, this->spriteSheet->getGridWidth(), this->spriteSheet->getGridHeight());
	std::cout << "Created Quad for sprite: " << gluErrorString(glGetError()) << std::endl;

	return this->addSprite(quad, spriteID);
}

Quad *SpriteRenderer::addSprite(float x, float y, float w, float h, int spriteID) {
	Quad *quad = new Quad(x, y, w, h);
	return this->addSprite(quad, spriteID);
}

void SpriteRenderer::display() {

	this->renderShader->bind();
	std::cout << "Bind shader: " << gluErrorString(glGetError()) << std::endl;

	glBindVertexArray(this->vao);
	std::cout << "Bind vertex array: " << gluErrorString(glGetError()) << std::endl;
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, this->numberOfSprites * 4);
	std::cout << "Bind draw arrays: " << gluErrorString(glGetError()) << std::endl;
}