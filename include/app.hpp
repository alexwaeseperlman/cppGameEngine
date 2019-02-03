#pragma once
#include <glad/glad.h>
#include <iostream>

#include "OpenGL/glu.h"

#include <GLFW/glfw3.h>

typedef struct frameInfo {
	int width = 0;
	int height = 0;
	struct window {
		int width = 0;
		int height = 0;
		int x = 0;
		int y = 0;
		bool focused = true;
		GLFWwindow *window;
	} window;
	struct mouse {
		double x = 0;
		double y = 0;
		bool leftButtonDown = false;
		bool rightButtonDown = false;
	} mouse;
	struct time {
		double timePassedUpdate = 0;
		double timePassedRender = 0;
		int frameCount = 0;
	} time;
	bool shouldClose = false;
};

typedef struct appInfo {
	struct render {
		int maxFPS = 60;
	} render;

	struct logic {
		int maxUPS = 120;
	} logic;
	GLFWwindow *window;
};

class App {
public:
	static frameInfo frame;
	static appInfo app;
	virtual bool init() = 0;
	virtual void error() = 0;
	virtual bool update() = 0;
	virtual bool render() = 0;
	virtual void cleanup();
	App();

private:
	void setup(GLFWwindow *win);
	void callUpdate();
	void callRender();

	void onWindowMoved(GLFWwindow *window, int x, int y);
	void onWindowResized(GLFWwindow *window, int width, int height);
	void onWindowFocused(GLFWwindow *window, int focused);
	void onFrameBufferResized(GLFWwindow *window, int width, int height);

	void onMouseMoved(GLFWwindow *window, double x, double y);
	void onMouseButton(GLFWwindow *window, int button, int action, int mods);

	// TODO: Add keyboard events
};