#pragma once
#include <glad/glad.h>

#include <OpenGL/glu.h>

#include <iostream>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window {
public:
	static void onError(int error, const char *description);

	Window(float width, float height, std::string title);
	~Window();

	static void init();

	float aspectRatio();
	glm::vec2 scale();

	void update();

	void setTitle(std::string title);

	void setSize(int w, int h);
	void setPosition(int x, int y);
	GLFWwindow *handle() { return this->windowptr; };

	float getX() { return x; }
	float getY() { return y; }

	int getWidth() { return w; }
	int getHeight() { return h; }

	int getFBWidth() { return fbWidth; }
	int getFBHeight() { return fbHeight; }

	bool shouldClose();

private:
	int w;
	int h;
	int x;
	int y;

	int fbWidth;
	int fbHeight;

	std::string title;
	GLFWwindow *windowptr;
};