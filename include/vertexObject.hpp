#pragma once
#include <glad/glad.h>
#include <vector>
/*
class VBO {
public:
    static VBO *createVBO(std::vector<float>);
    static VBO *createVBO(std::vector<float>, int size);
    /*static VBO *createVBO(std::vector<Eigen::Vector2f>);
    static VBO *createVBO(std::vector<Eigen::Vector3f>);
    static VBO *createVBO(std::vector<Eigen::Vector4f>);*//*

	static VBO *createVBO(std::vector<int>);
	static VBO *createVBO(std::vector<int>, int size);
	/*static VBO *createVBO(std::vector<Eigen::Vector2i>);
	static VBO *createVBO(std::vector<Eigen::Vector3i>);
	static VBO *createVBO(std::vector<Eigen::Vector4i>);*//*

	VBO();
	GLuint type;

	GLuint getID() { return id; }

	void dispose() { glDeleteBuffers(1, &id); }
	void bind() { glBindBuffer(GL_ARRAY_BUFFER, id); }

	int vertexSize;

private:
	GLuint id;
};

class VAO {
public:
	VAO(std::vector<VBO *>);

	int count;

	GLuint getID() { return id; }

	void dispose() { glDeleteVertexArrays(1, &id); }
	void bind() { glBindVertexArray(id); }

private:
	GLuint id;
};*/

class Quad {
public:
	Quad(float x, float y, float width, float height);
	void setPosition(float x, float y);
	void setSize(float w, float h);
	float data[8];
	float x, y, w, h;
	void updateVBO();
	void dispose() { glDeleteBuffers(1, &id); }
	GLuint getId() { return id; }
	static GLuint elementArrayBuffer;
	void bind();
	void updateData();

private:
	GLuint id;
};