#include "sprite.hpp"
#include "shader.hpp"

#include <IL/il.h>
#include <IL/ilu.h>

#ifdef __linux__
#include <GL/glu.h>
#else
#include <OpenGL/glu.h>
#endif

#include <iostream>

#include <fstream>

SpriteSheet::SpriteSheet(std::string filename) {

	// This was here to debug a segfault but it looks badass so I'm keeping it
	printf("Sprite Sheet: %p\n\tTex Coords: %p\n", (void *) this, (void *) (&this->texCoords));

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

			this->width = ilGetInteger(IL_IMAGE_WIDTH);
			this->height = ilGetInteger(IL_IMAGE_HEIGHT);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLuint) ilGetInteger(IL_IMAGE_WIDTH),
			             (GLuint) ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLuint *) ilGetData());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		} else {
			std::cerr << "Unable to convert image to pixels: " << iluErrorString(ilGetError()) << std::endl;
		}
		// Delete file from memory
		ilDeleteImages(1, &imgID);
	} else {
		std::cerr << "Unable to load image: " << iluErrorString(ilGetError()) << std::endl;
	}

	// Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Error loading texture from %p pixels! " << ilGetData() << gluErrorString(error);
	}
}

void SpriteSheet::loadFromJSON(std::string filename) {
	Json::Value root;

	std::string file = loadFile(filename);
	std::cout << file << std::endl;

	Json::Reader reader;
	bool parsingSuccessful = reader.parse(file.c_str(), root);

	if (!parsingSuccessful) {
		std::cout << "Failed to parse" << reader.getFormattedErrorMessages();
		return;
	}

	int index = 0;

	for (Json::ValueIterator itr = root["frames"].begin(); itr != root["frames"].end(); itr++) {
		float *coords = new float[4];
		coords[0] = (*itr)["frame"]["x"].asFloat() / this->width;
		coords[1] = (*itr)["frame"]["y"].asFloat() / this->height;
		coords[2] = (*itr)["frame"]["w"].asFloat() / this->width;
		coords[2] += coords[0];
		coords[3] = (*itr)["frame"]["h"].asFloat() / this->height;
		coords[3] += coords[1];
		std::cout << coords[0] << ", " << coords[1] << std::endl;
		this->texCoords.push_back(coords);
	}
}

Shader *SpriteRenderer::spriteShader = NULL;
GLint SpriteRenderer::spriteSheetUniform = NULL;

SpriteRenderer::SpriteRenderer(SpriteSheet *spriteSheet, int maxSprites) {
	this->spriteSheet = spriteSheet;
	this->maxSprites = maxSprites;

	// Generate all of the buffers for the sprite renderer
	glGenBuffers(1, &this->spriteCoordVBO);
	glGenBuffers(1, &this->spriteTextureCoordVBO);
	glGenBuffers(1, &this->elementVBO);

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	// TODO: Sprites should be able to be rotated at 90 degree angles

	// Initialize both buffers to have maxSprites * spriteVertices to store enough information for every sprite
	glBindBuffer(GL_ARRAY_BUFFER, this->spriteCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, this->maxSprites * Sprite::spriteVertices * sizeof(GLfloat) * Sprite::spriteVertexSize,
	             NULL /* This might cause some problems */, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->spriteTextureCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, this->maxSprites * Sprite::spriteVertices * sizeof(GLfloat) * Sprite::spriteVertexSize,
	             NULL /* This might cause some problems */, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->maxSprites * Sprite::spriteElements * sizeof(GLuint),
	             NULL /* This might cause some problems */, GL_STATIC_DRAW);
}

Sprite *SpriteRenderer::addSprite(float x, float y, int textureID) {
	printf("Renderer: %p\n\tSprite Sheet: %p\n\t\tTex Coords: %p\n", (void *) this, (void *) this->spriteSheet,
	       (void *) &this->spriteSheet->texCoords);
	Sprite *sprite = new Sprite(this, this->spriteCount++);

	sprite->setPosition(x, y);

	return sprite;
}

void SpriteRenderer::display() {
	glBindTexture(GL_TEXTURE_2D, this->spriteSheet->getTexture());
	SpriteRenderer::spriteShader->bind();
	glBindVertexArray(this->vao);
	std::cout << "Bind vao: " << gluErrorString(glGetError()) << std::endl;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementVBO);
	std::cout << "Bind buffer: " << gluErrorString(glGetError()) << std::endl;
	glDrawElements(GL_TRIANGLES, this->spriteCount * Sprite::spriteElements, GL_UNSIGNED_INT, 0);
}

void SpriteRenderer::removeSprite(Sprite *) {
	// TODO:
}

const int Sprite::spriteElementArray[6] = {0, 1, 2, 2, 1, 3};

Sprite::Sprite(SpriteRenderer *renderer, int rendererIndex) {
	std::cout << "Creating sprite" << std::endl;

	this->renderer = renderer;
	this->rendererIndex = rendererIndex;
	this->setTextureID(rendererIndex);
	std::cout << "Set texture" << std::endl;

	// Set the indexes in the default element buffer to point to this sprites indexes
	GLint *elements = new GLint[Sprite::spriteElements];
	for (int i = 0; i < Sprite::spriteElements; i++) {
		elements[i] = Sprite::spriteElementArray[i] + this->getRendererIndex() * Sprite::spriteVertices;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer->elementVBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, this->getRendererIndex() * Sprite::spriteElements * sizeof(GLuint),
	                Sprite::spriteElements * sizeof(GLuint), elements);
	delete[] elements;

	this->setPosition(0, 0);
	this->setSize(1, 1);
}

void Sprite::setTextureID(int textureID) {
	this->textureID = textureID;
	std::cout << "Set tex ID to " << textureID << std::endl;

	float *texCoords = (this->renderer->spriteSheet->texCoords[textureID]);

	// Update tex coords to use the right format
	this->texCoords[0] = texCoords[0];
	this->texCoords[1] = texCoords[1];
	//
	this->texCoords[2] = texCoords[0];
	this->texCoords[3] = texCoords[3];
	//
	this->texCoords[4] = texCoords[2];
	this->texCoords[5] = texCoords[1];
	//
	this->texCoords[6] = texCoords[2];
	this->texCoords[7] = texCoords[3];

	std::cout << "Set tex coords" << std::endl;

	// Texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, this->renderer->spriteTextureCoordVBO);
	glBufferSubData(GL_ARRAY_BUFFER,
	                this->rendererIndex * Sprite::spriteVertexSize * Sprite::spriteVertices * sizeof(GLfloat),
	                Sprite::spriteVertices * Sprite::spriteVertexSize * sizeof(GLfloat), this->texCoords);
}

void Sprite::setPosition(float x, float y) {
	this->x = x;
	this->y = y;

	this->updateCoords();
}

void Sprite::setSize(float w, float h) {
	this->w = w;
	this->h = h;

	this->updateCoords();
}

void Sprite::updateCoords() {

	this->coords[0] = this->x;
	this->coords[1] = this->y;
	//
	this->coords[2] = this->x;
	this->coords[3] = this->y + this->h;
	//
	this->coords[4] = this->x + this->w;
	this->coords[5] = this->y;
	//
	this->coords[6] = this->x + this->w;
	this->coords[7] = this->y + this->h;

	// Coordinates
	glBindBuffer(GL_ARRAY_BUFFER, this->renderer->spriteCoordVBO);
	glBufferSubData(GL_ARRAY_BUFFER,
	                this->rendererIndex * Sprite::spriteVertices * Sprite::spriteVertexSize * sizeof(GLfloat),
	                Sprite::spriteVertices * Sprite::spriteVertexSize * sizeof(GLfloat), this->coords);
}