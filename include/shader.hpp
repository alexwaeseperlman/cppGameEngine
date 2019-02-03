#pragma once

#include "util.hpp"
#include <glad/glad.h>
#include <string>
class Shader {
public:
	static Shader *loadShader(std::string filename);
	static void compile(GLuint shader, std::string source);
	Shader();

	std::string getVertexSource() { return this->vertexSource; }
	std::string getFragSource() { return this->fragSource; }

	void setVertexSource(std::string source);
	void setFragSource(std::string source);

	GLuint getVert() { return this->vertex; }
	GLuint getFrag() { return this->frag; }
	GLuint getProgram() { return this->program; }

	GLuint getAttribLocation(std::string);
	void setAttribLocation(std::string, GLuint);

	void bind() { glUseProgram(this->program); }
	GLuint glGetUniformLocation(std::string variableName);

	void printLog();
	void printCompileLog();

private:
	std::string vertexSource;
	std::string fragSource;
	GLuint vertex;
	GLuint frag;
	GLuint program;
};