#include "vertexObject.hpp"
#include <OpenGL/glu.h>
#include <iostream>
/*
VBO *VBO::createVBO(std::vector<float> values) {
  return VBO::createVBO(values, 1);
}
VBO *VBO::createVBO(std::vector<float> values, int size) {
  VBO *output = new VBO();
  output->bind();
  output->type = GL_FLOAT;

  glBufferData(GL_ARRAY_BUFFER, values.size() * sizeof(float), values.data(), GL_STATIC_DRAW);
  std::cout << "Buffer data: " << gluErrorString(glGetError()) << std::endl;

  output->vertexSize = size;
  return output;
}

VBO *VBO::createVBO(std::vector<int> values) {
  return VBO::createVBO(values, 1);
}
VBO *VBO::createVBO(std::vector<int> values, int size) {
  VBO *output = new VBO();
  output->bind();
  output->type = GL_INT;
  glBufferData(GL_ARRAY_BUFFER, values.size() * sizeof(int), values.data(), GL_STATIC_DRAW);
  std::cout << "Buffer data: " << gluErrorString(glGetError()) << std::endl;
  output->vertexSize = size;
  return output;
}

VBO::VBO() {
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &this->id);
  std::cout << "Buffer Allocated: " << gluErrorString(glGetError()) << std::endl;

  this->bind();
}

VAO::VAO(std::vector<VBO *> values) {
  glGenVertexArrays(1, &this->id);

  this->count = values.size();
  this->bind();

  int index = 0;

  for (VBO *vbo : values) {
    vbo->bind();
    glVertexAttribPointer(index, vbo->vertexSize, vbo->type, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(index);
    index++;
  }
}
*/

GLuint Quad::elementArrayBuffer;
bool elementArrayGenerated = false;

Quad::Quad(float x, float y, float w, float h) {

	if (!elementArrayGenerated) {
		elementArrayGenerated = true;
		glGenBuffers(1, &Quad::elementArrayBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Quad::elementArrayBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(int), (int[]){0, 1, 2, 3}, GL_STATIC_READ);
	}

	glGenBuffers(1, &this->id);
	std::cout << "Generated buffer: " << gluErrorString(glGetError()) << std::endl;

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	this->updateData();
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	std::cout << "Bound buffer: " << gluErrorString(glGetError()) << std::endl;
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), this->data, GL_STATIC_DRAW);
	std::cout << "Set buffer data: " << gluErrorString(glGetError()) << std::endl;

	std::cout << "Data: ";
	for (int i = 0; i < 8; i++) {
		std::cout << this->data[i] << ", ";
	}
	std::cout << std::endl;
}

void Quad::updateData() {
	this->data[0] = this->x;
	this->data[1] = this->y;

	this->data[2] = this->x;
	this->data[3] = this->y + this->h;

	this->data[4] = this->x + this->w;
	this->data[5] = this->y;

	this->data[6] = this->x + this->w;
	this->data[7] = this->y + this->h;
	std::cout << this->data[7] << std::endl;
}

void Quad::updateVBO() {
	this->updateData();
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), this->data);
}

void Quad::setPosition(float x, float y) {
	this->x = x;
	this->y = y;
	this->updateVBO();
}

void Quad::setSize(float w, float h) {
	this->w = w;
	this->h = h;
	this->updateVBO();
}
void Quad::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
}