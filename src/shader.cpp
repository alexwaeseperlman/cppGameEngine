#include "shader.hpp"
#include "OpenGL/glu.h"
#include <iostream>

Shader::Shader() {
	this->vertex = glCreateShader(GL_VERTEX_SHADER);
	this->frag = glCreateShader(GL_FRAGMENT_SHADER);
	this->program = glCreateProgram();
	std::cout << "Create program: " << gluErrorString(glGetError()) << std::endl;

	glAttachShader(this->program, this->vertex);
	glAttachShader(this->program, this->frag);
	glLinkProgram(this->program);
	std::cout << "Link program: " << gluErrorString(glGetError()) << std::endl;
}

void Shader::setVertexSource(std::string source) {
	this->vertexSource = source;
	Shader::compile(this->vertex, this->vertexSource);
	std::cout << "Compile vertex: " << gluErrorString(glGetError()) << std::endl;
}
void Shader::setFragSource(std::string source) {
	this->fragSource = source;
	Shader::compile(this->frag, this->fragSource);
	std::cout << "Compile frag: " << gluErrorString(glGetError()) << std::endl;
}

void Shader::compile(GLuint shader, std::string source) {
	const char *src = source.c_str();

	glShaderSource(shader, 1, &src, NULL);
	std::cout << "Shader source: " << gluErrorString(glGetError()) << std::endl;

	glCompileShader(shader);
	std::cout << "Compile: " << gluErrorString(glGetError()) << std::endl;

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		std::cout << "Shader compile failed: " << buffer << std::endl;
	}
}
Shader *Shader::loadShader(std::string filename) {
	Shader *output = new Shader();
	output->setVertexSource(loadFile(filename + ".vert"));
	output->setFragSource(loadFile(filename + ".frag"));
	output->printCompileLog();
	glLinkProgram(output->program);
	return output;
}

void Shader::printLog() {
	char programInfo[500];
	glGetProgramInfoLog(this->getProgram(), 500, NULL, programInfo);
	std::cout << programInfo << std::endl;
}

void Shader::printCompileLog() {
	char buffer[512];
	glGetShaderInfoLog(this->vertex, 512, NULL, buffer);
	std::cout << "Vertex: " << buffer << std::endl;

	glGetShaderInfoLog(this->frag, 512, NULL, buffer);
	std::cout << "Frag: " << buffer << std::endl;
}