#pragma once
#include <glad/glad.h>
#include <iostream>

#include <string>

#include "OpenGL/glu.h"

#include <GLFW/glfw3.h>

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
	void (*init)(frameInfo *, appInfo *);
	void (*update)(frameInfo *, appInfo *);
	void (*render)(frameInfo *, appInfo *);
	void (*cleanup)(frameInfo *, appInfo *);

	App(void (*init)(frameInfo *, appInfo *), void (*update)(frameInfo *, appInfo *),
	    void (*render)(frameInfo *, appInfo *), void (*cleanup)(frameInfo *, appInfo *));

private:
	void setup(GLFWwindow *win);
	void callUpdate();
	void callRender();
};