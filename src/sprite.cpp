#include "sprite.hpp"
#include "shader.hpp"

#include <IL/il.h>
#include <IL/ilu.h>
#include <OpenGL/glu.h>
#include <iostream>

SpriteSheet::SpriteSheet(std::string filename) {
	this->texCoords = std::vector<float *>();
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
		} else {
			std::cerr << "Unable to load image: " << iluErrorString(ilGetError()) << std::endl;
		}
		// Delete file from memory
		ilDeleteImages(1, &imgID);
	} else {
		std::cerr << "Unable to load image: " << iluErrorString(ilGetError()) << std::endl;
	}

	// Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("Error loading texture from %p pixels! %s\n", ilGetData(), gluErrorString(error));
	}
}

SpriteRenderer::SpriteRenderer(SpriteSheet *spriteSheet, int maxSprites) {
	glGenBuffers(1, &this->spriteCoordVBO);
	glGenBuffers(1, &this->spriteTextureCoordVBO);

	glGenVertexArrays(1, &this->vao);

	// Initialize both buffers to have maxSprites * spriteVertices to store enough information for every sprite
	glBindBuffer(GL_ARRAY_BUFFER, this->spriteCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, this->maxSprites * Sprite::spriteVertices * sizeof(GLfloat),
	             NULL /* This might cause some problems */, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->spriteTextureCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, this->maxSprites * Sprite::spriteVertices * sizeof(GLfloat),
	             NULL /* This might cause some problems */, GL_STATIC_DRAW);
}

Sprite *SpriteRenderer::addSprite(float x, float y, int textureID) {
}

void SpriteRenderer::removeSprite(Sprite *) {
}

Sprite::Sprite(SpriteRenderer *renderer, int rendererIndex) {
	this->renderer = renderer;
	this->rendererIndex = rendererIndex;
}

void Sprite::setTextureID(int textureID) {
}

void Sprite::setPosition(float x, float y) {
}

void Sprite::setSize(float w, float h) {
}