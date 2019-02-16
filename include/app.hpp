#pragma once
#include <glad/glad.h>
#include <iostream>

#include <string>

#ifdef __linux__
#include <GL/glu.h>
#else
#include <OpenGL/glu.h>
#endif

#include <GLFW/glfw3.h>

// __LINUX__, __WINDOWS__
#define __MAC_OSX__

#include "shader.hpp"
#include "sprite.hpp"
#include "util.hpp"
#include "vertexObject.hpp"

typedef struct frameInfo {
	struct time {
		double timePassedUpdate = 0;
		double timePassedRender = 0;
		int frameCount = 0;
	} time;
	bool shouldClose = false;
} frameInfo;

typedef struct appInfo {
	struct render {
		int maxFPS = 60;
	} render;

	struct logic {
		int maxUPS = 120;
	} logic;
	GLFWwindow *window;
} appInfo;

class App {
public:
	frameInfo *frame;
	appInfo *app;
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void cleanup() = 0;
	void setup();
};