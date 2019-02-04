#pragma once
#include "shader.hpp"
#include "vertexObject.hpp"

#include <array>
#include <string>
#include <vector>

class SpriteSheet {
public:
	SpriteSheet(std::string filename);

	void createGrid(float gridWidth, float gridHeight, float imgWidth, float imgHeight);

	// Returns an array of 4 floats 0=x1,1=y1,2=x2,3=y2
	std::vector<float *> texCoords;

	GLuint getTexture() { return texture; }

private:
	GLuint texture;
};

class Sprite;

class SpriteRenderer {
public:
	static Shader *spriteShader;
	static GLint spriteSheetUniform;
	SpriteSheet *spriteSheet;
	SpriteRenderer(SpriteSheet *spriteSheet, int maxSprites = 10000);

	Sprite *addSprite(float x, float y, int textureID);
	void removeSprite(Sprite *);

	void display();

	GLuint spriteCoordVBO;
	GLuint spriteTextureCoordVBO;
	GLuint elementVBO;

private:
	int maxSprites = 10000;
	int spriteCount = 0;
	std::vector<Sprite *> emptySprites = {};

	GLuint vao;
};

class Sprite {
public:
	static const int spriteVertices = 4;
	static const int spriteVertexSize = 2;
	static const int spriteElements = 6;
	static const int spriteElementArray[6];
	SpriteRenderer *renderer;
	Sprite(SpriteRenderer *renderer, int rendererIndex);
	void setTextureID(int textureID);
	int getTextureID() { return textureID; };

	void setPosition(float x, float y);
	void setSize(float w, float h);
	float *getCoordinates() { return coords; }
	float *getTextureCoordinates() { return texCoords; }
	float getX1() { return coords[0]; }
	float getY1() { return coords[1]; }
	float getX2() { return coords[6]; }
	float getY2() { return coords[7]; }

	void updateCoords();

	int getRendererIndex() { return rendererIndex; }

private:
	int textureID;

	float x;
	float y;
	float w;
	float h;

	float coords[8] = {0, 0, 0, 1, 1, 0, 1, 1};
	float texCoords[8] = {0, 0, 0, 1, 1, 0, 1, 1};

	int rendererIndex;
};
