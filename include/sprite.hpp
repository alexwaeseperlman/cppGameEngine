#pragma once
#include "shader.hpp"
#include "vertexObject.hpp"

#include <string>
#include <vector>

class SpriteSheet {
public:
	SpriteSheet(std::string filename);

	void createGrid(float gridWidth, float gridHeight, float imgWidth, float imgHeight);

	// Returns an array of 4 floats 0=x,1=y,2=x2,3=y2
	std::vector<float *> texCoords;

	GLuint getTexture() { return texture; }

private:
	GLuint texture;
};

class Sprite;

class SpriteRenderer {
public:
	SpriteSheet *spriteSheet;
	SpriteRenderer(SpriteSheet *spriteSheet, int maxSprites = 10000);

	Sprite *addSprite(float x, float y, int textureID);
	void removeSprite(Sprite *);

private:
	int maxSprites = 10000;
	int spriteCount = 0;
	std::vector<Sprite *> emptySprites = {};

	GLuint vao;

	GLuint spriteCoordVBO;
	GLuint spriteTextureCoordVBO;
	GLuint elementVBO;
};

class Sprite {
public:
	static const int spriteVertices = 4;
	static const int spriteElements = 6;
	SpriteRenderer *renderer;
	Sprite(SpriteRenderer *renderer, int rendererIndex);
	void setTextureID(int textureID);
	int getTextureID() { return textureID; };

	void setPosition(float x, float y);
	void setSize(float w, float h);
	float getX() { return coords[0]; }
	float getY() { return coords[1]; }
	float getW() { return coords[2]; }
	float getH() { return coords[3]; }

private:
	int textureID;

	float coords[4] = {0, 0, 1, 1};
	float texCoords[4] = {0, 0, 4, 4};

	int rendererIndex;
};
