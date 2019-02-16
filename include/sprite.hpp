#pragma once
#include "shader.hpp"
#include "vertexObject.hpp"

#include <array>
#include <json/json.h>
#include <string>
#include <vector>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

class SpriteSheet {
public:
	SpriteSheet(std::string filename);

	void createGrid(float gridWidth, float gridHeight, float imgWidth, float imgHeight);

	// Returns an array of 4 floats 0=x1,1=y1,2=x2,3=y2
	std::vector<float *> texCoords;

	void loadFromJSON(std::string filename);

	GLuint getTexture() { return texture; }

	Json::Value sprites;

	int getWidth() { return width; }
	int getHeight() { return height; }

private:
	int width;
	int height;
	GLuint texture;
};

class Sprite;

class SpriteRenderer {
public:
	static Shader *spriteShader;
	static GLint spriteSheetUniform;
	static GLint viewProjectionUniform;
	SpriteSheet *spriteSheet;
	SpriteRenderer(SpriteSheet *spriteSheet, int maxSprites = 10000);

	Sprite *addSprite(float x, float y, float z, int textureID);
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
	static const int spriteVertexSize = 3;
	static const int spriteElements = 6;
	static const int spriteElementArray[6];

	/**
	 * NESWi -> An integer representing North, East, South, West from 0-3
	 *
	 * ZRotf -> A float representing the degrees of rotation around the Z axis.
	 *
	 * Quatf -> Euler angles (uses a quaternion internally). Coordinates
	 *
	 * None -> The sprite is displayed from the bottom left coordinate without any rotation.
	 **/
	enum rotationMode { NESWi, ZRotf, Quatf, None };
	enum drawMode { CENTER, CORNER };

	SpriteRenderer *renderer;
	Sprite(SpriteRenderer *renderer);
	Sprite(SpriteRenderer *renderer, int rendererIndex);

	void setTexturePos(glm::vec2 pos1, glm::vec2 pos2);
	void setTextureID(int textureID);
	void updateTextureBuffer();
	int getTextureID() { return textureID; };

	void setPosition(float x, float y, float z);
	void setPosition(float x, float y);
	void setSize(float w, float h);
	glm::vec3 *getCoordinates() { return absolutePos; }
	float *getTextureCoordinates() { return texCoords; }

	int getNESW() { return this->nesw; }
	float getZRot() { return this->zrot; }
	glm::vec3 getEulerAngles() { return glm::eulerAngles(this->rotation); }
	glm::quat getRotation() { return this->rotation; }

	void setRotation(int nesw);
	void setRotation(float degrees);
	void setRotation(glm::vec3 degrees);
	void setRotation(float x, float y, float z);
	void setRotation(glm::quat degrees);

	void updateCoords();
	void updateBuffers();

	int getRendererIndex() { return rendererIndex; }

	void setRotationMode(rotationMode mode);

	void mirror();
	void setMirror(bool mirrored);
	bool isMirrored();

	drawMode getDrawMode() { return this->drawCoordinateMode; }
	void setDrawMode(drawMode mode);

protected:
	int textureID;

	bool mirrored = false;

	rotationMode rotMode = NESWi;
	drawMode drawCoordinateMode = CORNER;

	// This is only used for rotation mode neswi
	int nesw = 0;
	// This is only used for ZRotfCenter
	float zrot = 0;
	// This is only used for rotation mode Quatf
	glm::quat rotation;

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec2 size = glm::vec2(0.0f, 0.0f);
	glm::vec3 absolutePos[4] = {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	                            glm::vec3(0.0f, 0.0f, 0.0f)};

	float coords[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	float texCoords[8] = {0, 0, 0, 1, 1, 0, 1, 1};

	int rendererIndex;
};