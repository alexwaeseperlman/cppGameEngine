#pragma once
#include "shader.hpp"
#include "vertexObject.hpp"

#include <string>
#include <vector>
class SpriteSheet {
public:
	SpriteSheet(std::string filename, float gridWidth, float gridHeight);

	float getGridWidth() { return gridWidth; }
	float getGridHeight() { return gridHeight; }

	GLuint getTexture() { return texture; }

private:
	float gridWidth = 1;
	float gridHeight = 1;
	GLuint texture;
};

class Sprite : public Quad {
public:
	Sprite(Quad *quad, int spriteSheetID);
	Sprite(float x, float y, float w, float h, int spriteSheetID);

	int spriteSheetIndex;
};

class SpriteRenderer {
public:
	static Shader *spriteShader;
	SpriteSheet *spriteSheet;

	static GLint spriteNumberUniform;
	static GLint gridSizeUniform;
	static GLint spriteSheetUniform;

	SpriteRenderer(SpriteSheet *sheet, Shader *shader);
	SpriteRenderer(SpriteSheet *sheet);
	SpriteRenderer(Shader *shader);

	Sprite *addSprite(Sprite *sprite);
	Sprite *addSprite(float x, float y, int spriteID);
	Sprite *addSprite(Quad *sprite, int spriteID);
	Sprite *addSprite(float x, float y, float w, float h, int spriteID);
	// TODO: Add remove sprite

	void display();

private:
	std::vector<Sprite *> sprites = {};
	/*
	uniform int spriteNumber;
	uniform vec2 gridSize;
	uniform sampler2D spriteSheet;
	*/
};