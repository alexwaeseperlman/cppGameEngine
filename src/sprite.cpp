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

static float *zeroCoords = new float[4]();
void SpriteSheet::loadFromJSON(std::string filename) {
	this->texCoords.push_back(zeroCoords);

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

		// Coords need to be upside down because of opengl image coordinate space
		coords[2] = (*itr)["frame"]["x"].asFloat() / this->width;
		coords[3] = (*itr)["frame"]["y"].asFloat() / this->height;
		coords[0] = (*itr)["frame"]["w"].asFloat() / this->width + coords[2];
		coords[1] = (*itr)["frame"]["h"].asFloat() / this->height + coords[3];

		std::cout << coords[0] << ", " << coords[1] << std::endl;
		this->texCoords.push_back(coords);
	}
}

Shader *SpriteRenderer::spriteShader = NULL;
GLint SpriteRenderer::spriteSheetUniform = NULL;
GLint SpriteRenderer::viewProjectionUniform = NULL;

SpriteRenderer::SpriteRenderer(SpriteSheet *spriteSheet, int maxSprites) {
	this->spriteSheet = spriteSheet;
	this->maxSprites = maxSprites;

	// Generate all of the buffers for the sprite renderer
	glGenBuffers(1, &this->spriteCoordVBO);
	glGenBuffers(1, &this->spriteTextureCoordVBO);
	glGenBuffers(1, &this->elementVBO);

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	// Initialize both buffers to have maxSprites * spriteVertices to store enough information for every sprite
	glBindBuffer(GL_ARRAY_BUFFER, this->spriteCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, this->maxSprites * Sprite::spriteVertices * sizeof(GLfloat) * Sprite::spriteVertexSize,
	             NULL, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, Sprite::spriteVertexSize, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->spriteTextureCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, this->maxSprites * Sprite::spriteVertices * sizeof(GLfloat) * 2, NULL, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->maxSprites * Sprite::spriteElements * sizeof(GLuint), NULL,
	             GL_STATIC_DRAW);
}

Sprite *SpriteRenderer::addSprite(float x, float y, float z, int textureID) {

	printf("Renderer: %p\n\tSprite Sheet: %p\n\t\tTex Coords: %p\n", (void *) this, (void *) this->spriteSheet,
	       (void *) &this->spriteSheet->texCoords);
	Sprite *sprite;
	if (this->emptySprites.size() > 0) {
		sprite = this->emptySprites[0];
		this->emptySprites.pop_back();
	} else {
		sprite = new Sprite(this, this->spriteCount++);
	}

	sprite->setPosition(x, y, z);
	sprite->setTextureID(textureID);

	return sprite;
}

Sprite *SpriteRenderer::addSprite(float x, float y, int textureID) {

	printf("Renderer: %p\n\tSprite Sheet: %p\n\t\tTex Coords: %p\n", (void *) this, (void *) this->spriteSheet,
	       (void *) &this->spriteSheet->texCoords);
	Sprite *sprite;
	if (this->emptySprites.size() > 0) {
		sprite = this->emptySprites[0];
		this->emptySprites.pop_back();
	} else {
		sprite = new Sprite(this, this->spriteCount++);
	}

	sprite->setPosition(x, y, 0);
	sprite->setTextureID(textureID);

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

void SpriteRenderer::removeSprite(Sprite *sprite) {
	// Set the positions of each vertex to zero
	sprite->setSize(0, 0);
	sprite->setPosition(0, 0, 0);

	this->emptySprites.push_back(sprite);
}

const int Sprite::spriteElementArray[6] = {1, 0, 2, 3, 2, 0};

Sprite::Sprite(SpriteRenderer *renderer, int rendererIndex) {
	std::cout << "Creating sprite" << std::endl;

	this->renderer = renderer;
	this->rendererIndex = rendererIndex;
	this->setTextureID(rendererIndex);
	std::cout << "Set texture" << std::endl;

	// Set the indexes in the default element buffer to point to this sprites indexes
	GLint *elements = new GLint[Sprite::spriteElements];
	for (int i = 0; i < Sprite::spriteElements; i++) {
		elements[i] = Sprite::spriteElementArray[i] + this->rendererIndex * Sprite::spriteVertices;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer->elementVBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, this->rendererIndex * Sprite::spriteElements * sizeof(GLuint),
	                Sprite::spriteElements * sizeof(GLuint), elements);
	delete[] elements;

	this->setPosition(0, 0, -1);
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
	this->texCoords[2] = texCoords[2];
	this->texCoords[3] = texCoords[1];
	//
	this->texCoords[4] = texCoords[2];
	this->texCoords[5] = texCoords[3];
	//
	this->texCoords[6] = texCoords[0];
	this->texCoords[7] = texCoords[3];

	if (this->mirrored) {
		std::swap(this->texCoords[0], this->texCoords[2]);
		std::swap(this->texCoords[6], this->texCoords[4]);
	}

	std::cout << "Set tex coords" << std::endl;

	// Texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, this->renderer->spriteTextureCoordVBO);
	glBufferSubData(GL_ARRAY_BUFFER, this->rendererIndex * 2 * Sprite::spriteVertices * sizeof(GLfloat),
	                Sprite::spriteVertices * 2 * sizeof(GLfloat), this->texCoords);
}

void Sprite::setPosition(float x, float y, float z) {
	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;

	this->updateCoords();

	this->updateBuffers();
}

void Sprite::setPosition(float x, float y) {
	this->setPosition(x, y, this->pos.z);
}

void Sprite::setSize(float w, float h) {
	this->size.x = w;
	this->size.y = h;

	this->updateCoords();

	this->updateBuffers();
}

void Sprite::setRotation(int nesw) {
	this->nesw = nesw;
	this->rotMode = NESWi;
	this->updateCoords();
	this->updateBuffers();
}

void Sprite::setRotation(float degrees) {
	this->zrot = degrees;
	this->rotMode = ZRotf;
	this->updateCoords();
	this->updateBuffers();
}

void Sprite::setRotation(float x, float y, float z) {
	this->setRotation(glm::quat(glm::vec3(x, y, z)));
}

void Sprite::setRotation(glm::vec3 degrees) {
	this->setRotation(glm::quat(degrees));
}

void Sprite::setRotation(glm::quat degrees) {
	this->rotation = degrees;
	this->rotMode = Quatf;
	this->updateCoords();
	this->updateBuffers();
}

void Sprite::setRotationMode(rotationMode rotMode) {
	this->rotMode = rotMode;
	this->updateCoords();
	this->updateBuffers();
}

void Sprite::mirror() {
	this->setMirror(!this->mirrored);
}

void Sprite::setMirror(bool mirrored) {
	this->mirrored = mirrored;
	this->setTextureID(this->textureID);
}

bool Sprite::isMirrored() {
	return this->mirrored;
}

void Sprite::setDrawMode(drawMode mode) {
	this->drawCoordinateMode = mode;
	this->updateCoords();
	this->updateBuffers();
}

void Sprite::updateCoords() {
	glm::vec3 pos = this->pos;
	if (this->drawCoordinateMode == CENTER) {
		pos.x -= this->size.x / 2;
		pos.y -= this->size.y / 2;
	}

	if (this->rotMode == NESWi) {
		glm::vec2 adjustedSize = this->size;

		// Swap width and height if it's rotated 90 or 270 degrees to prevent stretching
		if (this->nesw % 2 == 1) {
			float x = adjustedSize.x;
			adjustedSize.x = adjustedSize.y;
			adjustedSize.y = x;
		}
		this->absolutePos[this->nesw % 4] = pos;
		this->absolutePos[(this->nesw + 1) % 4] = glm::vec3(pos.x + adjustedSize.x, pos.y, pos.z);
		this->absolutePos[(this->nesw + 2) % 4] = glm::vec3(pos.x + adjustedSize.x, pos.y + adjustedSize.y, pos.z);
		this->absolutePos[(this->nesw + 3) % 4] = glm::vec3(pos.x, pos.y + adjustedSize.y, pos.z);
	}
}

void Sprite::updateBuffers() {
	this->coords[0] = this->absolutePos[0].x;
	this->coords[1] = this->absolutePos[0].y;
	this->coords[2] = this->absolutePos[0].z;
	//
	this->coords[3] = this->absolutePos[1].x;
	this->coords[4] = this->absolutePos[1].y;
	this->coords[5] = this->absolutePos[1].z;
	//
	this->coords[6] = this->absolutePos[2].x;
	this->coords[7] = this->absolutePos[2].y;
	this->coords[8] = this->absolutePos[2].z;
	//
	this->coords[9] = this->absolutePos[3].x;
	this->coords[10] = this->absolutePos[3].y;
	this->coords[11] = this->absolutePos[3].z;

	// Coordinates
	glBindBuffer(GL_ARRAY_BUFFER, this->renderer->spriteCoordVBO);
	glBufferSubData(GL_ARRAY_BUFFER,
	                this->rendererIndex * Sprite::spriteVertices * Sprite::spriteVertexSize * sizeof(GLfloat),
	                Sprite::spriteVertices * Sprite::spriteVertexSize * sizeof(GLfloat), this->coords);
}