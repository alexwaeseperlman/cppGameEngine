#pragma once
#include <glad/glad.h>

#include <OpenGL/glu.h>

#include <iostream>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

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

	/*
	 * All of the event listeners
	 */
	// Keyboard
	static std::vector<GLFWkeyfun> keyListeners;
	static std::vector<GLFWcharfun> charListeners;
	static std::vector<GLFWcharmodsfun> modifierListeners;

	// Mouse
	static std::vector<GLFWcursorposfun> mouseMoveListeners;
	static std::vector<GLFWcursorenterfun> mouseEnterListeners;
	static std::vector<GLFWmousebuttonfun> mouseButtonListeners;
	static std::vector<GLFWscrollfun> scrollListeners;

	// OS
	static std::vector<GLFWdropfun> fileDropListeners;
	static std::vector<GLFWwindowsizefun> windowResizeListeners;
	static std::vector<GLFWframebuffersizefun> framebufferResizeListeners;
	static std::vector<GLFWwindowiconifyfun> minimizeListeners;
	static std::vector<GLFWwindowfocusfun> focusedListeners;
	static std::vector<GLFWwindowposfun> windowMovedListeners;
	static std::vector<GLFWwindowmaximizefun> windowMaximizedListeners;

	// TODO? joystick inputs

protected:
	int w;
	int h;
	int x;
	int y;

	int fbWidth;
	int fbHeight;

	static void keyfun(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void charfun(GLFWwindow *window, unsigned int codepoint);
	static void charmodsfun(GLFWwindow *window, unsigned int codepoint, int mods);

	static void cursorposfun(GLFWwindow *window, double xpos, double ypos);
	static void cursorenterfun(GLFWwindow *window, int entered);
	static void mousebuttonfun(GLFWwindow *window, int button, int action, int mods);
	static void scrollfun(GLFWwindow *window, double xoffset, double yoffset);

	static void dropfun(GLFWwindow *window, int count, const char **paths);
	static void windowsizefun(GLFWwindow *window, int width, int height);
	static void framebuffersizefun(GLFWwindow *window, int width, int height);
	static void windowiconifyfun(GLFWwindow *window, int iconified);
	static void windowfocusfun(GLFWwindow *window, int focused);
	static void windowposfun(GLFWwindow *window, int xpos, int ypos);
	static void windowmaximizefun(GLFWwindow *window, int maximized);

	std::string title;
	GLFWwindow *windowptr;
};