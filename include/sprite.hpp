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
	float gridWidth;
	float gridHeight;
	GLuint texture;
};

class SpriteRenderer {
public:
	static Shader *spriteShader;
	Shader *renderShader;
	SpriteSheet *spriteSheet;

	SpriteRenderer(SpriteSheet *sheet, Shader *shader);
	SpriteRenderer(SpriteSheet *sheet);
	SpriteRenderer(Shader *shader);

	Quad *addSprite(float x, float y, int spriteID);
	Quad *addSprite(Quad *sprite, int spriteID);
	Quad *addSprite(float x, float y, float w, float h, int spriteID);
	// TODO: Add remove sprite

	void display();

private:
	int numberOfSprites = 0;
	GLuint vao;
};